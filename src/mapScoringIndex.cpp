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

class mapScoringIndex
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapScoringIndex(ros::NodeHandle &n);
    ~mapScoringIndex();
    ros::NodeHandle& n;

    string wholeMapName;
    bool isFirstTime;
    string icpFileName;
    string velodyneDirName;
    string inputFilterName;
    string saveVTKname;
    string keepIndexName;
    int kSearch;
    double horizontalResRad;


    DP mapCloud;
    DP velodyneCloud;
    int rowLineSession;
    int rowLineLabel;

    ros::Publisher mapCloudPub;
    ros::Publisher velodyneCloudPub;
    unique_ptr<PM::Transformation> transformation;
    PM::TransformationParameters Trobot;
    PM::DataPointsFilters inputFilter;

    vector<vector<double>> initPoses;
    shared_ptr<NNS> featureNNS;

    void process(int index);
    DP readBin(string fileName);
    double getRangeOfPoint(double x, double y, double z);

};

mapScoringIndex::~mapScoringIndex()
{}

mapScoringIndex::mapScoringIndex(ros::NodeHandle& n):
    n(n),
    wholeMapName(getParam<string>("wholeMapName", ".")),
    isFirstTime(getParam<bool>("isFirstTime", true)),
    icpFileName(getParam<string>("icpFileName", ".")),
    velodyneDirName(getParam<string>("velodyneDirName", ".")),
    transformation(PM::get().REG(Transformation).create("RigidTransformation")),
    inputFilterName(getParam<string>("inputFilterName", ".")),
    saveVTKname(getParam<string>("saveVTKname", ".")),
    kSearch(getParam<int>("kSearch", 0)),
    horizontalResRad(getParam<double>("horizontalResRad", 0)),
    keepIndexName(getParam<string>("keepIndexName", "."))
{
    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("mapCloud", 2, true);
    velodyneCloudPub = n.advertise<sensor_msgs::PointCloud2>("velodyneCloud", 2, true);

    // load
    mapCloud = DP::load(wholeMapName);

    // is it the first time? creaste descriptor: session
    if(isFirstTime)
    {
        mapCloud.addDescriptor("session", PM::Matrix::Ones(1, mapCloud.features.cols()));
        mapCloud.addDescriptor("isLabel", PM::Matrix::Zero(1, mapCloud.features.cols()));
    }
    rowLineSession = mapCloud.getDescriptorStartingRow("session");
    rowLineLabel = mapCloud.getDescriptorStartingRow("isLabel");

    featureNNS.reset(NNS::create(mapCloud.features, mapCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));

    // read initial transformation
    int x, y;
    double temp;
    vector<double> test;
    ifstream in(icpFileName);
    if (!in) {
        cout << "Cannot open file.\n";
    }
    for (y = 0; y < totalICPnum; y++) {
        test.clear();
    for (x = 0; x < 16; x++) {
      in >> temp;
      test.push_back(temp);
    }
      initPoses.push_back(test);
    }
    in.close();

    ifstream inputiffilter(inputFilterName);
    inputFilter = PM::DataPointsFilters(inputiffilter);

    // process
    int index = 0;
    lastPosition(0)=initPoses[0][3];lastPosition(1)=initPoses[0][7];lastPosition(2)=initPoses[0][11];
    for(; index < this->totalICPnum; index++)
    {
        this->process(index);
        cout<<"-------------------------------------------------------"<<endl;
    }

    //save map
    mapCloud.save(saveVTKname);

    cout<<"All Finished!"<<endl;

//    this->wait();
}

void mapScoringIndex::process(int index)
{


}

mapScoringIndex::DP mapScoringIndex::readBin(string filename)
{
    DP data;

    fstream input(filename.c_str(), ios::in | ios::binary);
    if(!input.good()){
        cerr << "Could not read file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    input.seekg(0, ios::beg);

    data.addFeature("x",PM::Matrix::Constant(1,300000,0));
    data.addFeature("y",PM::Matrix::Constant(1,300000,0));
    data.addFeature("z",PM::Matrix::Constant(1,300000,0));
    data.addFeature("pad",PM::Matrix::Constant(1,300000,1));
    data.addDescriptor("intensity",PM::Matrix::Constant(1,300000,0));
    data.addDescriptor("ring",PM::Matrix::Constant(1,300000,0));

    int i;

    for (i=0; input.good() && !input.eof(); i++) {
        float a,b;
        input.read((char *) &a, 4*sizeof(unsigned char));
        data.features(0,i) = a;
        input.read((char *) &a, 4*sizeof(unsigned char));
        data.features(1,i) = a;
        input.read((char *) &a, 4*sizeof(unsigned char));
        data.features(2,i) = a;
        input.read((char *) &b, 4*sizeof(unsigned char));
        input.read((char *) &a, 4*sizeof(unsigned char));
        data.descriptors(0,i) = a;
        input.read((char *) &a, 2*sizeof(unsigned char));
        data.descriptors(1,i) = a;

        input.read((char *) &b, 10*sizeof(unsigned char));

    }
    input.close();
    data.conservativeResize(i);

    return data;
}

double mapScoringIndex::getRangeOfPoint(double x, double y, double z)
{
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapScoringIndex");
    ros::NodeHandle n;

    mapScoringIndex mapScoringIndex_(n);

    // ugly code

    return 0;
}
