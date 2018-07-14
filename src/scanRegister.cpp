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

class scanRegister
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

public:
    scanRegister(ros::NodeHandle &n);
    ~scanRegister();
    ros::NodeHandle& n;

    string icpFileName;
    string velodyneDirName;
    string saveVTKname;
    string keepIndexName;

    string inputFilterName;
    string mapPostFilterName;

    DP mapCloud;
    DP velodyneCloud;

    unique_ptr<PM::Transformation> transformation;
    PM::TransformationParameters Trobot;
    PM::DataPointsFilters mapFilter;
    PM::DataPointsFilters velFilter;

    vector<vector<double>> initPoses;
    vector<int> indexVector;

    void process(int indexCnt);
    DP readBin(string fileName);
};

scanRegister::~scanRegister()
{}

scanRegister::scanRegister(ros::NodeHandle& n):
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

    ifstream mapFilterss(mapPostFilterName);
    mapFilter = PM::DataPointsFilters(mapFilterss);

    ifstream velFilterss(inputFilterName);
    velFilter = PM::DataPointsFilters(velFilterss);


    // process
    int indexCnt = 0;
    for(; indexCnt < indexVector.size(); indexCnt++)
    {
        cout<<"------------------------------------------------------------------"<<endl;
        cout<<"The:  "<<indexCnt<<endl;
        this->process(indexCnt);
    }

    //save map
    cout<<mapCloud.features.cols()<<endl;
    mapFilter.apply(mapCloud);
    cout<<mapCloud.features.cols()<<endl;
    mapCloud.save(saveVTKname);

    cout<<"All Finished!"<<endl;
}

void scanRegister::process(int indexCnt)
{
    int index = indexVector.at(indexCnt);

    stringstream ss;
    ss<<setw(10)<<setfill('0')<<index;
    string str;
    ss>>str;
    string veloName = velodyneDirName + str + ".bin";
    cout<<veloName<<endl;

    velodyneCloud = readBin(veloName);
    velFilter.apply(velodyneCloud);

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
        mapCloud.concatenate(velodyneCloud_);

}

scanRegister::DP scanRegister::readBin(string filename)
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

int main(int argc, char **argv)
{

    ros::init(argc, argv, "scanRegister");
    ros::NodeHandle n;

    scanRegister scanRegister_(n);

    // ugly code

    return 0;
}
