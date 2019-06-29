#include "ros/ros.h"
#include "ros/console.h"

#include "pointmatcher/PointMatcher.h"
#include "pointmatcher/Timer.h"
#include "pointmatcher_ros/point_cloud.h"
#include "pointmatcher_ros/transform.h"
#include "nabo/nabo.h"
#include "eigen_conversions/eigen_msg.h"
#include "pointmatcher_ros/get_params_from_server.h"

#include <tf/transform_broadcaster.h>

#include <fstream>
#include <vector>
#include <algorithm>

#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>

#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
//#include <pcl/conversions.h>
//#include <pcl_ros/transforms.h>

#include <pcl/features/feature.h>
#include <pcl/features/fpfh.h>
#include <pcl/features/normal_3d.h>
#include <pcl/common/projection_matrix.h>

using namespace PointMatcherSupport;
using namespace pcl;
using namespace pcl::io;

using namespace std;
using namespace Eigen;

class saliencyMap
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    saliencyMap(ros::NodeHandle &n);
    ~saliencyMap();
    ros::NodeHandle& n;

    string loadMapName;
    string saveMapName;

    DP mapCloud;
    DP saveCloud;

    float normalRadius;
    float fpfhRadius;

    shared_ptr<NNS> fpfhNNS;
    shared_ptr<NNS> pointNNS;

    void process();

//    float dissimilarityMeasure(pcl::FPFHSignature33 fpfhA, pcl::FPFHSignature33 fpfhB);
    float distanceMeasure(Vector3f xyz_i, Vector3f xyz_j);
};

saliencyMap::~saliencyMap()
{}

saliencyMap::saliencyMap(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveMapName(getParam<string>("saveMapName", ".")),
    normalRadius(getParam<float>("normalRadius", 0)),
    fpfhRadius(getParam<float>("fpfhRadius", 0))
{

    // load
//    mapCloud = DP::load(loadMapName);

    // process
    this->process();

}

void saliencyMap::process()
{

    // saliency detection
    // ref: ITSC 2017, ICCV 2013


    // load cloud, fpfh for each point, using pcl
    pcl::PointCloud<pcl::PointXYZ> cloud;
    if (loadPLYFile(loadMapName, cloud) != 0)
        return;

    //normalization, proj to 0~1 unit sphere


    //Ptr
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_Ptr(new pcl::PointCloud<pcl::PointXYZ>);
    cloud_Ptr = cloud.makeShared();

    // estimate the normal
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    ne.setInputCloud (cloud_Ptr);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
    ne.setSearchMethod (tree);
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals_Ptr (new pcl::PointCloud<pcl::Normal>);
    ne.setRadiusSearch(normalRadius);
    ne.compute(*cloud_normals_Ptr);

    // fpfh
    pcl::FPFHEstimation<pcl::PointXYZ, pcl::Normal, pcl::FPFHSignature33> fpfh;
    fpfh.setInputCloud (cloud_Ptr);
    fpfh.setInputNormals (cloud_normals_Ptr);
    fpfh.setSearchMethod (tree);

    // IMPORTANT: the radius used here has to be larger than the radius used to estimate the surface normals!!!
    pcl::PointCloud<pcl::FPFHSignature33>::Ptr fpfhs (new pcl::PointCloud<pcl::FPFHSignature33> ());
    fpfh.setRadiusSearch (fpfhRadius);

    fpfh.compute (*fpfhs);

    // from pcl to ros_msg to libpointmatcher
    sensor_msgs::PointCloud2 rosCloud;
    pcl::toROSMsg(cloud, rosCloud);

    DP pmCloud = PointMatcher_ros::rosMsgToPointMatcherCloud<float>(rosCloud);

    pmCloud.addDescriptor("low-distc", PM::Matrix::Zero(1, pmCloud.features.cols()));
    pmCloud.addDescriptor("point-assoc", PM::Matrix::Zero(1, pmCloud.features.cols()));
    pmCloud.addDescriptor("high-distc", PM::Matrix::Zero(1, pmCloud.features.cols()));


    int rowLine_lowDistc = pmCloud.getDescriptorStartingRow("low-distc");
    int rowLine_pointAssoc = pmCloud.getDescriptorStartingRow("point-assoc");
    int rowLine_highDistc = pmCloud.getDescriptorStartingRow("high-distc");


    // build kd tree for fpfh descriptors
    Eigen::MatrixXf fpfh_des;
    fpfh_des.resize(33, pmCloud.features.cols());
    for(int c=0; c<pmCloud.features.cols(); c++)
        for(int r=0; r<33; r++)
         fpfh_des(r,c) = fpfhs->points[c].histogram[r];

    fpfhNNS.reset(NNS::create(fpfh_des, pmCloud.features.cols()-1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
//    pointNNS.reset(NNS::create(pmCloud.features, pmCloud.features.cols()-1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));

    // 1% point are considered
    int kSearch = pmCloud.features.cols()*0.01;

    // search the nearest
    PM::Matches matches_fpfh(
        Matches::Dists(kSearch, pmCloud.features.cols()),
        Matches::Ids(kSearch, pmCloud.features.cols())
    );

    fpfhNNS->knn(fpfh_des, matches_fpfh.ids, matches_fpfh.dists, kSearch, 0);

    for(int i=0; i<pmCloud.features.cols(); i++)
    {
        cout<<"-------------------------------------"<<endl;
        cout<<i<<endl;

        float d_L_sum = 0;

        //calc one point's low-level distinctness
        for(int j=0; j<kSearch; j++)
        {

            int id = matches_fpfh.ids(j, i);

            Vector3f xyz_i(pmCloud.features(0,i), pmCloud.features(1,i), pmCloud.features(2,i));
            Vector3f xyz_j(pmCloud.features(0,id), pmCloud.features(1,id), pmCloud.features(2,id));

            float distance_= this->distanceMeasure(xyz_i, xyz_j);

            d_L_sum += matches_fpfh.dists(j, i) / (1 + distance_);

//            cout<<j<<"  "<<matches_fpfh.dists(j, i)<<"  "<<1 + distance_<<endl;

        }

        float low_distc = 1 - std::exp(-1*d_L_sum/kSearch);

        pmCloud.descriptors(rowLine_lowDistc, i) = low_distc;
    }

    pmCloud.save(saveMapName);

}

//float saliencyMap::dissimilarityMeasure(pcl::FPFHSignature33 fpfhA, pcl::FPFHSignature33 fpfhB)
//{
//    float D=0;
//    for(int n=0; n<33; n++)
//    {
//        D += std::pow((fpfhA.histogram[n] - fpfhB.histogram[n]), 2) / (fpfhA.histogram[n] + fpfhB.histogram[n]);
//    }
//    return D;
//}

float saliencyMap::distanceMeasure(Vector3f xyz_i, Vector3f xyz_j)
{
    float dises = std::pow(xyz_i(0) - xyz_j(0), 2) + std::pow(xyz_i(1) - xyz_j(1), 2) + std::pow(xyz_i(2) - xyz_j(2), 2);
    return sqrt(dises);
}



int main(int argc, char **argv)
{

    ros::init(argc, argv, "saliencyMap");
    ros::NodeHandle n;

    saliencyMap saliencymap(n);

    // ugly code

    return 0;
}
