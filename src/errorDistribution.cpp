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
    string saveDisName;

    DP mapCloud;
    DP salientCloud;
    DP salientResultsCloud;

    shared_ptr<NNS> distanceNNS;

    void process();

};

errorDistribution::~errorDistribution()
{}

errorDistribution::errorDistribution(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveDisName(getParam<string>("saveDisName", "."))
{

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();

}

void errorDistribution::process()
{
    int rowLineSalient = mapCloud.getDescriptorStartingRow("salient");

    // prediction == results
    int rowLineSalientResults = mapCloud.getDescriptorStartingRow("salient_predicted");

    // saliency
    int cntSalient = 0;
    salientCloud = mapCloud.createSimilarEmpty();
    for(int s=0; s<mapCloud.features.cols(); s++)
    {
        if(mapCloud.descriptors(rowLineSalient, s) == 1)
        {
            salientCloud.setColFrom(cntSalient, mapCloud, s);
            cntSalient++;
        }
    }
    salientCloud.conservativeResize(cntSalient);

    //detection of saliency
    int cntSalientResults = 0;
    salientResultsCloud = mapCloud.createSimilarEmpty();
    for(int sr=0; sr<mapCloud.features.cols(); sr++)
    {
        if(mapCloud.descriptors(rowLineSalientResults, sr) == 1)
        {
            salientResultsCloud.setColFrom(cntSalientResults, mapCloud, sr);
            cntSalientResults++;
        }
    }
    salientResultsCloud.conservativeResize(cntSalientResults);

    cout<<"salient count:   "<<salientCloud.features.cols()<<endl;
    cout<<"results count:   "<<salientResultsCloud.features.cols()<<endl;

    // kd-tree on libnabo
    distanceNNS.reset(NNS::create(salientCloud.features, salientCloud.features.rows(), NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_NNS(
        Matches::Dists(1, salientResultsCloud.features.cols()),
        Matches::Ids(1, salientResultsCloud.features.cols())
    );
    distanceNNS->knn(salientResultsCloud.features, matches_NNS.ids, matches_NNS.dists, 1, 0, NNS::ALLOW_SELF_MATCH);

    // savings
    ofstream saveTxt(saveDisName);

    for(int m=0; m<salientResultsCloud.features.cols(); m++)
    {
        saveTxt<<sqrt(matches_NNS.dists(0,m))<<endl;
        saveTxt.flush();
    }
    saveTxt.close();

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "errordistribution");
    ros::NodeHandle n;

    errorDistribution errordistribution(n);

    // ugly code

    return 0;
}
