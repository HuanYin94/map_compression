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

class mapCompare
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapCompare(ros::NodeHandle &n);
    ~mapCompare();
    ros::NodeHandle& n;

    string loadMapName0;
    string loadMapName1;
    string saveIndexName;
    string saveDistsName;


    DP mapCloud0;
    DP mapCloud1;

    shared_ptr<NNS> distanceNNS;

    void process();
    vector<int> intersection(vector<int> a, vector<int> b);

};

mapCompare::~mapCompare()
{}

mapCompare::mapCompare(ros::NodeHandle& n):
    n(n),
    loadMapName0(getParam<string>("loadMapName0", ".")),
    loadMapName1(getParam<string>("loadMapName1", ".")),
    saveIndexName(getParam<string>("saveIndexName", ".")),
    saveDistsName(getParam<string>("saveDistsName", "."))
{

    // load
    mapCloud0 = DP::load(loadMapName0);
    mapCloud1 = DP::load(loadMapName1);

    cout<<"JUST COMPARE"<<endl;

    // process
    this->process();

}

void mapCompare::process()
{
//    int rowLineSession = mapCloud0.getDescriptorStartingRow("session");
    int rowLineSalient0 = mapCloud0.getDescriptorStartingRow("salient");
    int rowLineSalient1 = mapCloud1.getDescriptorStartingRow("salient");

    // save and anal
    vector<int> salientIndex0;
    DP salientCloud0 = mapCloud0.createSimilarEmpty();
    int cnt0=0;
    for(int i=0; i<mapCloud0.features.cols(); i++)
    {
        if(mapCloud0.descriptors(rowLineSalient0, i) == 1)
        {
            salientCloud0.setColFrom(cnt0, mapCloud0, i);
            salientIndex0.push_back(i);
            cnt0++;
        }
    }
    salientCloud0.conservativeResize(cnt0);

    vector<int> salientIndex1;
    DP salientCloud1 = mapCloud1.createSimilarEmpty();
    int cnt1=0;
    for(int i=0; i<mapCloud1.features.cols(); i++)
    {
        if(mapCloud1.descriptors(rowLineSalient1, i) == 1)
        {
            salientCloud1.setColFrom(cnt1, mapCloud1, i);
            salientIndex1.push_back(i);
            cnt1++;
        }
    }
    salientCloud1.conservativeResize(cnt1);

    //Compare, the num of same index and mean distance

    vector<int> samer = this->intersection(salientIndex0, salientIndex1);
    cout<<"num of PC0:  "<<salientCloud0.features.cols()<<endl;
    cout<<"num of PC1:  "<<salientCloud1.features.cols()<<endl;
    cout<<"Same Index:  "<<samer.size()<<endl;

    distanceNNS.reset(NNS::create(salientCloud0.features, salientCloud0.features.rows()-1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_NNS(
        Matches::Dists(1, salientCloud1.features.cols()),
        Matches::Ids(1, salientCloud1.features.cols())
    );
    distanceNNS->knn(salientCloud1.features, matches_NNS.ids, matches_NNS.dists, 1, 0);

    vector<double> distsVector;
    for(int i=0; i<salientCloud1.features.cols(); i++)
    {
        distsVector.push_back(sqrt(matches_NNS.dists(0, i)));
    }

    double meanDist = std::accumulate(std::begin(distsVector), std::end(distsVector), 0.0) / distsVector.size();
    cout<<"Mean Distance:   "<<meanDist<<endl;

}

vector<int> mapCompare::intersection(vector<int> a, vector<int> b)
{
    vector<int> resultVector(0);

    for(int bb=0; bb<b.size(); bb++)
    {
        vector<int>::iterator result = std::find(a.begin(), a.end(), b.at(bb));
        if(result != a.end())
            resultVector.push_back(*result);
    }

    return resultVector;
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapCompare");
    ros::NodeHandle n;

    mapCompare mapcompare(n);

    // ugly code

    return 0;
}
