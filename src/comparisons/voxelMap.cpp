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

using namespace PointMatcherSupport;
using namespace pcl;
using namespace pcl::io;

using namespace std;

class voxelMap
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;

public:
    voxelMap(ros::NodeHandle &n);
    ~voxelMap();
    ros::NodeHandle& n;

    string loadMapName;
    string saveMapName;

    DP mapCloud;
    DP saveCloud;

    double leafSize;

    void process();

};

voxelMap::~voxelMap()
{}

voxelMap::voxelMap(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveMapName(getParam<string>("saveMapName", ".")),
    leafSize(getParam<double>("leafSize", 0))
{

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();

}

void voxelMap::process()
{
    pcl::PCLPointCloud2::Ptr pcl_map (new pcl::PCLPointCloud2 ());
    pcl_conversions::toPCL(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "world", ros::Time(0)), *pcl_map);

    pcl::PCLPointCloud2::Ptr cloud_filtered (new pcl::PCLPointCloud2 ());

    // Create the filtering object
    pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
    sor.setInputCloud (pcl_map);

    cout<<"start"<<endl;

    sor.setLeafSize (leafSize, leafSize, leafSize);
    sor.filter (*cloud_filtered);

    cout<<"sampled..."<<endl;

//    // use ros API
    sensor_msgs::PointCloud2 bridge_ros_msg;
    pcl_conversions::fromPCL(*cloud_filtered, bridge_ros_msg);
    DP newMapCloud = PointMatcher_ros::rosMsgToPointMatcherCloud<float>(bridge_ros_msg);

    cout<<"Before:   "<<mapCloud.features.cols()<<endl;
    cout<<"Remains:  "<<newMapCloud.features.cols()<<endl;

    newMapCloud.save(saveMapName);
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "voxelMap");
    ros::NodeHandle n;

    voxelMap voxelmap(n);

    // ugly code

    return 0;
}
