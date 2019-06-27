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

class loadFinalResults
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;

public:
    loadFinalResults(ros::NodeHandle &n);
    ~loadFinalResults();
    ros::NodeHandle& n;

    string loadResultName;
    string loadMapName;
    string saveCloudName;

    DP mapCloud;

};

loadFinalResults::~loadFinalResults()
{}

loadFinalResults::loadFinalResults(ros::NodeHandle& n):
    n(n),
    loadResultName(getParam<string>("loadResultName", ".")),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveCloudName(getParam<string>("saveCloudName", "."))
{
    //map
    mapCloud = DP::load(loadMapName);
    mapCloud.addDescriptor("salient_predicted", PM::Matrix::Zero(1, mapCloud.features.cols()));
    int rowLineResults = mapCloud.getDescriptorStartingRow("salient_predicted");

    //results
    // read poses
    int temp;
    vector<int> results;
    ifstream in(loadResultName);
    for (int y = 0; y < 9999999; y++) {
        in >> temp;
      results.push_back(temp);
    }
    in.close();

    for(int m=0; m<mapCloud.features.cols(); m++)
    {
        if(results.at(m) == 1)
            mapCloud.descriptors(rowLineResults, m) = 1;
        else
            mapCloud.descriptors(rowLineResults, m) = 0;
    }

     mapCloud.save(saveCloudName);


}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "loadfinalresults");
    ros::NodeHandle n;

    loadFinalResults loadfinalresults(n);

    // ugly code

    return 0;
}
