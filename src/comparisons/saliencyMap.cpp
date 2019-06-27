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


    // load , fpfh for each point, using pcl
    pcl::PointCloud<pcl::PointXYZ> cloud;
    if (loadPLYFile(filename, cloud) != 0) return false;



}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "saliencyMap");
    ros::NodeHandle n;

    saliencyMap saliencymap(n);

    // ugly code

    return 0;
}
