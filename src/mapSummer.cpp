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

class mapSummer
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapSummer(ros::NodeHandle &n);
    ~mapSummer();
    ros::NodeHandle& n;

    string loadMapName0;
    string loadMapName1;
    string saveMapName;

    DP mapCloud0;
    DP mapCloud1;

    void process();

};

mapSummer::~mapSummer()
{}

mapSummer::mapSummer(ros::NodeHandle& n):
    n(n),
    loadMapName0(getParam<string>("loadMapName0", ".")),
    loadMapName1(getParam<string>("loadMapName1", ".")),
    saveMapName(getParam<string>("saveMapName", "."))
{

    // load
    mapCloud0 = DP::load(loadMapName0);
    mapCloud1 = DP::load(loadMapName1);

    cout<<"JUST COMPARE"<<endl;

    // process
    this->process();

}

void mapSummer::process()
{
    int rowLineSession = mapCloud0.getDescriptorStartingRow("session");

    DP saveSumMap = mapCloud0;

    for(int m=0; m<mapCloud0.features.cols(); m++)
    {
        int s = mapCloud0.descriptors(rowLineSession, m) + mapCloud1.descriptors(rowLineSession, m);
        saveSumMap.descriptors(rowLineSession, m) =s;
    }

    saveSumMap.save(saveMapName);

}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapsummer");
    ros::NodeHandle n;

    mapSummer mapsummer(n);

    // ugly code

    return 0;
}
