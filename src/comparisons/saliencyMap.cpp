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
#include <math.h>

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

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/common/common.h>

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

    string loadMapDir;
    string saveSaliencyDir;
    int subMapCnt;

    DP mapCloud;
    DP saveCloud;

    float normalRadius;
    float fpfhRadius;
    float accRatio;
    float highRatio_pointAssoc;
    float sigma;
    float highRatio_highDistinct;
    float highRadius;
    int highSearchNum;

    shared_ptr<NNS> fpfhNNS;
    shared_ptr<NNS> pointNNS_1;
    shared_ptr<NNS> pointNNS_2;


    void process(string loadMapName, string saveSaliencyName);

    float ChiSquareMeasure(pcl::FPFHSignature33 fpfhA, pcl::FPFHSignature33 fpfhB);
    float distanceMeasure(Vector3f xyz_i, Vector3f xyz_j);
};

saliencyMap::~saliencyMap()
{}

saliencyMap::saliencyMap(ros::NodeHandle& n):
    n(n),
    normalRadius(getParam<float>("normalRadius", 0)),
    fpfhRadius(getParam<float>("fpfhRadius", 0)),
    accRatio(getParam<float>("accRatio", 0)),
    highRatio_pointAssoc(getParam<float>("highRatio_pointAssoc", 0)),
    sigma(getParam<float>("sigma", 0)),
    highRatio_highDistinct(getParam<float>("highRatio_highDistinct", 0)),
    highRadius(getParam<float>("highRadius", 0)),
    loadMapDir(getParam<string>("loadMapDir", ".")),
    saveSaliencyDir(getParam<string>("saveSaliencyDir", ".")),
    subMapCnt(getParam<int>("subMapCnt", 0))
{
    // process

    for(int i=0; i<subMapCnt; i++)
    {
        string loadMapName = loadMapDir + to_string(i) + ".ply";
        string saveSaliencyName = saveSaliencyDir + to_string(i) + ".txt";

        this->process(loadMapName, saveSaliencyName);
    }

}




void saliencyMap::process(string loadMapName, string saveSaliencyName)
{
    cout<<loadMapName<<endl;

    // saliency detection
    // ref: ITSC 2017, ICCV 2013
    // change the kd treeiun metric

    // load cloud, fpfh for each point, using pcl
    pcl::PointCloud<pcl::PointXYZ> cloud;
    if (loadPLYFile(loadMapName, cloud) != 0)
        return;

    double t0 = ros::Time::now().toSec();

    //normalization, proj to 0~1 unit sphere
    pcl::PointXYZ min;
    pcl::PointXYZ max;
    pcl::getMinMax3D(cloud, min, max);
    float max_[3] = {max.x, max.y, max.z};
    float min_[3] = {min.x, min.y, min.z};

    float max_xyz = *std::max_element(max_, max_+3);
    float min_xyz = *std::min_element(min_, min_+3);

    for(int i=0; i<cloud.points.size(); i++)
    {
        cloud.points[i].x = (cloud.points[i].x - min_xyz) / (max_xyz-min_xyz);
        cloud.points[i].y = (cloud.points[i].y - min_xyz) / (max_xyz-min_xyz);
        cloud.points[i].z = (cloud.points[i].z - min_xyz) / (max_xyz-min_xyz);
    }


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

    // from ros_msg to DataPoints (libpointmatcher)
    DP pmCloud = PointMatcher_ros::rosMsgToPointMatcherCloud<float>(rosCloud);

    pmCloud.addDescriptor("low-distc", PM::Matrix::Zero(1, pmCloud.features.cols()));
    pmCloud.addDescriptor("point-assoc", PM::Matrix::Zero(1, pmCloud.features.cols()));
    pmCloud.addDescriptor("high-distc", PM::Matrix::Zero(1, pmCloud.features.cols()));
    pmCloud.addDescriptor("saliency", PM::Matrix::Zero(1, pmCloud.features.cols()));

    int rowLine_lowDistc = pmCloud.getDescriptorStartingRow("low-distc");
    int rowLine_pointAssoc = pmCloud.getDescriptorStartingRow("point-assoc");
    int rowLine_highDistc = pmCloud.getDescriptorStartingRow("high-distc");
    int rowLine_saliency = pmCloud.getDescriptorStartingRow("saliency");



    double t1 = ros::Time::now().toSec();
    cout<<"Prepare Time Cost (s):   "<<t1-t0<<endl;






    ///calc one point's low-level distinctness
    /// use l2 distance in kd-tree in this case

    // build kd tree for fpfh descriptors
    Eigen::MatrixXf fpfh_des;
    fpfh_des.resize(33, pmCloud.features.cols());
    for(int c=0; c<pmCloud.features.cols(); c++)
        for(int r=0; r<33; r++)
         fpfh_des(r,c) = fpfhs->points[c].histogram[r];

    fpfhNNS.reset(NNS::create(fpfh_des, pmCloud.features.cols()-1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));

    // 1% point are considered
    int kSearch = pmCloud.features.cols()*accRatio;

    // search the nearest
    PM::Matches matches_fpfh(
        Matches::Dists(kSearch, pmCloud.features.cols()),
        Matches::Ids(kSearch, pmCloud.features.cols())
    );

    fpfhNNS->knn(fpfh_des, matches_fpfh.ids, matches_fpfh.dists, kSearch, 0);

    float low_distinc_array[pmCloud.features.cols()]; //for the next session

    for(int i=0; i<pmCloud.features.cols(); i++)
    {

        float d_L_sum = 0;

        for(int j=0; j<kSearch; j++)
        {

            int id = matches_fpfh.ids(j, i);

            Vector3f xyz_i(pmCloud.features(0,i), pmCloud.features(1,i), pmCloud.features(2,i));
            Vector3f xyz_j(pmCloud.features(0,id), pmCloud.features(1,id), pmCloud.features(2,id));

            float Euc_distance = this->distanceMeasure(xyz_i, xyz_j);
            float Chi_distance = this->ChiSquareMeasure(fpfhs->points[i], fpfhs->points[id]);

            d_L_sum += Chi_distance / (1 + Euc_distance);

//            cout<<j<<"  "<<matches_fpfh.dists(j, i)<<"  "<<1 + distance_<<endl;

            if(d_L_sum != d_L_sum)
            {
                cout<<""<<endl;
                break;

            }

        }

        float low_distc = 1 - std::exp(-1*d_L_sum/kSearch);

        pmCloud.descriptors(rowLine_lowDistc, i) = low_distc;

        low_distinc_array[i] = low_distc;
    }

    double t2 = ros::Time::now().toSec();
    cout<<"Low-Dis Time Cost (s):   "<<t2-t1<<endl;






    ///calc one point's point association
    // change a point according to the clostest highest low-distinct

    // sort the low-distc, up
    std::sort(low_distinc_array, low_distinc_array + pmCloud.features.cols());
    // top high low_distinct points
    int high_thresh_num_Assoc = (1-highRatio_pointAssoc)*pmCloud.features.cols();
    float high_thresh_Assoc = low_distinc_array[high_thresh_num_Assoc];

    DP pmCloud_high = pmCloud.createSimilarEmpty();

    int cnt = 0;
    for(int i=0; i<pmCloud.features.cols(); i++)
    {
        if(pmCloud.descriptors(rowLine_lowDistc, i) > high_thresh_Assoc)
        {
            pmCloud_high.setColFrom(cnt, pmCloud, i);
            cnt++;
        }
    }
    pmCloud_high.conservativeResize(cnt);

    pointNNS_1.reset(NNS::create(pmCloud_high.features, pmCloud_high.features.cols()-1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));

    PM::Matches matches_xyz_1(
        Matches::Dists(1, pmCloud.features.cols()),
        Matches::Ids(1, pmCloud.features.cols())
    );

    pointNNS_1->knn(pmCloud.features, matches_xyz_1.ids, matches_xyz_1.dists, 1, 0);

    for(int i=0; i<pmCloud.features.cols(); i++)
    {
        float D_foci = pmCloud_high.descriptors(rowLine_lowDistc, matches_xyz_1.ids(0,i));
        float point_assoc = D_foci * std::exp(-1*matches_xyz_1.dists(0,i) / (2*sigma*sigma));

        pmCloud.descriptors(rowLine_pointAssoc, i) = point_assoc;
    }

    double t3 = ros::Time::now().toSec();
    cout<<"Point-Assoc Time Cost (s):   "<<t3-t2<<endl;







    ///calc one point's high-level distinctness

    // top high low_distinct points
    // 10% in the paper, change highest themselves
    int high_thresh_num_High = (1-highRatio_highDistinct)*pmCloud.features.cols();
    float high_thresh_High = low_distinc_array[high_thresh_num_High];

    pointNNS_2.reset(NNS::create(pmCloud.features, pmCloud.features.cols()-1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));

    PM::Matches matches_xyz_2(
        Matches::Dists(this->highSearchNum, pmCloud.features.cols()),
        Matches::Ids(this->highSearchNum, pmCloud.features.cols())
    );

    pointNNS_2->knn(pmCloud.features, matches_xyz_2.ids, matches_xyz_2.dists, this->highSearchNum, 0);

    for(int i=0; i<pmCloud.features.cols(); i++)
    {
        if(pmCloud.descriptors(rowLine_lowDistc, i) <= high_thresh_High)
        {
            continue;
        }

        float d_H_sum = 0;
        int neighbor_cnt = 0;

        for(int j=0; j<highSearchNum; j++)
        {
            float Euc_distance_ = std::sqrt(matches_xyz_2.dists(j, i));

            if(Euc_distance_ > this->highRadius)
            {
                break;
            }

            float ChiSquare = this->ChiSquareMeasure(fpfhs->points[i], fpfhs->points[j]);

            d_H_sum += ChiSquare * std::log(1 + Euc_distance_);

            neighbor_cnt++;
        }

        float high_distinc = 1 - std::exp(-1*d_H_sum/neighbor_cnt);

        pmCloud.descriptors(rowLine_highDistc, i) = high_distinc;

    }


    double t4 = ros::Time::now().toSec();
    cout<<"High-Sid Cost (s):   "<<t4-t3<<endl;



    ///calc one point's saliency
    ofstream saver(saveSaliencyName);

    for(int i=0; i<pmCloud.features.cols(); i++)
    {
        float a = pmCloud.descriptors(rowLine_lowDistc,i);
        float b = pmCloud.descriptors(rowLine_pointAssoc,i);
        float c = pmCloud.descriptors(rowLine_highDistc,i);
        pmCloud.descriptors(rowLine_saliency, i) = 0.5*(a+b) + 0.5*c;

        saver << a << "  "
                << b << "  "
                   << c << "  "
                      << 0.5*(a+b) + 0.5*c << endl;
    }
    saver.close();

}

float saliencyMap::ChiSquareMeasure(pcl::FPFHSignature33 fpfhA, pcl::FPFHSignature33 fpfhB)
{
    float D=0;
    for(int n=0; n<33; n++)
    {
        if((fpfhA.histogram[n] + fpfhB.histogram[n]) == 0)
            continue;

        D += std::pow((fpfhA.histogram[n] - fpfhB.histogram[n]), 2) / (fpfhA.histogram[n] + fpfhB.histogram[n]);
    }
    return D;
}

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
