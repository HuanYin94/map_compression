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

class mergeCloud
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mergeCloud(ros::NodeHandle &n);
    ~mergeCloud();
    ros::NodeHandle& n;

    string loadMapName;
    string loadTrajName;
    string loadSaliencyDir;
    string saveMapName;

    DP mapCloud;
    DP trajCloud;

    vector<vector<double>> initPoses;

    int splitPathLength;
    string saveCloudDir;

    shared_ptr<NNS> featureNNSTraj;

    void process();

};

mergeCloud::~mergeCloud()
{}

mergeCloud::mergeCloud(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    loadTrajName(getParam<string>("loadTrajName", ".")),
    saveCloudDir(getParam<string>("saveCloudDir", ".")),
    splitPathLength(getParam<int>("splitPathLength", 0)),
    loadSaliencyDir(getParam<string>("loadSaliencyDir", ".")),
    saveMapName(getParam<string>("saveMapName", "."))
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
        if(in.eof()) break;
    for (x = 0; x < 16; x++) {
      in >> temp;
      test.push_back(temp);
    }
      initPoses.push_back(test);
    }
    in.close();

    // process
    this->process();

}

void mergeCloud::process()
{

    // turn trajectory(poses) to a DP::CLOUD
    // feature-rows: x, y, z, directly
    trajCloud = mapCloud.createSimilarEmpty();
    for(int p=0; p<initPoses.size(); p++)
    {
        trajCloud.features(0, p) = initPoses[p][3];
        trajCloud.features(1, p) = initPoses[p][7];
        trajCloud.features(2, p) = initPoses[p][11];
    }
    trajCloud.conservativeResize(initPoses.size());

    // map on traj, too slow to build a large one, use index to search

    featureNNSTraj.reset(NNS::create(trajCloud.features, trajCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_Traj(
        Matches::Dists(1, mapCloud.features.cols()),
        Matches::Ids(1, mapCloud.features.cols())
    );
    featureNNSTraj->knn(mapCloud.features, matches_Traj.ids, matches_Traj.dists, 1, 0);

    mapCloud.addDescriptor("trajRecord", PM::Matrix::Zero(1, mapCloud.features.cols()));
    int rowLine_trajRecord = mapCloud.getDescriptorStartingRow("trajRecord");

    for(int i=0; i<mapCloud.features.cols(); i++)
    {
//        cout<<i<<endl;
        int cloudCnt = matches_Traj.ids(0,i) / this->splitPathLength;

        mapCloud.descriptors(rowLine_trajRecord, i) = cloudCnt;

    }


    mapCloud.addDescriptor("low-distc", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("point-assoc", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("high-distc", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("saliency", PM::Matrix::Zero(1, mapCloud.features.cols()));



    int rowLine_lowDistc = mapCloud.getDescriptorStartingRow("low-distc");
    int rowLine_pointAssoc = mapCloud.getDescriptorStartingRow("point-assoc");
    int rowLine_highDistc = mapCloud.getDescriptorStartingRow("high-distc");
    int rowLine_saliency = mapCloud.getDescriptorStartingRow("saliency");




    // one by one for merging
    for(int v=0; v<(trajCloud.features.cols()/splitPathLength) ; v++ )
    {

        // read saliency

        string saliencyFile = loadSaliencyDir + to_string(v) + ".txt";

        int x, y;
        double temp;
        vector<double> test;
        vector<vector<double>> features;
        ifstream in(saliencyFile);
        if (!in) {
            cout << "Cannot open file.\n";
        }
        for (y = 0; y < 999999; y++) {
            test.clear();
            if(in.eof())
                break;
        for (x = 0; x < 4; x++) {
            in >> temp;
            test.push_back(temp);
        }
            features.push_back(test);
        }
        in.close();

        // set saliency
        int cnt = 0;
        for(int c=0; c<mapCloud.features.cols(); c++)
        {
            if(mapCloud.descriptors(rowLine_trajRecord, c) != v)
                continue;

            mapCloud.descriptors(rowLine_lowDistc, c) = features[cnt][0];
            mapCloud.descriptors(rowLine_pointAssoc, c) = features[cnt][1];
            mapCloud.descriptors(rowLine_highDistc, c) = features[cnt][2];
            mapCloud.descriptors(rowLine_saliency, c) = features[cnt][3];

//            if(v==0)
//            {
//                cout<<cnt<<"    "<<features[cnt][3]<<endl;
//            }

            cnt ++;
        }

        cout<<"Save "<<v<<" , point cnt:   "<<cnt<<endl;

    }

    mapCloud.save(this->saveMapName);
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mergeCloud");
    ros::NodeHandle n;

    mergeCloud mergecloud(n);

    // ugly code

    return 0;
}
