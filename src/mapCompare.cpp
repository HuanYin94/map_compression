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

class mapCompare
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapCompare(ros::NodeHandle &n);
    ~mapCompare();
    ros::NodeHandle& n;

    string loadMapName0;
    string loadMapName1;
    string saveIndexName;
    string saveDistsName;


    DP mapCloud0;
    DP mapCloud1;

    void process();

};

mapCompare::~mapCompare()
{}

mapCompare::mapCompare(ros::NodeHandle& n):
    n(n),
    loadMapName0(getParam<string>("loadMapName0", ".")),
    loadMapName1(getParam<string>("loadMapName1", ".")),
    saveIndexName(getParam<string>("saveIndexName", ".")),
    saveDistsName(getParam<string>("saveDistsName", "."))
{

    // load
    mapCloud0 = DP::load(loadMapName0);
    mapCloud1 = DP::load(loadMapName1);

    cout<<"JUST COMPARE"<<endl;

    // process
    this->process();

}

void mapCompare::process()
{


}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapCompare");
    ros::NodeHandle n;

    mapCompare mapcompare(n);

    // ugly code

    return 0;
}
