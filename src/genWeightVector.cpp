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

class genWeightVector
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
public:
    genWeightVector(ros::NodeHandle &n);
    ~genWeightVector();
    ros::NodeHandle& n;

    string loadMapName;
    string saveWeightName;
    DP mapCloud;

    void process();

};

genWeightVector::~genWeightVector()
{}

genWeightVector::genWeightVector(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveWeightName(getParam<string>("saveWeightName", "."))
{
    // load
    mapCloud = DP::load(loadMapName);
    this->process();
}

void genWeightVector::process()
{
    int rowLineSession = mapCloud.getDescriptorStartingRow("session");

    ofstream output;
    output.open(saveWeightName);

    for(int m=0; m<mapCloud.features.cols(); m++)
    {
//        cout<<m<<endl;
        output << mapCloud.descriptors(rowLineSession, m) << endl;
    }

    output.close();
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "genWeightVector");
    ros::NodeHandle n;

    genWeightVector genWeightVector_(n);

    // ugly code

    return 0;
}
