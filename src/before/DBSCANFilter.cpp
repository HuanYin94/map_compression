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

class DBSCAN
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    DBSCAN(ros::NodeHandle &n);
    ~DBSCAN();
    ros::NodeHandle& n;

    string loadMapName;
    string saveCloudName;

    DP mapCloud;

    int kSearch;
    int minPoints;
    double e;

    shared_ptr<NNS> distanceNNS;

    void process();

    vector<int> intersection(vector<int> a, vector<int> b);
    vector<int> deletion(vector<int> a, vector<int> b);
};

DBSCAN::~DBSCAN()
{}

DBSCAN::DBSCAN(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveCloudName(getParam<string>("saveCloudName", ".")),
    minPoints(getParam<int>("minPoints", 0)),
    e(getParam<double>("e", 0)),
    kSearch(getParam<int>("kSearch", 0))
{

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();

}

void DBSCAN::process()
{

    // kd-tree on libnabo, itself matching
    distanceNNS.reset(NNS::create(mapCloud.features, mapCloud.features.rows(), NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_NNS(
        Matches::Dists(kSearch, mapCloud.features.cols()),
        Matches::Ids(kSearch, mapCloud.features.cols())
    );
    distanceNNS->knn(mapCloud.features, matches_NNS.ids, matches_NNS.dists, kSearch, 0); // not NNS::ALLOW_SELF_MATCH

    // find the seed points
    // index of the pointCloud
    vector<int> seedVector;
    int cnt;
    for(int p=0; p<mapCloud.features.cols(); p++)
    {
        cnt=0;
        for(int N=0; N<kSearch; N++)
        {
            if(sqrt(matches_NNS.dists(N,p)) < e)
                cnt++;
        }

        if(cnt>=minPoints)
            seedVector.push_back(p);
    }

    cout<<"seed num:    "<<seedVector.size()<<endl;

    // clustering
    int clusterNum = 0;
    vector<int> unvisitVector;
    for(int i=0; i<mapCloud.features.cols(); i++)
    {
        unvisitVector.push_back(i);
    }
    vector<int> unvisitVector_old;

    // loop in loop

    vector<vector<int>> clusteredIndex;
    vector<int> queueVector;
    vector<int> neighbors;

    while(seedVector.size()>0)
    {
        cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
        int randomIndex = (rand() % (seedVector.size()-0))+ 0;
        cout<<"processing seed:   "<<seedVector.at(randomIndex)<<endl;

        // copy
        unvisitVector_old.clear();
        unvisitVector_old.assign(unvisitVector.begin(), unvisitVector.end());
//        cout<<"copy finished"<<endl;

        queueVector.push_back(seedVector.at(randomIndex));
//        cout<<"queueing"<<endl;

        //remove
        vector<int>::iterator iter_=std::find(unvisitVector.begin(), unvisitVector.end(), seedVector.at(randomIndex));
        unvisitVector.erase(iter_);
//        cout<<"removed"<<endl;

        while(queueVector.size()>0)
        {
            int core = queueVector.at(0);
            vector<int>::iterator iter=std::find(queueVector.begin(), queueVector.end(), core);
            queueVector.erase(iter);
//            cout<<"released core:   "<<core<<endl;

            vector<int>::iterator result = std::find(seedVector.begin(), seedVector.end(), core);

            // it's a seed
            if(result != seedVector.end())
            {
//                cout<<"--------------------------------"<<endl;
//                cout<<"result value:    "<<*result<<endl;
                // get it's neighbors!
                for(int N=0; N<kSearch; N++)
                {
                    if(sqrt(matches_NNS.dists(N,*result)) < e)
                    {
                        neighbors.push_back(matches_NNS.ids(N,*result));
                    }
                }

//                cout<<"unvisit size:    "<<unvisitVector.size()<<endl;
//                cout<<"neighbors size:    "<<neighbors.size()<<endl;

                vector<int> delta = this->intersection(neighbors, unvisitVector);
                neighbors.clear();
                // give the delta to Queueing
                queueVector.clear();
                queueVector.assign(delta.begin(), delta.end());
//                cout<<"Queue Size:  "<<queueVector.size()<<endl;

                // reset
//                cout<<"delta size:    "<<delta.size()<<endl;
                vector<int> tempVector = this->deletion(unvisitVector, delta);
                unvisitVector.clear();
                unvisitVector.assign(tempVector.begin(), tempVector.end());
//                cout<<"unvisit new size:    "<<unvisitVector.size()<<endl;
            }

        }

        clusterNum++;
        cout<<"unvisit old size:    "<<unvisitVector_old.size()<<endl;

        vector<int> C = this->deletion(unvisitVector_old, unvisitVector);
        // check the size
        if(C.size() > this->minPoints)
        {
            clusteredIndex.push_back(C);
        }

        vector<int> CC = this->deletion(seedVector, C);
        seedVector.clear();
        seedVector.assign(CC.begin(), CC.end());

        cout<<"num of C:    "<<C.size()<<"    seed num:   "<<seedVector.size()<<endl;
    }

    for(int cNum=0; cNum<clusteredIndex.size(); cNum++)
    {
        cout<<cNum<<"   "<<clusteredIndex.at(cNum).size()<<endl;
    }

    mapCloud.addDescriptor("Cluster", PM::Matrix::Zero(1, mapCloud.features.cols()));
    int rowLineCluster = mapCloud.getDescriptorStartingRow("Cluster");

    for(int cNum=0; cNum<clusteredIndex.size(); cNum++)
    {
        int color = cNum + 1;
        for(int index=0; index<clusteredIndex.at(cNum).size(); index++)
        {
            mapCloud.descriptors(rowLineCluster, clusteredIndex[cNum][index]) = color;
        }
    }

    mapCloud.save(saveCloudName);

}

vector<int> DBSCAN::intersection(vector<int> a, vector<int> b)
{
    vector<int> resultVector(0);

//    std::sort(a.begin(), a.end());
//    std::sort(b.begin(), b.end());

//    auto iter = std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), resultVector.begin());
//    resultVector.resize(iter-resultVector.begin());

    for(int bb=0; bb<b.size(); bb++)
    {
        vector<int>::iterator result = std::find(a.begin(), a.end(), b.at(bb));
        if(result != a.end())
            resultVector.push_back(*result);
    }

    return resultVector;
}

vector<int> DBSCAN::deletion(vector<int> a, vector<int> b)
{

    vector<int> resultVector(0);

//    if(b.size()==0 || a.size()==0)
//        return a;

//    std::sort(a.begin(), a.end());
//    std::sort(b.begin(), b.end());

//    auto iter = std::set_difference(a.begin(), a.end(), b.begin(), b.end(), resultVector.begin());
//    resultVector.resize(iter-resultVector.begin());

    for(int aa=0; aa<a.size(); aa++)
    {
        vector<int>::iterator result = std::find(b.begin(), b.end(), a.at(aa));
        if(result == b.end())
            resultVector.push_back(a.at(aa));
    }

    return resultVector;

}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "DBSCAN");
    ros::NodeHandle n;

    DBSCAN dbscan(n);

    // ugly code

    return 0;
}
