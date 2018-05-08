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

class mapAccuLaser
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapAccuLaser(ros::NodeHandle &n);
    ~mapAccuLaser();
    ros::NodeHandle& n;

    string wholeMapName;
    bool isFirstTime;
    string icpFileName;
    string velodyneDirName;
    int totalICPnum;
    string inputFilterName;
    int maxSession;
    double rangeFilter;
    string saveVTKname;
    string savePnumsName;

    DP mapCloud;
    DP velodyneCloud;
    int rowLine;

    ros::Publisher mapCloudPub;
    ros::Publisher velodyneCloudPub;
    unique_ptr<PM::Transformation> transformation;
    PM::TransformationParameters Trobot;
    PM::DataPointsFilters inputFilter;

    vector<vector<double>> initPoses;
    shared_ptr<NNS> featureNNS;

    void process(int index);
    void wait();
    DP readBin(string fileName);

};

mapAccuLaser::~mapAccuLaser()
{}

mapAccuLaser::mapAccuLaser(ros::NodeHandle& n):
    n(n),
    wholeMapName(getParam<string>("wholeMapName", ".")),
    isFirstTime(getParam<bool>("isFirstTime", true)),
    icpFileName(getParam<string>("icpFileName", ".")),
    velodyneDirName(getParam<string>("velodyneDirName", ".")),
    transformation(PM::get().REG(Transformation).create("RigidTransformation")),
    totalICPnum(getParam<int>("totalICPnum", 0)),
    inputFilterName(getParam<string>("inputFilterName", ".")),
    maxSession(getParam<int>("maxSession", 0)),
    rangeFilter(getParam<double>("rangeFilter", 0)),
    saveVTKname(getParam<string>("saveVTKname", ".")),
    savePnumsName(getParam<string>("savePnumsName", "."))
{
    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("mapCloud", 2, true);
    velodyneCloudPub = n.advertise<sensor_msgs::PointCloud2>("velodyneCloud", 2, true);

    // load
    mapCloud = DP::load(wholeMapName);

    // is it the first time? creaste descriptor: session
    if(isFirstTime)
    {
        mapCloud.addDescriptor("session", PM::Matrix::Ones(1, mapCloud.features.cols()));
    }
    rowLine = mapCloud.getDescriptorStartingRow("session");

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
    for(; index < totalICPnum; index++)
    {
        this->process(index);
        cout<<"-------------------------------------------------------"<<endl;
    }

//    mapCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "global", ros::Time(0)));
//    velodyneCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(velodyneCloud, "global", ros::Time(0)));

    // count & save session nums
    {
        PM::Matrix nums = PM::Matrix::Zero(1, maxSession);
        int session;
        for(int i=0; i<mapCloud.features.cols(); i++)
        {
            session = mapCloud.descriptors(rowLine, i);
            nums(0, session-1) = nums(0, session-1) + 1;
        }

        ofstream saveNums(savePnumsName);
        for(int i=1; i<=maxSession; i++)
        {
            saveNums<<i<<"  "<<nums(0, i-1)<<endl;
        }
    }

    //save map
    mapCloud.save(saveVTKname);

    cout<<"All Finished!"<<endl;

//    this->wait();
}

void mapAccuLaser::process(int index)
{
    // loading velodyne
    stringstream ss;
    ss<<setw(10)<<setfill('0')<<index;
    string str;
    ss>>str;
    string veloName = velodyneDirName + str + ".bin";
    cout<<veloName<<endl;

    velodyneCloud = readBin(veloName);
    inputFilter.apply(velodyneCloud);

    Trobot = PM::TransformationParameters::Identity(4, 4);
    Trobot(0,0)=initPoses[index][0];Trobot(0,1)=initPoses[index][1];Trobot(0,2)=initPoses[index][2];Trobot(0,3)=initPoses[index][3];
    Trobot(1,0)=initPoses[index][4];Trobot(1,1)=initPoses[index][5];Trobot(1,2)=initPoses[index][6];Trobot(1,3)=initPoses[index][7];
    Trobot(2,0)=initPoses[index][8];Trobot(2,1)=initPoses[index][9];Trobot(2,2)=initPoses[index][10];Trobot(2,3)=initPoses[index][11];
    Trobot(3,0)=initPoses[index][12];Trobot(3,1)=initPoses[index][13];Trobot(3,2)=initPoses[index][14];Trobot(3,3)=initPoses[index][15];

    transformation->correctParameters(Trobot);

    if(transformation->checkParameters(Trobot))
    {
        DP velodyneCloud_ = transformation->compute(velodyneCloud, Trobot);

        PM::Matches matches_overlap(
            Matches::Dists(1, velodyneCloud_.features.cols()),
            Matches::Ids(1, velodyneCloud_.features.cols())
        );

        featureNNS->knn(velodyneCloud_.features, matches_overlap.ids, matches_overlap.dists, 1, 0);

        // label the session, core
        for(int m=0; m<velodyneCloud_.features.cols(); m++)
        {

            if(matches_overlap.dists(0, m) > rangeFilter
              || mapCloud.descriptors(rowLine, matches_overlap.ids(0, m)) >= maxSession)
            {
                continue;
            }
            else
            {
                mapCloud.descriptors(rowLine, matches_overlap.ids(0, m)) = mapCloud.descriptors(rowLine, matches_overlap.ids(0, m)) + 1;
            }
        }
    }


}

void mapAccuLaser::wait()
{
    ros::Rate r(1);

    while(ros::ok())
    {
    }
}

mapAccuLaser::DP mapAccuLaser::readBin(string filename)
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

    ros::init(argc, argv, "mapAccuLaser");
    ros::NodeHandle n;

    mapAccuLaser mapAccuLaser_(n);

    // ugly code

    return 0;
}
