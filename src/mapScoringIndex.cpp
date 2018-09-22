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

    bool isKITTI;
    bool isChery;

    string wholeMapName;
    bool isFirstTime;
    string icpFileName;
    string velodyneDirName;
    string saveVTKname;
    string keepIndexName;

    int kSearch;
    double horizontalResRad;
    double limitRange;

    DP mapCloud;
    DP velodyneCloud;
    int rowLineSession;
    int rowLineLabel;

    unique_ptr<PM::Transformation> transformation;
    PM::TransformationParameters Trobot;
    PM::DataPointsFilters inputFilter;

    vector<vector<double>> initPoses;
    shared_ptr<NNS> featureNNS;
    vector<int> indexVector;

    void process(int indexCnt);
    DP readYQBin(string filename);
    DP readKITTIBin(string filename);

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
    saveVTKname(getParam<string>("saveVTKname", ".")),
    kSearch(getParam<int>("kSearch", 0)),
    horizontalResRad(getParam<double>("horizontalResRad", 0)),
    keepIndexName(getParam<string>("keepIndexName", ".")),
    limitRange(getParam<double>("limitRange", 0)),
    isKITTI(getParam<bool>("isKITTI", 0)),
    isChery(getParam<bool>("isChery", 0))
{
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
    indexVector.pop_back();  // last one repeated

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

void mapScoringIndex::process(int indexCnt)
{
    int index = indexVector.at(indexCnt);

    DP velodyneCloud;

    if(!isChery)
    {
        stringstream ss;
        if(isKITTI)
            ss<<setw(6)<<setfill('0')<<index;
        else
            ss<<setw(10)<<setfill('0')<<index;

        string str;
        ss>>str;
        string veloName = velodyneDirName + str + ".bin";
        cout<<veloName<<endl;

        if(isKITTI)
            velodyneCloud = this->readKITTIBin(veloName);       // KITTI dataset
        else
            velodyneCloud = this->readYQBin(veloName);  // YQ dataset
    }
    else
    {
        string vtkFileName = velodyneDirName + std::to_string(index) + ".vtk";
        velodyneCloud = DP::load(vtkFileName);  // Chery dataset
    }

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
            Matches::Dists(kSearch, velodyneCloud_.features.cols()),
            Matches::Ids(kSearch, velodyneCloud_.features.cols())
        );

        featureNNS->knn(velodyneCloud_.features, matches_overlap.ids, matches_overlap.dists, kSearch, 0);

        for(int m=0; m<velodyneCloud_.features.cols(); m++)
        {
            // Points find their way: a new approach
            // not _point

            double rangeFilter = this->limitRange;

            for(int k=0; k<kSearch; k++)
            {
                //core: rangeFilter? && Labeling && maxSession ?
                // rangeFilter, sqrt!!! lose before
                if(sqrt(matches_overlap.dists(k, m)) > rangeFilter
                   || mapCloud.descriptors(rowLineLabel, matches_overlap.ids(k, m)) == 1)
                    continue;
                else
                {
                    mapCloud.descriptors(rowLineSession, matches_overlap.ids(k, m)) = mapCloud.descriptors(rowLineSession, matches_overlap.ids(k, m)) + 1;
                    mapCloud.descriptors(rowLineLabel, matches_overlap.ids(k, m)) = 1; // labeled
                }
            }
        }
    }

    // clear mapCloud Labelled
    for(int m=0; m<mapCloud.descriptors.cols(); m++)
    {
        mapCloud.descriptors(rowLineLabel, m) = 0;
    }

}

mapScoringIndex::DP mapScoringIndex::readYQBin(string filename)
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

//For kitti dataset
mapScoringIndex::DP mapScoringIndex::readKITTIBin(string fileName)
{
    DP tempScan;

    int32_t num = 1000000;
    float *data = (float*)malloc(num*sizeof(float));

    // pointers
    float *px = data+0;
    float *py = data+1;
    float *pz = data+2;
    float *pr = data+3;

    // load point cloud
    FILE *stream;
    stream = fopen (fileName.c_str(),"rb");
    num = fread(data,sizeof(float),num,stream)/4;

    //ethz data structure
    tempScan.addFeature("x", PM::Matrix::Zero(1, num));
    tempScan.addFeature("y", PM::Matrix::Zero(1, num));
    tempScan.addFeature("z", PM::Matrix::Zero(1, num));
    tempScan.addDescriptor("intensity", PM::Matrix::Zero(1, num));

    int x = tempScan.getFeatureStartingRow("x");
    int y = tempScan.getFeatureStartingRow("y");
    int z = tempScan.getFeatureStartingRow("z");
    int intensity = tempScan.getDescriptorStartingRow("intensity");


    for (int32_t i=0; i<num; i++)
    {
        tempScan.features(x,i) = *px;
        tempScan.features(y,i) = *py;
        tempScan.features(z,i) = *pz;
        tempScan.descriptors(intensity,i) = *pr;
        px+=4; py+=4; pz+=4; pr+=4;
    }
    fclose(stream);

    ///free the ptr
    {
        free(data);
    }

    return tempScan;
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapScoringIndex");
    ros::NodeHandle n;

    mapScoringIndex mapScoringIndex_(n);

    // ugly code

    return 0;
}
