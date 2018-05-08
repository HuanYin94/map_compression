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

class randomSampleMap
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    randomSampleMap(ros::NodeHandle &n);
    ~randomSampleMap();
    ros::NodeHandle& n;

    string loadMapName;
    ros::Publisher mapCloudPub;
    string saveMapName;

    DP mapCloud;
    int keepPointsNum;
    int totalPointsNum;
    DP saveCloud;

    void wait();
    void process();

};

randomSampleMap::~randomSampleMap()
{}

randomSampleMap::randomSampleMap(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    keepPointsNum(getParam<int>("keepPointsNum", 0)),
    totalPointsNum(getParam<int>("totalPointsNum", 0)),
    saveMapName(getParam<string>("saveMapName", "."))
{
    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("mapCloud", 2, true);

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();

    mapCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(saveCloud, "global", ros::Time(0)));

    this->wait();
}

void randomSampleMap::wait()
{
    ros::Rate r(1);

    while(ros::ok())
    {
    }
}

void randomSampleMap::process()
{
    vector<int> randomIndex;
    for(int m=0; m<this->keepPointsNum;  )
    {
        int index = (rand() % (totalPointsNum-0))+ 0;
        int findNum = std::count(randomIndex.begin(), randomIndex.end(), index);

        if(findNum>0)
        {
            continue;
        }
        else
        {
            randomIndex.push_back(index);
            m++;
        }

    }

    cout<<"Size:  "<<randomIndex.size()<<endl;

    saveCloud = mapCloud.createSimilarEmpty();
    for(int i=0; i<randomIndex.size(); i++)
    {
        saveCloud.setColFrom(i, mapCloud, randomIndex.at(i));
    }
    saveCloud.conservativeResize(randomIndex.size());

    saveCloud.save(saveMapName);

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "randomSampleMap");
    ros::NodeHandle n;

    randomSampleMap randomSamplemap(n);

    // ugly code

    return 0;
}
