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

using namespace std;
using namespace PointMatcherSupport;

class genVisMatrix
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    genVisMatrix(ros::NodeHandle &n);
    ~genVisMatrix();
    ros::NodeHandle& n;

    bool isKITTI;

    string loadMapName;
    string loadTrajName;
    string loadVeloDir;
    string keepIndexName;

    DP mapCloud;
    DP velodyneCloud;
    PM::DataPointsFilters inputFilter;

    double limitRange;
    int kSearch;
    int rowLineLabel;

    vector<vector<double>> initPoses;
    PM::TransformationParameters Trobot;
    unique_ptr<PM::Transformation> transformation;
    vector<int> indexVector;
    shared_ptr<NNS> featureNNS;

    // if need the inout filter? confused
    PM::DataPointsFilters inputFilters;

    string saveDirName;

    void process(int index);
    DP readBin(string fileName);

};

genVisMatrix::~genVisMatrix()
{}

genVisMatrix::genVisMatrix(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    loadTrajName(getParam<string>("loadTrajName", ".")),
    loadVeloDir(getParam<string>("loadVeloDir", ".")),
    keepIndexName(getParam<string>("keepIndexName", ".")),
    limitRange(getParam<double>("limitRange", 0)),
    kSearch(getParam<int>("kSearch", 0)),
    transformation(PM::get().REG(Transformation).create("RigidTransformation")),
    saveDirName(getParam<string>("saveDirName", ".")),
    isKITTI(getParam<bool>("isKITTI", 0))
{

    // load
    mapCloud = DP::load(loadMapName);
    featureNNS.reset(NNS::create(mapCloud.features, mapCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));

    mapCloud.addDescriptor("isLabel", PM::Matrix::Zero(1, mapCloud.features.cols()));
    rowLineLabel = mapCloud.getDescriptorStartingRow("isLabel");


    // read initial transformation
    int x, y;
    double temp;
    vector<double> test;
    ifstream in(loadTrajName);
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

    // process, wanna see all
    int indexCnt = 0;
    for(; indexCnt < indexVector.size(); indexCnt++)
    {
        this->process(indexCnt);
    }

}

void genVisMatrix::process(int indexCnt)
{

    int index = indexVector.at(indexCnt);

    cout<<"------------------------------------------------------------------"<<endl;
    stringstream ss;

    if(isKITTI)
        ss<<index;
    else
        ss<<setw(10)<<setfill('0')<<index;

    string str;
    ss>>str;

    string veloName;
    if(isKITTI)
        veloName = loadVeloDir + str + ".vtk";
    else
        veloName = loadVeloDir + str + ".bin";

    cout<<veloName<<endl;

    if(isKITTI)
        velodyneCloud = DP::load(veloName);       // KITTI dataset
    else
        velodyneCloud = this->readBin(veloName);  // YQ dataset

    Trobot= PM::TransformationParameters::Identity(4, 4);
    Trobot(0,0)=initPoses[index][0];Trobot(0,1)=initPoses[index][1];Trobot(0,2)=initPoses[index][2];Trobot(0,3)=initPoses[index][3];
    Trobot(1,0)=initPoses[index][4];Trobot(1,1)=initPoses[index][5];Trobot(1,2)=initPoses[index][6];Trobot(1,3)=initPoses[index][7];
    Trobot(2,0)=initPoses[index][8];Trobot(2,1)=initPoses[index][9];Trobot(2,2)=initPoses[index][10];Trobot(2,3)=initPoses[index][11];
    Trobot(3,0)=initPoses[index][12];Trobot(3,1)=initPoses[index][13];Trobot(3,2)=initPoses[index][14];Trobot(3,3)=initPoses[index][15];

    transformation->correctParameters(Trobot);

    // save the indexes to the txt
    ofstream rowOfMatrix;
    string fileName = saveDirName + std::to_string(indexCnt) + ".txt";
    rowOfMatrix.open(fileName);
    int matchedMapCnt = 0;

    if(transformation->checkParameters(Trobot))
    {

        DP velodyneCloud_ = transformation->compute(velodyneCloud, Trobot);

        // search the nearest
        PM::Matches matches_overlap(
            Matches::Dists(kSearch, velodyneCloud_.features.cols()),
            Matches::Ids(kSearch, velodyneCloud_.features.cols())
        );

        featureNNS->knn(velodyneCloud_.features, matches_overlap.ids, matches_overlap.dists, kSearch, 0);

        for(int m=0; m<velodyneCloud_.features.cols(); m++)
        {
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
                    mapCloud.descriptors(rowLineLabel, matches_overlap.ids(k, m)) = 1; // labeled
                    rowOfMatrix << matches_overlap.ids(k, m) << endl;
                    matchedMapCnt++;
                }
            }
        }
    }

    // clear mapCloud Labelled
    for(int m=0; m<mapCloud.descriptors.cols(); m++)
    {
        mapCloud.descriptors(rowLineLabel, m) = 0;
    }

    cout<<"matched map points Cnt:  "<<matchedMapCnt<<endl;
    rowOfMatrix.close();
    cout<<"saved~"<<endl;

}

genVisMatrix::DP genVisMatrix::readBin(string filename)
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

    ros::init(argc, argv, "genVisMatrix");
    ros::NodeHandle n;

    genVisMatrix genvismatrix(n);
    ros::spin();

    return 0;
}
