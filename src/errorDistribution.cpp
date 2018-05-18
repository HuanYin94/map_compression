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

class errorDistribution
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    errorDistribution(ros::NodeHandle &n);
    ~errorDistribution();
    ros::NodeHandle& n;

    string loadMapName;

    DP mapCloud;
    DP salientCloud;
    DP salientResultsCloud;

    void process();

};

errorDistribution::~errorDistribution()
{}

errorDistribution::errorDistribution(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", "."))
{

    // load
    mapCloud = DP::load(loadMapName);

    cout<<"JUST SHOW"<<endl;

    // process
    this->process();

}

void errorDistribution::process()
{


}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "errordistribution");
    ros::NodeHandle n;

    errorDistribution errordistribution(n);

    // ugly code

    return 0;
}
