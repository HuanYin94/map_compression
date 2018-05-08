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
    string saveNodeDirName;
    string saveEdgeDirName;
    string saveXYZDirName;

    DP mapCloud;
    DP trajCloud;

    int mapLength;
    int windowLength;
    int startIndex;
    int seperateCnt;
    int endIndex;
    int kSearchMap;
    double searchRangeMax;

    vector<vector<double>> initPoses;

    shared_ptr<NNS> featureNNSTraj;
    shared_ptr<NNS> featureNNSSlide;

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
    kSearchMap(getParam<int>("kSearchMap", 0)),
    saveNodeDirName(getParam<string>("saveNodeDirName", ".")),
    saveEdgeDirName(getParam<string>("saveEdgeDirName", ".")),
    saveXYZDirName(getParam<string>("saveXYZDirName", ".")),
    searchRangeMax(getParam<double>("searchRangeMax", 0))
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
    int rowLineSalient = mapCloud.getDescriptorStartingRow("salient");
    int rowLineRange = mapCloud.getDescriptorStartingRow("Range");
    int rowLineHeight = mapCloud.getDescriptorStartingRow("Height");
    int rowLineBin0 = mapCloud.getDescriptorStartingRow("Bin0");
    int rowLineBin1 = mapCloud.getDescriptorStartingRow("Bin1");
    int rowLineBin2 = mapCloud.getDescriptorStartingRow("Bin2");
    int rowLineBin3 = mapCloud.getDescriptorStartingRow("Bin3");


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

    for(int p=startIndex; p<endIndex-windowLength; p=p+seperateCnt, saveCount++)
    {
        cout<<"Sample:  "<<p<<endl;

        // build sliding point clouds, like the sorrounding point clouds
        DP slidingCloud = mapCloud.createSimilarEmpty();
        int count = 0;
        for(int s=0; s<mapCloud.features.cols(); s++)
        {
            if((matches_Traj.ids(0,s) - p) < windowLength && (matches_Traj.ids(0,s) - p) >= 0)
            {
                slidingCloud.setColFrom(count, mapCloud, s);
                count++;
            }
        }
        slidingCloud.conservativeResize(count);

        // build kd-tree inside the sliding map itself
        featureNNSSlide.reset(NNS::create(slidingCloud.features, slidingCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
        PM::Matches matches_Slide(
            Matches::Dists(kSearchMap, slidingCloud.features.cols()),
            Matches::Ids(kSearchMap, slidingCloud.features.cols())
        );
        featureNNSSlide->knn(slidingCloud.features, matches_Slide.ids, matches_Slide.dists, kSearchMap, 0);

        ofstream saveNode(saveNodeDirName+std::to_string(saveCount)+".txt");
        ofstream saveEdge(saveEdgeDirName+std::to_string(saveCount)+".txt");
        ofstream saveXYZ(saveXYZDirName+std::to_string(saveCount)+".txt");

        int edgeCount = 0;

        for(int m=0; m<slidingCloud.features.cols(); m++)
        {
            // save the nodes
            saveNode<<m<<"  "
                   <<slidingCloud.descriptors(rowLineSalient, m)<<"  "
                   <<slidingCloud.descriptors(rowLineRange, m)<<"  "
                   <<slidingCloud.descriptors(rowLineHeight, m)<<"  "
                   <<slidingCloud.descriptors(rowLineBin0, m)<<"  "
                   <<slidingCloud.descriptors(rowLineBin1, m)<<"  "
                   <<slidingCloud.descriptors(rowLineBin2, m)<<"  "
                   <<slidingCloud.descriptors(rowLineBin3, m)<<endl;
            saveNode.flush();

            //save the edges, overlapped
            float angle;
            for(int k=0; k<kSearchMap; k++)
            {
                if(sqrt(matches_Slide.dists(k,m)) < searchRangeMax)
                {
                    // 03.03-compute a new propety: angle
                    angle = this->getDeltaAngle(slidingCloud.getDescriptorViewByName("normals")(0, m),
                                                          slidingCloud.getDescriptorViewByName("normals")(1, m),
                                                          slidingCloud.getDescriptorViewByName("normals")(2, m),
                                                          slidingCloud.getDescriptorViewByName("normals")(0, matches_Slide.ids(k,m)),
                                                          slidingCloud.getDescriptorViewByName("normals")(1, matches_Slide.ids(k,m)),
                                                          slidingCloud.getDescriptorViewByName("normals")(2, matches_Slide.ids(k,m)));

                    angle = angle / PI * 180;

                    saveEdge<<edgeCount<<"  "
                              <<m<<"  "
                              <<matches_Slide.ids(k,m)<<"  "
                              <<sqrt(matches_Slide.dists(k,m))<<"  "
                             <<angle<<endl;
                    saveEdge.flush();
                    edgeCount++;
                }
            }

            // save the XYZs, for further process
            saveXYZ<<m<<"  "
                   <<slidingCloud.features(0,m)<<"  "
                   <<slidingCloud.features(1,m)<<"  "
                   <<slidingCloud.features(2,m)<<endl;

        }
        saveNode.close();
        saveEdge.close();
        saveXYZ.close();

        //save for show
        if(p==8000)
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
