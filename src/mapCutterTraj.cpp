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

#define PI 3.14159265

using namespace std;
using namespace PointMatcherSupport;

class mapCutterTraj
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapCutterTraj(ros::NodeHandle &n);
    ~mapCutterTraj();
    ros::NodeHandle& n;

    string loadMapName;
    string loadTrajName;
    string saveTrainName;
    string saveTestName;

    DP mapCloud;
    DP trajCloud;

    string keepIndexName;
    vector<int> indexVector;

    int cutPoint0;
    int cutPoint1;
    int cutPoint2;
    int cutPoint3;

    vector<vector<double>> initPoses;

    shared_ptr<NNS> featureNNSTraj;

    void process();

};

mapCutterTraj::~mapCutterTraj()
{}

mapCutterTraj::mapCutterTraj(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    loadTrajName(getParam<string>("loadTrajName", ".")),
    keepIndexName(getParam<string>("keepIndexName", ".")),
    cutPoint0(getParam<int>("cutPoint0", 0)),
    cutPoint1(getParam<int>("cutPoint1", 0)),
    cutPoint2(getParam<int>("cutPoint2", 0)),
    cutPoint3(getParam<int>("cutPoint3", 0)),
    saveTrainName(getParam<string>("saveTrainName", ".")),
    saveTestName(getParam<string>("saveTestName", "."))
{

    // load
    mapCloud = DP::load(loadMapName);

    // read poses
    int x, y;
    double temp;
    vector<double> test;
    ifstream in(loadTrajName);
    if (!in) {
        cout << "Cannot open file.\n";
    }
    for (y = 0; y < 999999; y++) {
        test.clear();
    for (x = 0; x < 16; x++) {
      in >> temp;
      test.push_back(temp);
    }
      initPoses.push_back(test);
    }
    in.close();

    // read all the effective index from list in the txt
    int l;
    ifstream in_(keepIndexName);
    if (!in_) {
        cout << "Cannot open file.\n";
    }
    while(!in_.eof())
    {
        in_>>l;
        indexVector.push_back(l);
    }

    // process
    this->process();

}

void mapCutterTraj::process()
{

    // turn trajectory(poses) to a DP::CLOUD
    // feature-rows: x, y, z, directly
    trajCloud = mapCloud.createSimilarEmpty();
    for(int p=0; p<indexVector.size(); p++)
    {
        int index =indexVector.at(p);

        trajCloud.features(0, p) = initPoses[index][3];
        trajCloud.features(1, p) = initPoses[index][7];
        trajCloud.features(2, p) = initPoses[index][11];
    }
    trajCloud.conservativeResize(indexVector.size());

    // map on traj, too slow to build a large one, use index to search

    featureNNSTraj.reset(NNS::create(trajCloud.features, trajCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_Traj(
        Matches::Dists(1, mapCloud.features.cols()),
        Matches::Ids(1, mapCloud.features.cols())
    );
    featureNNSTraj->knn(mapCloud.features, matches_Traj.ids, matches_Traj.dists, 1, 0);


    DP saveTrainCloud = mapCloud.createSimilarEmpty();
    DP saveTestCloud = mapCloud.createSimilarEmpty();

    int trainCount=0;
    int testCount=0;

    for(int i=0; i<mapCloud.features.cols(); i++)
    {
        cout<<i<<endl;
        int numOfTraj = matches_Traj.ids(0,i);

        if(numOfTraj >= cutPoint0 && numOfTraj <= cutPoint1)
        {
            saveTrainCloud.setColFrom(trainCount, mapCloud, i);
            trainCount++;
        }
        else if(numOfTraj >= cutPoint2 && numOfTraj <= cutPoint3)
        {
            saveTestCloud.setColFrom(testCount, mapCloud, i);
            testCount++;
        }
    }

    saveTrainCloud.conservativeResize(trainCount);
    saveTestCloud.conservativeResize(testCount);

    saveTrainCloud.save(saveTrainName);
    saveTestCloud.save(saveTestName);

    cout<<"Splitted & Saved"<<endl;

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapcuttertraj");
    ros::NodeHandle n;

    mapCutterTraj mapcuttertraj(n);

    // ugly code

    return 0;
}
