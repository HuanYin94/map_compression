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

class mapSessioner
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapSessioner(ros::NodeHandle &n);
    ~mapSessioner();
    ros::NodeHandle& n;

    string loadMapName;
    string saveMapName;

    DP mapCloud;
    int sessionCut;

    void wait();
    void process();

};

mapSessioner::~mapSessioner()
{}

mapSessioner::mapSessioner(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    sessionCut(getParam<int>("sessionCut", 0)),
    saveMapName(getParam<string>("saveMapName", "."))
{

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();

    this->wait();
}

void mapSessioner::wait()
{
    ros::Rate r(1);

    while(ros::ok())
    {
    }
}

void mapSessioner::process()
{
    /**Session Trick?**/

    int rowLine = mapCloud.getDescriptorStartingRow("session");

    for(int m=0; m<mapCloud.features.cols(); m++)
    {
        if(mapCloud.descriptors(rowLine, m) > this->sessionCut)
        {
            mapCloud.descriptors(rowLine, m) = this->sessionCut;
        }
    }

    mapCloud.save(this->saveMapName);

    cout<<"finished"<<endl;

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapSessioner");
    ros::NodeHandle n;

    mapSessioner mapsessioner(n);

    // ugly code

    return 0;
}
