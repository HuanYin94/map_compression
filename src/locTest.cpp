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

class mapCheck
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

public:
    mapCheck(ros::NodeHandle &n);
    ~mapCheck();
    ros::NodeHandle& n;

    string loadMapName;
    string loadTrajName;

    DP mapCloud;

    int mapLength;

    vector<vector<double>> initPoses;

    void process();

};

mapCheck::~mapCheck()
{}

mapCheck::mapCheck(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    loadTrajName(getParam<string>("loadTrajName", ".")),
    mapLength(getParam<int>("mapLength", 0))
{

    // load
    mapCloud = DP::load(loadMapName);

    // read initial transformation
    int x, y;
    double temp;
    vector<double> test;
    ifstream in(loadTrajName);
    if (!in) {
        cout << "Cannot open file.\n";
    }
    for (y = 0; y < mapLength; y++) {
        test.clear();
    for (x = 0; x < 16; x++) {
      in >> temp;
      test.push_back(temp);
    }
      initPoses.push_back(test);
    }
    in.close();

    cout<<"start locTest"<<endl;

    // process
    this->process();

}

void mapCheck::process()
{

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapCheck");
    ros::NodeHandle n;

    mapCheck mapcheck(n);

    // ugly code

    return 0;
}
