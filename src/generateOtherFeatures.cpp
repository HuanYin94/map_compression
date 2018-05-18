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
    string saveFeaturesName;

    DP mapCloud;
    DP trajCloud;

    int mapLength;
    int roadDNA;
    int maxDensity;

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
    roadDNA(getParam<int>("roadDNA", 0)),
    saveFeaturesName(getParam<string>("saveFeaturesName", ".")),
    maxDensity(getParam<int>("maxDensity", 0))
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
    mapCloud.addDescriptor("Range", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("Height", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("Orientation", PM::Matrix::Zero(1, mapCloud.features.cols()));

    int rowLineSalient = mapCloud.getDescriptorStartingRow("salient");
    int rowLineRange = mapCloud.getDescriptorStartingRow("Range");
    int rowLineHeight = mapCloud.getDescriptorStartingRow("Height");
    int rowLineOrientation = mapCloud.getDescriptorStartingRow("Orientation");
    int rowLineNormal = mapCloud.getDescriptorStartingRow("normals");
    int rowLineIntensity = mapCloud.getDescriptorStartingRow("intensity");
    int rowLineDensity = mapCloud.getDescriptorStartingRow("densities");

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

    //Ort:Traj   ;   Oritentation:MapCloud;  diff-descritors
    trajCloud.addDescriptor("Ort", PM::Matrix::Zero(1, trajCloud.features.cols()));
    int rowLineOrt = trajCloud.getDescriptorStartingRow("Ort");
    int indexStart;
    int indexEnd;
    for(int t=0; t<trajCloud.features.cols(); t++)
    {
        indexStart = 9999999;
        indexEnd = -9999999;
        for(int n=t-roadDNA; n<=t+roadDNA; n++)
        {
            if(n<0 || n>=mapLength)
                continue;
            if(n<indexStart)
                indexStart = n;
            if(n>indexEnd)
                indexEnd = n;
        }

        double X = abs(trajCloud.features(0,indexStart) - trajCloud.features(0,indexEnd));
        double Y = abs(trajCloud.features(1,indexStart) - trajCloud.features(1,indexEnd));

        double ort = std::atan(X/Y)/3.1416926537*180;
        double ortNorm = ort / 90;
        trajCloud.descriptors(rowLineOrt, t) = ortNorm;  // normalization of the orientation

    }

    // temp save
    //trajCloud.save("/home/yh/mapModel/05.09/traj_cloud.vtk");

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
        // R & H features
        double Range = sqrt(matches_Traj.dists(0,m));
        double Height = abs(mapCloud.features(2, m) - trajCloud.features(2, matches_Traj.ids(0,m)));

        // road direction
        double Orientation = trajCloud.descriptors(rowLineOrt, matches_Traj.ids(0,m));

        //densities re-filter, sth. is tool large
        if(mapCloud.descriptors(rowLineDensity, m) > maxDensity)
            mapCloud.descriptors(rowLineDensity, m) = maxDensity;

        //save the descriptors
        {
            mapCloud.descriptors(rowLineRange, m)=Range;
            mapCloud.descriptors(rowLineHeight, m)=Height;
            mapCloud.descriptors(rowLineOrientation, m)=Orientation;
        }

    }

    // finally
    mapCloud.save(saveCloudName);
    /*
    ofstream saver(saveFeaturesName);
    for(int m=0; m<mapCloud.features.cols(); m++)
    {
        saver<<mapCloud.descriptors(rowLineNormal,m)<<"   "
               <<mapCloud.descriptors(rowLineNormal+1,m)<<"   "
                 <<mapCloud.descriptors(rowLineNormal+2,m)<<"   "
                   <<mapCloud.descriptors(rowLineIntensity,m)<<"   "
                     <<mapCloud.descriptors(rowLineRange,m)<<"   "
                        <<mapCloud.descriptors(rowLineHeight,m)<<"   "
                          <<mapCloud.descriptors(rowLineOrientation,m)<<"   "
                            <<mapCloud.descriptors(rowLineSalient,m)<<endl;
        saver.flush();
    }
    saver.close();
    */

    cout<<"FINISHED"<<endl;
}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "Generation");
    ros::NodeHandle n;

    Generation generation(n);

    // ugly code

    return 0;
}
