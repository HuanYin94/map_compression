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

class saliencyMap
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;

public:
    saliencyMap(ros::NodeHandle &n);
    ~saliencyMap();
    ros::NodeHandle& n;

    string loadMapName;
    string saveMapName;

    DP mapCloud;
    DP saveCloud;

    void process();

};

saliencyMap::~saliencyMap()
{}

saliencyMap::saliencyMap(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveMapName(getParam<string>("saveMapName", "."))
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
    if (loadPLYFile(loadMapName, cloud) != 0) return false;

    //Ptr
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_Ptr(new pcl::PointCloud<pcl::PointXYZ>);
    cloud_Ptr = cloud.makeShared();

    // estimate the normal
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
    ne.setInputCloud (cloud_Ptr);
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
    ne.setSearchMethod (tree);
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normals_Ptr (new pcl::PointCloud<pcl::Normal>);
    ne.setRadiusSearch(0.5);
    ne.compute(*cloud_normals_Ptr);

    // fpfh
    pcl::FPFHEstimation<pcl::PointXYZ, pcl::Normal, pcl::FPFHSignature33> fpfh;
    fpfh.setInputCloud (cloud_Ptr);
    fpfh.setInputNormals (cloud_normals_Ptr);
    fpfh.setSearchMethod (tree);

    // IMPORTANT: the radius used here has to be larger than the radius used to estimate the surface normals!!!
    pcl::PointCloud<pcl::FPFHSignature33>::Ptr fpfhs (new pcl::PointCloud<pcl::FPFHSignature33> ());
    fpfh.setRadiusSearch (1.0);

    fpfh.compute (*fpfhs);

    cout<<<<endl;

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "saliencyMap");
    ros::NodeHandle n;

    saliencyMap saliencymap(n);

    // ugly code

    return 0;
}
