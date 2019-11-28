#include <iostream>
#include <string>

#include "pointmatcher/PointMatcher.h"
#include "pointmatcher_ros/point_cloud.h"
#include "pointmatcher_ros/get_params_from_server.h"

#include "tf/LinearMath/Transform.h"
#include "tf/transform_datatypes.h"

#include <fstream>
#include <vector>
#include <algorithm>

#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include <numeric>


using namespace std;
using namespace PointMatcherSupport;

class viewer
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;

public:
    viewer(ros::NodeHandle &n);
    ~viewer();
    ros::NodeHandle& n;

    string loadMapName;

    DP mapCloud;

    void process();

    ros::Publisher mapCloudPub;
    ros::Publisher normalPub;

    visualization_msgs::MarkerArray normalsArray;
    int stepNum;

    double arrowLength;

};

viewer::~viewer()
{}

viewer::viewer(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    stepNum(getParam<int>("stepNum", 1)),
    arrowLength(getParam<double>("arrowLength", 0))
{

    mapCloud = DP::load(loadMapName);

    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("mapCloud", 1, true);
    normalPub = n.advertise<visualization_msgs::MarkerArray>("normals", 1, true);

    // set markers
    cout<<"begin setting"<<endl;
    int markerId = 0;

    for(int m=0; m<mapCloud.features.cols(); m=m+stepNum)
    {
        visualization_msgs::Marker marker;

        marker.header.frame_id = "world";
        marker.header.stamp = ros::Time();
        marker.ns = "my_namespace";
        marker.id = markerId;
        marker.type = visualization_msgs::Marker::ARROW;
        marker.action = visualization_msgs::Marker::ADD;
        marker.scale.x = 1.0;
        marker.scale.y = 0.2;
        marker.scale.z = 0.2;
        marker.color.a = 0.5; // Don't forget to set the alpha!
        marker.color.r = 1.0;
        marker.color.g = 0.0;
        marker.color.b = 0.0;

        // position
        marker.pose.position.x = mapCloud.features(0,m);
        marker.pose.position.y = mapCloud.features(1,m);
        marker.pose.position.z = mapCloud.features(2,m);

        // orientation
        tf::Vector3 axis(mapCloud.getDescriptorViewByName("normals")(0, m),mapCloud.getDescriptorViewByName("normals")(1, m),mapCloud.getDescriptorViewByName("normals")(2, m));
        tf::Vector3 marker_axis(1, 0, 0);
        tf::Quaternion qt(marker_axis.cross(axis.normalize()), marker_axis.angle(axis.normalize()));
        geometry_msgs::Quaternion quat_msg;
        tf::quaternionTFToMsg(qt, quat_msg);
        marker.pose.orientation = quat_msg;

        normalsArray.markers.push_back(marker);

        markerId ++;
    }
    cout<<"set finish"<<endl;

    cout<<"Size:    "<<normalsArray.markers.size()<<endl;

    for(int i=0;i<10;i++)
    {
        cout<<i<<endl;
        mapCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "world", ros::Time(0)));
        normalPub.publish(normalsArray);
        ros::Duration(0.5).sleep();
    }


}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "viewer_");
    ros::NodeHandle n;

    viewer viewer(n);

    return 0;
}
