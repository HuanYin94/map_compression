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

class mapCompareSub
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapCompareSub(ros::NodeHandle &n);
    ~mapCompareSub();
    ros::NodeHandle& n;

    string loadMapName0;
    string loadMapName1;
    string saveIndexName;
    string saveDistsName;
    string loadOriginMapName;

    DP mapCloud0;
    DP mapCloud1;
    DP mapCloudOrigin;

    shared_ptr<NNS> distanceNNS;

    void process();
    vector<int> intersection(vector<int> a, vector<int> b);
    int findThePoint(Eigen::Vector3f input);

};

mapCompareSub::~mapCompareSub()
{}

mapCompareSub::mapCompareSub(ros::NodeHandle& n):
    n(n),
    loadMapName0(getParam<string>("loadMapName0", ".")),
    loadMapName1(getParam<string>("loadMapName1", ".")),
    saveIndexName(getParam<string>("saveIndexName", ".")),
    saveDistsName(getParam<string>("saveDistsName", ".")),
    loadOriginMapName(getParam<string>("loadOriginMapName", "."))
{

    // load
    mapCloud0 = DP::load(loadMapName0);
    mapCloud1 = DP::load(loadMapName1);
    mapCloudOrigin = DP::load(loadOriginMapName);

    if(!mapCloudOrigin.descriptorExists("salient"))
    {
        mapCloudOrigin.addDescriptor("salient", PM::Matrix::Zero(1, mapCloudOrigin.features.cols()));
    }

    cout<<"JUST COMPARE"<<endl;

    // process
    this->process();

}

void mapCompareSub::process()
{
//    int rowLineSession = mapCloud0.getDescriptorStartingRow("session");
    int rowLineSalient0 = mapCloud0.getDescriptorStartingRow("salient");
    int rowLineSalient1 = mapCloud1.getDescriptorStartingRow("salient");
    int rowLineSalient2 = mapCloudOrigin.getDescriptorStartingRow("salient");

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
//    cout<<"0.0"<<endl;
    // project the salient index from the compressed map to the full origin map
    for(int m=0; m<mapCloud1.features.cols(); m++)
    {
//        cout<<m<<endl;
        int index;
        if(mapCloud1.descriptors(rowLineSalient1, m) == 1)
        {
            index = this->findThePoint(mapCloud1.features.col(m).head(3));
        }

        mapCloudOrigin.descriptors(rowLineSalient2, index) = 1;
    }
//    cout<<"0.1"<<endl;
    vector<int> salientIndex2;
    DP salientCloud2 = mapCloudOrigin.createSimilarEmpty();
    int cnt2=0;
    for(int i=0; i<mapCloudOrigin.features.cols(); i++)
    {
        if(mapCloudOrigin.descriptors(rowLineSalient2, i) == 1)
        {
            salientCloud2.setColFrom(cnt2, mapCloudOrigin, i);
            salientIndex2.push_back(i);
            cnt2++;
        }
    }
    salientCloud2.conservativeResize(cnt2);

    //Compare, the num of same index and mean distance

    vector<int> samer = this->intersection(salientIndex0, salientIndex2);
    cout<<"num of PC0:  "<<salientCloud0.features.cols()<<endl;
    cout<<"num of PC2:  "<<salientCloud2.features.cols()<<endl;
    cout<<"Same Index:  "<<samer.size()<<endl;

//    distanceNNS.reset(NNS::create(salientCloud0.features, salientCloud0.features.rows()-1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
//    PM::Matches matches_NNS(
//        Matches::Dists(1, salientCloud1.features.cols()),
//        Matches::Ids(1, salientCloud1.features.cols())
//    );
//    distanceNNS->knn(salientCloud1.features, matches_NNS.ids, matches_NNS.dists, 1, 0);

//    vector<double> distsVector;
//    for(int i=0; i<salientCloud1.features.cols(); i++)
//    {
//        distsVector.push_back(sqrt(matches_NNS.dists(0, i)));
//    }

//    double meanDist = std::accumulate(std::begin(distsVector), std::end(distsVector), 0.0) / distsVector.size();
//    cout<<"Mean Distance:   "<<meanDist<<endl;

}

vector<int> mapCompareSub::intersection(vector<int> a, vector<int> b)
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

int mapCompareSub::findThePoint(Eigen::Vector3f input)
{
    const int cnt(mapCloudOrigin.features.cols());

    for(int i = 0; i < cnt; i++)
    {
        if(mapCloudOrigin.features.col(i).head(3) == input)
        {
            return i;
        }
    }

    return -1;
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapCompareSub");
    ros::NodeHandle n;

    mapCompareSub mapcomparesub(n);

    // ugly code

    return 0;
}
