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

class mapCompresser
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapCompresser(ros::NodeHandle &n);
    ~mapCompresser();
    ros::NodeHandle& n;

    string loadMapName;
    string saveMapName;
    string desName;

    DP mapCloud;

    void process();

};

mapCompresser::~mapCompresser()
{}

mapCompresser::mapCompresser(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveMapName(getParam<string>("saveMapName", ".")),
    desName(getParam<string>("desName", "."))
{

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();
}

void mapCompresser::process()
{

    int rowLineSalientResults = mapCloud.getDescriptorStartingRow(desName);

    DP salientMap;
    int cnt = 0;
    salientMap = mapCloud.createSimilarEmpty();
    for(int m=0; m<mapCloud.features.cols(); m++)
    {
        if(mapCloud.descriptors(rowLineSalientResults, m) == 1)
        {
            salientMap.setColFrom(cnt, mapCloud, m);
            cnt++;
        }
    }
    salientMap.conservativeResize(cnt);

    salientMap.save(saveMapName);

    cout<<"Origin:  "<<mapCloud.features.cols()<<endl;
    cout<<"Saved:   "<<salientMap.features.cols()<<endl;
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapCompresser");
    ros::NodeHandle n;

    mapCompresser mapcompresser(n);

    return 0;
}
