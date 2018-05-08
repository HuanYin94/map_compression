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
    int rowLineBin0 = mapCloud.getDescriptorStartingRow("Bin0");
    int rowLineBin1 = mapCloud.getDescriptorStartingRow("Bin1");
    int rowLineBin2 = mapCloud.getDescriptorStartingRow("Bin2");
    int rowLineBin3 = mapCloud.getDescriptorStartingRow("Bin3");

    // get view by name
    int rowLineNormal = mapCloud.getDescriptorStartingRow("normals");

    int rowLineIntensity = mapCloud.getDescriptorStartingRow("intensity");
    int rowLineProb = mapCloud.getDescriptorStartingRow("probabilityStatic");


    ofstream savePoints(saveFileName);
    for(int i=0; i<mapCloud.features.cols(); i++)
    {
        /*
        savePoints<<mapCloud.descriptors(rowLineBin0, i)<<"   "
                    <<mapCloud.descriptors(rowLineBin1, i)<<"   "
                      <<mapCloud.descriptors(rowLineBin2, i)<<"   "
                        <<mapCloud.descriptors(rowLineBin3, i)<<"   "
                          <<mapCloud.descriptors(rowLineRange, i)<<"   "
                            <<mapCloud.descriptors(rowLineHeight, i)<<"   "
            c                  <<mapCloud.descriptors(rowLineSalient, i)<<endl;
        */

        savePoints<<mapCloud.getDescriptorViewByName("normals")(0, i)<<"   "<<
               mapCloud.getDescriptorViewByName("normals")(1, i)<<"   "<<
               mapCloud.getDescriptorViewByName("normals")(2, i)<<"   "<<
               mapCloud.descriptors(rowLineIntensity, i)<<"   "<<
               mapCloud.descriptors(rowLineProb, i)<<"   "<<
               mapCloud.descriptors(rowLineRange, i)<<"   "<<
               mapCloud.descriptors(rowLineHeight, i)<<"   "<<
               mapCloud.descriptors(rowLineSalient, i)<<"   "<<endl;


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
