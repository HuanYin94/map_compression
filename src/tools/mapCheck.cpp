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

class mapCheck
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapCheck(ros::NodeHandle &n);
    ~mapCheck();
    ros::NodeHandle& n;

    string loadMapName;
    string loadTrajName;
    ros::Publisher mapCloudPub;
    ros::Publisher staticCloudPub;
    ros::Publisher mapPathPub;
    ros::Publisher pathCloudPub;

    int staticInt;
    DP mapCloud;
    DP staticCloud;
    DP pathCloud;

    nav_msgs::Path mapPath;
    string keepIndexName;

    vector<vector<double>> initPoses;

    int showType;
    shared_ptr<NNS> featureNNS;

    void process();

};

mapCheck::~mapCheck()
{}

mapCheck::mapCheck(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    staticInt(getParam<int>("staticInt", 0)),
    loadTrajName(getParam<string>("loadTrajName", ".")),
    showType(getParam<int>("showType", 0))
{
    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("mapCloud", 2, true);
    staticCloudPub = n.advertise<sensor_msgs::PointCloud2>("staticCloud", 2, true);
    mapPathPub = n.advertise<nav_msgs::Path>("mapPath", 20000, true);
    pathCloudPub = n.advertise<sensor_msgs::PointCloud2>("pathCloud", 2, true);

    // load
    // get the loadtime
    double t0 = ros::Time::now().toSec();
    mapCloud = DP::load(loadMapName);
    double t1 = ros::Time::now().toSec();
    cout<<"-----------------------------------------"<<endl;
    cout<<"LOADING TIME COST:  "<<t1-t0<<"  seconds."<<endl;
    cout<<"-----------------------------------------"<<endl;

    // time for building KD-tree
    double t2 = ros::Time::now().toSec();
    featureNNS.reset(NNS::create(mapCloud.features, mapCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    double t3 = ros::Time::now().toSec();
    cout<<"-----------------------------------------"<<endl;
    cout<<"KD-TREE TIME COST:  "<<t3-t2<<"  seconds."<<endl;
    cout<<"-----------------------------------------"<<endl;

    // read initial transformation
    int x, y;
    double temp;
    vector<double> test;
    ifstream in(loadTrajName);
    if (!in) {
        cout << "Cannot open file.\n";
    }
    for (y = 0; y < 9999999; y++) {
        test.clear();
        if(in.eof()) break;
    for (x = 0; x < 16; x++) {
      in >> temp;
      test.push_back(temp);
    }
      initPoses.push_back(test);
    }
    in.close();

    // read initial transformation
    mapPath.header.frame_id = "global";
    mapPath.header.stamp = ros::Time::now();
    for (y = 0; y < initPoses.size(); y++)
    {

        geometry_msgs::PoseStamped pose;
        pose.header.frame_id = mapPath.header.frame_id;
        pose.header.stamp = mapPath.header.stamp;
        pose.pose.position.x = initPoses[y][3];
        pose.pose.position.y = initPoses[y][7];
        pose.pose.position.z = initPoses[y][11];
        pose.pose.orientation = tf::createQuaternionMsgFromYaw(0);  // always 0;

        mapPath.poses.push_back(pose);
    }

    // transfer the path to thecloud
    pathCloud = mapCloud.createSimilarEmpty();
    for(int p = 0; p< initPoses.size(); p++)
    {
        pathCloud.features(0, p) = initPoses[p][3];
        pathCloud.features(1, p) = initPoses[p][7];
        pathCloud.features(2, p) = initPoses[p][11];
    }
    pathCloud.conservativeResize(initPoses.size());

    // process
    this->process();

//    staticCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(staticCloud, "global", ros::Time(0)));
//    mapPathPub.publish(mapPath);
//    pathCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(pathCloud, "global", ros::Time(0)));

//    if(ros::ok())
//    {
        mapCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "global", ros::Time(0)));
        staticCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(staticCloud, "global", ros::Time(0)));
//        ros::Duration(10).sleep();
//    }
}

void mapCheck::process()
{
    /**  CHECK TEST  **/

    /**SALIENT**/
    if(showType == 1)
    {
        int rowLine = mapCloud.getDescriptorStartingRow("salient");
        staticCloud = mapCloud.createSimilarEmpty();
        int count=0;
        for(int i=0; i<mapCloud.features.cols(); i++)
        {
            if(mapCloud.descriptors(rowLine, i) == 1)
            {
                staticCloud.setColFrom(count, mapCloud, i);
                count++;
            }

        }

        staticCloud.conservativeResize(count);
    }
    else if(showType == 0)
    {
        /**SESSIONS**/

        int rowLine = mapCloud.getDescriptorStartingRow("session");
        staticCloud = mapCloud.createSimilarEmpty();
        int count=0;
        for(int i=0; i<mapCloud.features.cols(); i++)
        {
            if(mapCloud.descriptors(rowLine, i) >= staticInt)
            {
                staticCloud.setColFrom(count, mapCloud, i);
                count++;
            }

        }

        staticCloud.conservativeResize(count);
    }
    else if(showType == 2)
    {
        int rowLine = mapCloud.getDescriptorStartingRow("salient_predicted");
        staticCloud = mapCloud.createSimilarEmpty();
        int count=0;
        for(int i=0; i<mapCloud.features.cols(); i++)
        {
            if(mapCloud.descriptors(rowLine, i) == 1)
            {
                staticCloud.setColFrom(count, mapCloud, i);
                count++;
            }

        }

        staticCloud.conservativeResize(count);
    }


    cout<<"map num:  "<<mapCloud.features.cols()<<endl;
//    cout<<"salient num:  "<<staticCloud.features.cols()<<endl;
    cout<<"static num:  "<<staticCloud.features.cols()<<endl;

    // the first points of YQ map are not observed ? ...
//    cout<<mapCloud.features(0,0)<<"  "<<mapCloud.features(1,0)<<"  "<<mapCloud.features(2,0)<<endl;
//    cout<<mapCloud.features(0,1)<<"  "<<mapCloud.features(1,1)<<"  "<<mapCloud.features(2,1)<<endl;

//    ofstream salientStream;
//    ofstream pointStream;
//    string salientFileName = "/home/yh/salients.txt";
//    string pointFileName = "/home/yh/points.txt";
//    salientStream.open(salientFileName);
//    pointStream.open(pointFileName);

//    int rowLine = mapCloud.getDescriptorStartingRow("salient");
//    for(int i=0; i<mapCloud.features.cols(); i++)
//    {
//        salientStream << mapCloud.descriptors(rowLine, i) << endl;
//        pointStream << mapCloud.features(0, i) << "   "
//                        << mapCloud.features(1, i) << "   "
//                           << mapCloud.features(2, i) << endl;
//    }
//    salientStream.close();
//    pointStream.close();

    // ZXM
//    ofstream saverXYZ("/home/yh/xyz_b500.txt");
//    ofstream saverLabel("/home/yh/labe_b500.txt");
//    int rowLineS = mapCloud.getDescriptorStartingRow("salient");
//    for(int i=0; i<mapCloud.descriptors.cols(); i++)
//    {
//        saverXYZ<<mapCloud.features(0,i)<<"  "
//               <<mapCloud.features(1,i)<<"  "
//              <<mapCloud.features(2,i)<<endl;
//        saverLabel<<mapCloud.descriptors(rowLineS, i)<<endl;
//    }
//    saverXYZ.close();
//    saverLabel.close();
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapCheck");
    ros::NodeHandle n;

    mapCheck mapcheck(n);

    // ugly code

    return 0;
}
