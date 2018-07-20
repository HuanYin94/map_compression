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
#include <stdio.h>
#include <iomanip>

#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>

#include <numeric>

using namespace std;
using namespace PointMatcherSupport;

class kittiRegister
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

public:
    kittiRegister(ros::NodeHandle &n);
    ~kittiRegister();
    ros::NodeHandle& n;

    string icpFileName;
    string velodyneDirName;
    string saveVTKname;

    string mapPostFilterName;
    string inputFilterName;

    DP mapCloud;
    string keepIndexName;
    vector<int> indexVector;

    unique_ptr<PM::Transformation> transformation;
    PM::TransformationParameters Trobot;

    PM::DataPointsFilters mapFilter;
    PM::DataPointsFilters inputFilter;

    vector<vector<double>> initPoses;

    void process(int indexCnt);
};

kittiRegister::~kittiRegister()
{}

kittiRegister::kittiRegister(ros::NodeHandle& n):
    n(n),
    icpFileName(getParam<string>("icpFileName", ".")),
    velodyneDirName(getParam<string>("velodyneDirName", ".")),
    transformation(PM::get().REG(Transformation).create("RigidTransformation")),
    mapPostFilterName(getParam<string>("mapPostFilterName", ".")),
    inputFilterName(getParam<string>("inputFilterName", ".")),
    saveVTKname(getParam<string>("saveVTKname", ".")),
    keepIndexName(getParam<string>("keepIndexName", "."))
{

    // read initial transformation
    int x, y;
    double temp;
    vector<double> test;
    ifstream in(icpFileName);
    if (!in) {
        cout << "Cannot open file.\n";
    }
    for (y = 0; y < 9999999; y++) {
        test.clear();
    for (x = 0; x < 12; x++) {
      in >> temp;
      test.push_back(temp);
    }
      initPoses.push_back(test);
    }
    in.close();

    ifstream mapFilterss(mapPostFilterName);
    mapFilter = PM::DataPointsFilters(mapFilterss);

    ifstream inputFilterss(inputFilterName);
    inputFilter = PM::DataPointsFilters(inputFilterss);

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
    int indexCnt = 0;
    for(; indexCnt < indexVector.size(); indexCnt++)
    {
        cout<<"------------------------------------------------------------------"<<endl;
        cout<<"The:  "<<indexCnt<<endl;
        this->process(indexCnt);
    }
    //save map
    mapCloud.save(saveVTKname);
    cout<<"All Finished!"<<endl;
}

void kittiRegister::process(int indexCnt)
{
    int index = indexVector.at(indexCnt);

    stringstream ss;
    ss<<index;
    string str;
    ss>>str;
    string veloName = velodyneDirName + str + ".vtk";

    DP velodyneCloud = DP::load(veloName);
    inputFilter.apply(velodyneCloud);

    Trobot = PM::TransformationParameters::Identity(4, 4);
    Trobot(0,0)=initPoses[index][0];Trobot(0,1)=initPoses[index][1];Trobot(0,2)=initPoses[index][2];Trobot(0,3)=initPoses[index][3];
    Trobot(1,0)=initPoses[index][4];Trobot(1,1)=initPoses[index][5];Trobot(1,2)=initPoses[index][6];Trobot(1,3)=initPoses[index][7];
    Trobot(2,0)=initPoses[index][8];Trobot(2,1)=initPoses[index][9];Trobot(2,2)=initPoses[index][10];Trobot(2,3)=initPoses[index][11];
    Trobot(3,0)=initPoses[index][12];Trobot(3,1)=initPoses[index][13];Trobot(3,2)=initPoses[index][14];Trobot(3,3)=initPoses[index][15];

    transformation->correctParameters(Trobot);

    DP velodyneCloud_ = transformation->compute(velodyneCloud, Trobot);

    if(indexCnt == 0)
        mapCloud = velodyneCloud_;
    else
    {
        mapCloud.concatenate(velodyneCloud_);
        mapFilter.apply(mapCloud);
    }

    cout<<"map Size:    "<<mapCloud.features.cols()<<endl;
    mapCloud.save(saveVTKname);

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "kittiRegister");
    ros::NodeHandle n;

    kittiRegister kittiRegister_(n);

    // ugly code

    return 0;
}
