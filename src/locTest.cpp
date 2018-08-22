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

class locTest
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;

public:
    locTest(ros::NodeHandle &n);
    ~locTest();
    ros::NodeHandle& n;

    string wholeMapName;
    string icpFileName;
    string velodyneDirName;
    string keepIndexName;

    string saveLocName;
    string saveTimeName;

    string icpYaml;
    string inputFilterYaml;

    DP mapCloud;
    DP velodyneCloud;

    double deltaTime;

    int startIndex;
    int endIndex;

    bool isKITTI;

    //icp
    PM::DataPointsFilters inputFilters;
    PM::ICPSequence icp;
    PM::TransformationParameters Ticp;
    PM::TransformationParameters Tinit;

    vector<vector<double>> initPoses;
    vector<int> indexVector;

    void process(int index);
    DP readYQBin(string filename);
    DP readKITTIBin(string filename);

    ros::Publisher mapCloudPub;
    ros::Publisher velCloudPub;
    tf::TransformBroadcaster tfBroadcaster;

};

locTest::~locTest()
{}

locTest::locTest(ros::NodeHandle& n):
    n(n),
    wholeMapName(getParam<string>("wholeMapName", ".")),
    icpFileName(getParam<string>("icpFileName", ".")),
    startIndex(getParam<int>("startIndex", 0)),
    endIndex(getParam<int>("endIndex", 0)),
    icpYaml(getParam<string>("icpYaml", ".")),
    velodyneDirName(getParam<string>("velodyneDirName", ".")),
    saveLocName(getParam<string>("saveLocName", ".")),
    saveTimeName(getParam<string>("saveTimeName", ".")),
    inputFilterYaml(getParam<string>("inputFilterYaml", ".")),
    isKITTI(getParam<bool>("isKITTI", ".")),
    keepIndexName(getParam<string>("keepIndexName", "."))
{
    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("map_cloud", 2, true);
    velCloudPub = n.advertise<sensor_msgs::PointCloud2>("velodyne_cloud", 2, true);

    // load
    mapCloud = DP::load(wholeMapName);

    // read initial transformation
    int x, y;
    double temp;
    vector<double> test;
    ifstream in(icpFileName);
    if (!in) {
        cout << "Cannot open file.\n";
    }
    for (y = 0; y < 99999; y++) {
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

    cout<<"start locTest"<<endl;

    // initial icp
    ifstream ifs(icpYaml);
    icp.loadFromYaml(ifs);
    icp.setMap(mapCloud);

    ofstream saverTime(saveTimeName);
    ofstream saverLoc(saveLocName);

    // process
    // from start to end
    for(int index = indexVector.at(startIndex); index<=indexVector.at(endIndex); index++)
    {
        this->process(index);

        //saving...
        if(index!=startIndex)
        {
            saverTime<<deltaTime<<endl;
            saverLoc<<Ticp(0,0)<<"  "<<Ticp(0,1)<<"  "<<Ticp(0,2)<<"  "<<Ticp(0,3)<<"  "
                      <<Ticp(1,0)<<"  "<<Ticp(1,1)<<"  "<<Ticp(1,2)<<"  "<<Ticp(1,3)<<"  "
                     <<Ticp(2,0)<<"  "<<Ticp(2,1)<<"  "<<Ticp(2,2)<<"  "<<Ticp(2,3)<<"  "
                    <<Ticp(3,0)<<"  "<<Ticp(3,1)<<"  "<<Ticp(3,2)<<"  "<<Ticp(3,3)<<endl;
        }

        // publish
        velCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(velodyneCloud, "robot", ros::Time::now()));
        mapCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "global", ros::Time::now()));
        tfBroadcaster.sendTransform(PointMatcher_ros::eigenMatrixToStampedTransform<float>(Ticp, "global", "robot", ros::Time::now()));
    }

}

void locTest::process(int index)
{
    // first time, just set the initial value
    if(index == startIndex)
    {
        Tinit = PM::TransformationParameters::Identity(4, 4);
        Tinit(0,0)=initPoses[index][0];Tinit(0,1)=initPoses[index][1];Tinit(0,2)=initPoses[index][2];Tinit(0,3)=initPoses[index][3];
        Tinit(1,0)=initPoses[index][4];Tinit(1,1)=initPoses[index][5];Tinit(1,2)=initPoses[index][6];Tinit(1,3)=initPoses[index][7];
        Tinit(2,0)=initPoses[index][8];Tinit(2,1)=initPoses[index][9];Tinit(2,2)=initPoses[index][10];Tinit(2,3)=initPoses[index][11];
        Tinit(3,0)=initPoses[index][12];Tinit(3,1)=initPoses[index][13];Tinit(3,2)=initPoses[index][14];Tinit(3,3)=initPoses[index][15];
        return;
    }

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

    // icp

    // bug "Ignore..." fixed, quat!
    Eigen::Matrix3f BaseToMapRotation = Tinit.block(0,0,3,3);
    Eigen::AngleAxisf BaseToMapAxisAngle(BaseToMapRotation);    // RotationMatrix to AxisAngle
    Tinit.block(0,0,3,3) = BaseToMapAxisAngle.toRotationMatrix();

//    cout<<Tinit<<endl;

    double t0 = ros::Time::now().toSec();

    Ticp = icp(velodyneCloud, Tinit);

    double t1 = ros::Time::now().toSec();
    deltaTime = t1-t0;

    Tinit = Ticp;

    // save the results outside this function

}

locTest::DP locTest::readYQBin(string filename)
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
locTest::DP locTest::readKITTIBin(string fileName)
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

    ros::init(argc, argv, "locTest");
    ros::NodeHandle n;

    locTest loctest(n);
    ros::spin();

    return 0;
}
