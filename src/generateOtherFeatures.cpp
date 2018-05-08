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

#define MAXEST 999999

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
    string saveCloudName;

    DP mapCloud;
    DP trajCloud;

    int mapLength;
    int roadDNA;

    vector<vector<double>> initPoses;

    shared_ptr<NNS> featureNNSTraj;

    PM::DataPointsFilters mapFilters;

    void process();

};

Generation::~Generation()
{}

Generation::Generation(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    loadTrajName(getParam<string>("loadTrajName", ".")),
    mapLength(getParam<int>("mapLength", 0)),
    saveCloudName(getParam<string>("saveCloudName", ".")),
    roadDNA(getParam<int>("roadDNA", 0))
{
    // load
    mapCloud = DP::load(loadMapName);

    //filter normals for trajectory cloud
//    ifstream mapiffilter(loadFilterName);
//    mapFilters = PM::DataPointsFilters(mapiffilter);

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

}

void Generation::process()
{
    /// continue the CRF

    //add new descriptors
    mapCloud.addDescriptor("Curvity", PM::Matrix::Zero(1, mapCloud.features.cols()));

    int rowLineSalient = mapCloud.getDescriptorStartingRow("salient");
    int rowLineRange = mapCloud.getDescriptorStartingRow("Range");
    int rowLineHeight = mapCloud.getDescriptorStartingRow("Height");
    int rowLineNormal = mapCloud.getDescriptorStartingRow("normals");
    int rowLineProb = mapCloud.getDescriptorStartingRow("probabilityStatic");
    int rowLineIntensity = mapCloud.getDescriptorStartingRow("intensity");


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

    /*
     * // stupid calculation
    mapFilters.apply(trajCloud);

    int rowLineNormalTraj = trajCloud.getDescriptorStartingRow("normals");

    // smooth process
    // Average normals
    // small test
    for(int t=0; t<trajCloud.features.cols(); t++)
    {
        vector<double> normal_x_v;
        vector<double> normal_y_v;
        vector<double> normal_z_v;
        for(int n=t-20; n<t+20; n++)
        {
            if(n<0 || n>=mapLength)
                continue;

            // pushing
            normal_x_v.push_back(trajCloud.getDescriptorViewByName("normals")(0, n));
            normal_y_v.push_back(trajCloud.getDescriptorViewByName("normals")(1, n));
            normal_z_v.push_back(trajCloud.getDescriptorViewByName("normals")(2, n));

        }

        //Averaging
        double meanNX = std::accumulate(std::begin(normal_x_v), std::end(normal_x_v), 0.0) / normal_x_v.size();
        double meanNY = std::accumulate(std::begin(normal_y_v), std::end(normal_y_v), 0.0) / normal_y_v.size();
        double meanNZ = std::accumulate(std::begin(normal_z_v), std::end(normal_z_v), 0.0) / normal_z_v.size();
        //giving
        trajCloud.descriptors(rowLineNormalTraj,t) = meanNX;
        trajCloud.descriptors(rowLineNormalTraj+1,t) = meanNY;
        trajCloud.descriptors(rowLineNormalTraj+2,t) = meanNZ;

    }


    // temp save
    trajCloud.save("/home/yh/mapModel/05.08/traj_cloud.vtk");
    */

    cout<<"start"<<endl;

    // map on traj, too slow to build a large one, use index to search

    featureNNSTraj.reset(NNS::create(trajCloud.features, trajCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_Traj(
        Matches::Dists(1, mapCloud.features.cols()),
        Matches::Ids(1, mapCloud.features.cols())
    );
    featureNNSTraj->knn(mapCloud.features, matches_Traj.ids, matches_Traj.dists, 1, 0);

    // Every point counts!
    for(int m=0; m<mapCloud.features.cols(); m++)
    {
        // Range and Height are calculated

        // curvity of the road


    }

    // finally
    mapCloud.save(saveCloudName);

}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "Generation");
    ros::NodeHandle n;

    Generation generation(n);

    // ugly code

    return 0;
}
