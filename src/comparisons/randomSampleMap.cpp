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

#include <time.h>

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
    saveMapName(getParam<string>("saveMapName", "."))
{

    // load
    mapCloud = DP::load(loadMapName);

    totalPointsNum = mapCloud.features.cols();

    // process
    this->process();

}

void randomSampleMap::process()
{
    srand(time(NULL));

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

    saveCloud = mapCloud.createSimilarEmpty();
    for(int i=0; i<randomIndex.size(); i++)
    {
        saveCloud.setColFrom(i, mapCloud, randomIndex.at(i));
    }
    saveCloud.conservativeResize(randomIndex.size());

    saveCloud.save(saveMapName);

    cout<<"Size:  "<<randomIndex.size()<<endl;

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "randomSampleMap");
    ros::NodeHandle n;

    randomSampleMap randomSamplemap(n);

    // ugly code

    return 0;
}
