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

class compareDistribution
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    compareDistribution(ros::NodeHandle &n);
    ~compareDistribution();
    ros::NodeHandle& n;

    string saveDisName;
    string loadMapName1;
    string loadMapName2;

    DP loadMapCloud1;
    DP loadMapCloud2;

    shared_ptr<NNS> distanceNNS;

    void process();

};

compareDistribution::~compareDistribution()
{}

compareDistribution::compareDistribution(ros::NodeHandle& n):
    n(n),
    loadMapName1(getParam<string>("loadMapName1", ".")),
    loadMapName2(getParam<string>("loadMapName2", ".")),
    saveDisName(getParam<string>("saveDisName", "."))
{

    // load
    loadMapCloud1 = DP::load(loadMapName1);
    loadMapCloud2 = DP::load(loadMapName2);

    // process
    this->process();

}

void compareDistribution::process()
{

    cout<<"Cloud 1:  "<<loadMapCloud1.features.cols()<<endl;
    cout<<"Cloud 2:  "<<loadMapCloud2.features.cols()<<endl;

    // kd-tree on libnabo
    distanceNNS.reset(NNS::create(loadMapCloud1.features, loadMapCloud1.features.rows(), NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_NNS(
        Matches::Dists(1, loadMapCloud2.features.cols()),
        Matches::Ids(1, loadMapCloud2.features.cols())
    );
    distanceNNS->knn(loadMapCloud2.features, matches_NNS.ids, matches_NNS.dists, 1, 0, NNS::ALLOW_SELF_MATCH);

    // savings
    ofstream saveTxt(saveDisName);

    for(int m=0; m<loadMapCloud2.features.cols(); m++)
    {
        saveTxt<<sqrt(matches_NNS.dists(0,m))<<endl;
        saveTxt.flush();
    }
    saveTxt.close();

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "compareDistribution");
    ros::NodeHandle n;

    compareDistribution compareDistribution(n);

    // ugly code

    return 0;
}
