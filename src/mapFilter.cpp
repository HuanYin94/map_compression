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

#include <numeric>

using namespace std;
using namespace PointMatcherSupport;

class mapFilter
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;

public:
    mapFilter(ros::NodeHandle &n);
    ~mapFilter();
    ros::NodeHandle& n;

    ros::Publisher mapCloudPub;
    ros::Publisher filterCloudPub;

    string loadMapName;
    string saveMapName;
    string mapPostFilters;

    PM::DataPointsFilters mapFilters;

    DP mapCloud;
    DP filterCloud;

    void process();

};

mapFilter::~mapFilter()
{}

mapFilter::mapFilter(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveMapName(getParam<string>("saveMapName", ".")),
    mapPostFilters(getParam<string>("mapPostFilters", "."))
{
    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("mapCloud", 2, true);
    filterCloudPub = n.advertise<sensor_msgs::PointCloud2>("filterCloud", 2, true);

    // load
    mapCloud = DP::load(loadMapName);
    mapCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "global", ros::Time(0)));

    // process
    this->process();

    filterCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(filterCloud, "global", ros::Time(0)));

    filterCloud.save(saveMapName);
}

void mapFilter::process()
{

    ifstream mapFilterss(mapPostFilters);
    mapFilters = PM::DataPointsFilters(mapFilterss);

    cout<<"Before filter count:  "<<mapCloud.features.cols()<<endl;

    mapFilters.apply(mapCloud);
    filterCloud = mapCloud;

    cout<<"After that:  "<<filterCloud.features.cols()<<endl;

    // descriports process
    // remove useless
    if(filterCloud.descriptorExists("ring"))
        filterCloud.removeDescriptor("ring");

    // cout for test
    cout<<"Desp num:   "<<filterCloud.descriptors.rows()<<endl;
    cout<<"eigen values:   "<<filterCloud.getDescriptorStartingRow("eigValues")<<endl;
    cout<<"--rows:  "<<filterCloud.getDescriptorDimension("eigValues")<<endl;
    cout<<"intensity:   "<<filterCloud.getDescriptorStartingRow("intensity")<<endl;
    cout<<"density:   "<<filterCloud.getDescriptorStartingRow("densities")<<endl;
    cout<<"normals:   "<<filterCloud.getDescriptorStartingRow("normals")<<endl;
    cout<<"--rows:  "<<filterCloud.getDescriptorDimension("normals")<<endl;

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapFilter");
    ros::NodeHandle n;

    mapFilter mapfilter(n);

    // ugly code

    return 0;
}
