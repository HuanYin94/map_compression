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

class Saver
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    Saver(ros::NodeHandle &n);
    ~Saver();
    ros::NodeHandle& n;

    string loadMapName;
    string saveFileName;

    DP mapCloud;

    void process();

};

Saver::~Saver()
{}

Saver::Saver(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveFileName(getParam<string>("saveFileName", "."))
{

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();

}

void Saver::process()
{
    int rowLineSalient = mapCloud.getDescriptorStartingRow("salient");
    int rowLineRange = mapCloud.getDescriptorStartingRow("Range");
    int rowLineHeight = mapCloud.getDescriptorStartingRow("Height");
    int rowLineOrientation = mapCloud.getDescriptorStartingRow("Orientation");
    int rowLineNormal = mapCloud.getDescriptorStartingRow("normals");
    int rowLineIntensity = mapCloud.getDescriptorStartingRow("intensity");
    int rowLineDensities = mapCloud.getDescriptorStartingRow("densities");
    int rowLineSalientResults = mapCloud.getDescriptorStartingRow("salient_results");

    // normal ones
/*
    ofstream savePoints(saveFileName);
    for(int m=0; m<mapCloud.features.cols(); m++)
    {
        savePoints<<mapCloud.descriptors(rowLineNormal,m)<<"   "
                <<mapCloud.descriptors(rowLineNormal+1,m)<<"   "
                <<mapCloud.descriptors(rowLineNormal+2,m)<<"   "
                <<mapCloud.descriptors(rowLineIntensity,m)<<"   "
                <<mapCloud.descriptors(rowLineDensities,m)<<"   "
                <<mapCloud.descriptors(rowLineRange,m)<<"   "
                <<mapCloud.descriptors(rowLineHeight,m)<<"   "
                <<mapCloud.descriptors(rowLineOrientation,m)<<"   "
                <<mapCloud.descriptors(rowLineSalient,m)<<endl;


        savePoints.flush();
    }
    savePoints.close();
 */

    // save the xyz only for detection results
    ofstream savePoints(saveFileName);
    for(int m=0; m<mapCloud.features.cols(); m++)
    {
        if(mapCloud.descriptors(rowLineSalientResults,m) == 1)
            savePoints<<mapCloud.features(0,m)<<"   "
                      <<mapCloud.features(1,m)<<"   "
                      <<mapCloud.features(2,m)<<endl;

        savePoints.flush();
    }
    savePoints.close();


}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "saver");
    ros::NodeHandle n;

    Saver saver(n);

    // ugly code

    return 0;
}
