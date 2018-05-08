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

class mapCutter
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapCutter(ros::NodeHandle &n);
    ~mapCutter();
    ros::NodeHandle& n;

    string loadMapName;
    ros::Publisher mapCloudPub;
    ros::Publisher trainCloudPub;
    ros::Publisher testCloudPub;

    DP mapCloud;
    DP trainCloud;
    DP testCloud;
    int cutX;
    int cutY;
    int sessionTh;

    string saveTrainName;
    string saveTestName;

    void wait();
    void process();

};

mapCutter::~mapCutter()
{}

mapCutter::mapCutter(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    cutX(getParam<int>("cutX", 0)),
    cutY(getParam<int>("cutY", 0)),
    saveTrainName(getParam<string>("saveTrainName", ".")),
    saveTestName(getParam<string>("saveTestName", "."))
{
    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("mapCloud", 2, true);
    trainCloudPub = n.advertise<sensor_msgs::PointCloud2>("trainCloud", 2, true);
    testCloudPub = n.advertise<sensor_msgs::PointCloud2>("testCloud", 2, true);

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();

    trainCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(trainCloud, "global", ros::Time(0)));
    mapCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "global", ros::Time(0)));
    testCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(testCloud, "global", ros::Time(0)));

    this->wait();
}

void mapCutter::wait()
{
    ros::Rate r(1);

    while(ros::ok())
    {
    }
}

void mapCutter::process()
{

    trainCloud = mapCloud.createSimilarEmpty();
    testCloud = mapCloud.createSimilarEmpty();
    int trainCount = 0;
    int testCount = 0;

    int rowLineX = mapCloud.getFeatureStartingRow("x");
    int rowLineZ = mapCloud.getFeatureStartingRow("y");

    for(int i=0; i<mapCloud.features.cols(); i++)
    {
        if(mapCloud.features(rowLineX,i) < cutX &&
           mapCloud.features(rowLineZ,i) > cutY)
        {
            trainCloud.setColFrom(trainCount, mapCloud, i);
            trainCount++;
        }
        else
        {
            testCloud.setColFrom(testCount, mapCloud, i);
            testCount++;
        }

    }

    trainCloud.conservativeResize(trainCount);
    testCloud.conservativeResize(testCount);

    ///save
    ///
    trainCloud.save(saveTrainName);
    testCloud.save(saveTestName);

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapCutter");
    ros::NodeHandle n;

    mapCutter mapcutter(n);

    // ugly code

    return 0;
}
