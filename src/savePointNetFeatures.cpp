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

class Generation
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    Generation(ros::NodeHandle &n);
    ~Generation();
    ros::NodeHandle& n;

    string loadMapName;
    string loadTrajName;
    string savePointsDirName;

    DP mapCloud;
    DP trajCloud;

    int mapLength;
    int windowLength;
    int startIndex;
    int seperateCnt;
    int endIndex;
    int kSearchMap;

    vector<vector<double>> initPoses;

    shared_ptr<NNS> featureNNSTraj;

    void wait();
    void process();
    float getDeltaAngle(float x1, float y1, float z1, float x2, float y2, float z2);

};

Generation::~Generation()
{}

Generation::Generation(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    loadTrajName(getParam<string>("loadTrajName", ".")),
    mapLength(getParam<int>("mapLength", 0)),
    windowLength(getParam<int>("windowLength", 0)),
    startIndex(getParam<int>("startIndex", 0)),
    seperateCnt(getParam<int>("seperateCnt", 0)),
    endIndex(getParam<int>("endIndex", 0)),
    savePointsDirName(getParam<string>("savePointsDirName", "."))
{

    // load
    mapCloud = DP::load(loadMapName);

    //map filter
    string mapfilterName = "/home/yh/mine_ws/src/test_detection/cfg/map_filters_normals.yaml";
    ifstream mapiffilter(mapfilterName);
    PM::DataPointsFilters mapFilters = PM::DataPointsFilters(mapiffilter);
    mapFilters.apply(mapCloud);

    // read poses
    int x, y;
    double temp;
    vector<double> test;
    ifstream in(loadTrajName);
    if (!in) {
        cout << "Cannot open file.\n";
    }
    for (y = 0; y < mapLength; y++) {
        test.clear();
    for (x = 0; x < 16; x++) {
      in >> temp;
      test.push_back(temp);
    }
      initPoses.push_back(test);
    }
    in.close();



    // process
    this->process();

    this->wait();
}

void Generation::wait()
{
    ros::Rate r(1);

    while(ros::ok())
    {
    }
}

void Generation::process()
{
    // must have, after getFeatures.cpp
    int rowLineIntensity = mapCloud.getDescriptorStartingRow("intensity");
    int rowLineSalient = mapCloud.getDescriptorStartingRow("salient");



    // turn poses to a DP::CLOUD
    // feature-rows: x, y, z, directly
    trajCloud = mapCloud.createSimilarEmpty();
    for(int p=0; p<mapLength; p++)
    {
        trajCloud.features(0, p) = initPoses[p][3];
        trajCloud.features(1, p) = initPoses[p][7];
        trajCloud.features(2, p) = initPoses[p][11];
    }
    trajCloud.conservativeResize(mapLength);

    // map on traj, too slow to build a large one, use index to search

    featureNNSTraj.reset(NNS::create(trajCloud.features, trajCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_Traj(
        Matches::Dists(1, mapCloud.features.cols()),
        Matches::Ids(1, mapCloud.features.cols())
    );
    featureNNSTraj->knn(mapCloud.features, matches_Traj.ids, matches_Traj.dists, 1, 0);

    int saveCount = 0;

    // different from CRF,the q is the center of the sliding window
    for(int q=startIndex; q<=endIndex-windowLength/2; q=q+seperateCnt, saveCount++)
    {
        cout<<"Sample:  "<<q<<endl;

        // build sliding point clouds, like the sorrounding point clouds
        DP slidingCloud = mapCloud.createSimilarEmpty();
        int count = 0;
        for(int s=0; s<mapCloud.features.cols(); s++)
        {
            if(abs(matches_Traj.ids(0,s) - q) <= windowLength/2)
            {
                slidingCloud.setColFrom(count, mapCloud, s);
                count++;
            }
        }
        slidingCloud.conservativeResize(count);

        ofstream savePoints(savePointsDirName+std::to_string(saveCount)+".txt");


        for(int m=0; m<slidingCloud.features.cols(); m++)
        {

            // save the points
              savePoints<<slidingCloud.features(0, m) - trajCloud.features(0, q)<<"  "
                        <<slidingCloud.features(1, m) - trajCloud.features(1, q)<<"  "
                        <<slidingCloud.features(2, m) - trajCloud.features(2, q)<<"  "
                        <<slidingCloud.descriptors(rowLineIntensity, m)<<"  "
                        <<slidingCloud.descriptors(rowLineSalient, m)<<"  "<<endl;
              savePoints.flush();

        }
        savePoints.close();

        //save for show
        if(q==300)
            slidingCloud.save("/home/yh/slide.vtk");

    }

}

float Generation::getDeltaAngle(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float up = x1*x2 + y1*y2 + z1*z2;
    float down = sqrt(pow(x1,2) + pow(y1,2) + pow(z1,2))*sqrt(pow(x2,2) + pow(y2,2) + pow(z2,2));
    return std::acos(abs(up/down));
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "Generation");
    ros::NodeHandle n;

    Generation generation(n);

    // ugly code

    return 0;
}
