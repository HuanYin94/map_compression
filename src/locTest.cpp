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
    typedef PM::Matches Matches;

public:
    locTest(ros::NodeHandle &n);
    ~locTest();
    ros::NodeHandle& n;

    string loadMapName;
    string loadTrajName;
    string icpYaml;
    string loadVeloDir;

    DP mapCloud;

    int mapLength;
    int startIndex;
    int endIndex;

    vector<vector<double>> initPoses;

    //icp
    PM::ICPSequence icp;
    PM::TransformationParameters Ticp;
    PM::TransformationParameters Tinit;

    Eigen::Vector2d gnd;
    Eigen::Vector2d loc;

    void process(int index);
    double getLocError(Eigen::Vector2d gnd, Eigen::Vector2d loc);

};

locTest::~locTest()
{}

locTest::locTest(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    loadTrajName(getParam<string>("loadTrajName", ".")),
    mapLength(getParam<int>("mapLength", 0)),
    startIndex(getParam<int>("startIndex", 0)),
    endIndex(getParam<int>("endIndex", 0)),
    icpYaml(getParam<string>("icpYaml", ".")),
    loadVeloDir(getParam<string>("loadVeloDir", "."))
{

    // load
    mapCloud = DP::load(loadMapName);

    // read initial transformation
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

    cout<<"start locTest"<<endl;

    // initial icp
    ifstream ifs(icpYaml);
    icp.loadFromYaml(ifs);
    icp.setMap(mapCloud);

    // process
    // from start to end
    for(int index = startIndex; index<=endIndex; index++)
    {
        this->process(index);
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

    // load velodyne data
    stringstream ss;
    ss<<index;
    string str;
    ss>>str;
    string veloName = loadVeloDir + str + ".vtk";
    DP velodyneCloud = DP::load(veloName);

    cout<<"------------------------------------------------------------------"<<endl;
    cout<<veloName<<endl;

    // icp

    // bug "Ignore..." fixed, quat!
    Eigen::Matrix3f BaseToMapRotation = Tinit.block(0,0,3,3);
    Eigen::AngleAxisf BaseToMapAxisAngle(BaseToMapRotation);    // RotationMatrix to AxisAngle
    Tinit.block(0,0,3,3) = BaseToMapAxisAngle.toRotationMatrix();

    Ticp = icp(velodyneCloud, Tinit);

    Tinit = Ticp;

    //check the error
    gnd(0) = initPoses[index][3];    gnd(1) = initPoses[index][7];
    loc(0) = Ticp(0, 3);    loc(1) = Ticp(1, 3);
    double error = this->getLocError(gnd, loc);
    cout<<"The position is away:    "<<error<<"     meters."<<endl;


}

double locTest::getLocError(Eigen::Vector2d gnd, Eigen::Vector2d loc)
{
    return sqrt(pow(gnd(0)-loc(0),2) + pow(gnd(1)-loc(1),2));
}


int main(int argc, char **argv)
{

    ros::init(argc, argv, "locTest");
    ros::NodeHandle n;

    locTest loctest(n);

    return 0;
}
