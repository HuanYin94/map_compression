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
    ros::Publisher mapCloudPub;
    string saveTxtName;
    string saveCloudName;

    DP mapCloud;
    DP trajCloud;

    int mapLength;
    int kSearchMap;
    int kSearchTraj;
    nav_msgs::Path mapPath;

    vector<vector<double>> initPoses;

    shared_ptr<NNS> featureNNSTraj;
    shared_ptr<NNS> featureNNSMap;


    void wait();
    void process();

    float getDistance(Eigen::Vector3f inputA, Eigen::Vector3f inputB);
    float getLineDistance(Eigen::Vector3f node0, Eigen::Vector3f node1, Eigen::Vector3f nodeIn);


};

Generation::~Generation()
{}

Generation::Generation(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    loadTrajName(getParam<string>("loadTrajName", ".")),
    mapLength(getParam<int>("mapLength", 0)),
    kSearchMap(getParam<int>("kSearchMap", 0)),
    kSearchTraj(getParam<int>("kSearchTraj", 0)),
    saveTxtName(getParam<string>("saveTxtName", ".")),
    saveCloudName(getParam<string>("saveCloudName", "."))
{
    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("mapCloud", 2, true);

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

    mapCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "global", ros::Time(0)));
//    mapPathPub.publish(mapPath);

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
    ofstream saveTxt(saveTxtName);

    //add new descriptors
    mapCloud.addDescriptor("Range", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("Height", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("Bin0", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("Bin1", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("Bin2", PM::Matrix::Zero(1, mapCloud.features.cols()));
    mapCloud.addDescriptor("Bin3", PM::Matrix::Zero(1, mapCloud.features.cols()));

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

    cout<<"start"<<endl;

    // set NNS Map & Traj
    // map on map, for searching nearest mapPoints, edges of CRF
    featureNNSMap.reset(NNS::create(mapCloud.features, mapCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_Map(
        Matches::Dists(kSearchMap, mapCloud.features.cols()),
        Matches::Ids(kSearchMap, mapCloud.features.cols())
    );
    featureNNSMap->knn(mapCloud.features, matches_Map.ids, matches_Map.dists, kSearchMap, 0);

    // map on traj, too slow to build a large one, use index to search

    featureNNSTraj.reset(NNS::create(trajCloud.features, trajCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches_Traj(
        Matches::Dists(1, mapCloud.features.cols()),
        Matches::Ids(1, mapCloud.features.cols())
    );
    featureNNSTraj->knn(mapCloud.features, matches_Traj.ids, matches_Traj.dists, 1, 0);

    cout<<"two  kd-tree-s built"<<endl;

    // Every point counts!

    for(int m=0; m<mapCloud.features.cols(); m++)
    {
        cout<<"------------------------"<<endl;
        cout<<"mapPoint:    "<<m<<endl;
        double t0 = ros::Time::now().toSec();

        // R & H features
        double Range = sqrt(matches_Traj.dists(0,m));
        double Height = abs(mapCloud.features(2, m) - trajCloud.features(2, matches_Traj.ids(0,m)));

        // thePoint in mapCloud
        Eigen::Vector3f thePoint = mapCloud.features.col(m).head(3);

        // build surrounding cloud points
        DP surroundCloud = mapCloud.createSimilarEmpty();
        int count = 0;
        for(int s=0; s<mapCloud.features.cols(); s++)
        {
            if(abs(matches_Traj.ids(0,s) - matches_Traj.ids(0,m)) <= (kSearchTraj/2))
            {
                surroundCloud.setColFrom(count, mapCloud, s);
                count++;
            }
        }
        surroundCloud.conservativeResize(count);

//        // save for check & show
//        if(m==1000)
//            surroundCloud.save("/home/yh/test.vtk");

        // store all the nearest distances
        vector<float> nearDists;

        // statistics on the nearest mapPoint to the Line(the Pose, the Point)
        for(int p=0; p<kSearchTraj; p++)
        {
            // find the pose that assigned
            int deltaIndex = (p>(kSearchTraj/2))?(p-(kSearchTraj/2)):((kSearchTraj/2)-p);
            int index = matches_Traj.ids(0,m) + deltaIndex;
            index>mapLength?index=mapLength:index=index;
            index<0?index=0:index=index;

            // thePose in the trajectory
            Eigen::Vector3f thePose = trajCloud.features.col(index).head(3);

            float xmin=std::min(thePose(0), thePoint(0));
            float xmax=std::max(thePose(0), thePoint(0));
            float ymin=std::min(thePose(1), thePoint(1));
            float ymax=std::max(thePose(1), thePoint(1));
            float zmin=std::min(thePose(2), thePoint(2));
            float zmax=std::max(thePose(2), thePoint(2));

            float nearestDist = MAXEST;
            for(int s=0; s<surroundCloud.features.cols(); s++)
            {
                // judge if the point is in the cube
                if(surroundCloud.features(0,s)>xmin && surroundCloud.features(0,s)<xmax &&
                   surroundCloud.features(1,s)>ymin && surroundCloud.features(1,s)<ymax &&
                   surroundCloud.features(2,s)>zmin && surroundCloud.features(2,s)<zmax)
                {
                    Eigen::Vector3f tempPoint = surroundCloud.features.col(s).head(3);
                    float dist = getLineDistance(thePose, thePoint, tempPoint);
                    dist<nearestDist?nearestDist=dist:nearestDist=nearestDist;
                }
            }

            // no point inside?
            nearestDist==MAXEST?nearDists.push_back(0):nearDists.push_back(nearestDist);

        }


        // histogram of normalization
        PM::Matrix bins = PM::Matrix::Zero(1, 4);
        for(int v=0; v<nearDists.size(); v++)
        {
            float d = nearDists.at(v);
            if(d <= 0.1)
                bins(0,0) = bins(0,0) + 1;
            else if(d > 0.1 && d <= 0.4)
                bins(0,1) = bins(0,1) + 1;
            else if(d > 0.4 && d <= 1.0)
                bins(0,2) = bins(0,2) + 1;
            else
                bins(0,3) = bins(0,3) + 1;
        }

        Eigen::Vector4d Prob;
        Prob(0)=bins(0,0)/nearDists.size();
        Prob(1)=bins(0,1)/nearDists.size();
        Prob(2)=bins(0,2)/nearDists.size();
        Prob(3)=bins(0,3)/nearDists.size();

        // save txt
        saveTxt<<m<<"  "<<mapCloud.descriptors(rowLineSalient, m)<<"  "<<Range<<"  "<<Height<<"  "<<Prob(0)<<"  "<<Prob(1)<<"  "<<Prob(2)<<"  "<<Prob(3)<<endl;
        // save mapCloud
        {
            mapCloud.descriptors(rowLineRange, m)=Range;
            mapCloud.descriptors(rowLineHeight, m)=Height;
            mapCloud.descriptors(rowLineBin0, m)=Prob(0);
            mapCloud.descriptors(rowLineBin1, m)=Prob(1);
            mapCloud.descriptors(rowLineBin2, m)=Prob(2);
            mapCloud.descriptors(rowLineBin3, m)=Prob(3);
        }

        double t1 = ros::Time::now().toSec();
        cout<<"Time:  "<<t1-t0<<endl;

    }

    // finally
    mapCloud.save(saveCloudName);

}


float Generation::getDistance(Eigen::Vector3f inputA, Eigen::Vector3f inputB)
{
    return sqrt(pow(inputA(0)-inputB(0),2) + pow(inputA(1)-inputB(1),2) + pow(inputA(2)-inputB(2),2));
}

float Generation::getLineDistance(Eigen::Vector3f node0, Eigen::Vector3f node1, Eigen::Vector3f nodeIn)
{
    // Heron's formula & 1/2ah
    float a = getDistance(node0, node1);
    float b = getDistance(node0, nodeIn);
    float c = getDistance(node1, nodeIn);
    float p = (a+b+c)/2;
    float S = sqrt(p*(p-a)*(p-b)*(p-c));
    return (2*S)/a;
}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "Generation");
    ros::NodeHandle n;

    Generation generation(n);

    // ugly code

    return 0;
}
