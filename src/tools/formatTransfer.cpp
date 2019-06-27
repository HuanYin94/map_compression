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

#include <stdio.h>
#include <dirent.h>

using namespace std;
using namespace PointMatcherSupport;

class formatTransfer
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;

public:
    formatTransfer(ros::NodeHandle &n);
    ~formatTransfer();
    ros::NodeHandle& n;

    string loadMapDir;
    string loadFormat;
    string saveFormat;

    string fileName;
    string globalPath;
    DP mapCloud;
};

formatTransfer::~formatTransfer()
{}

formatTransfer::formatTransfer(ros::NodeHandle& n):
    n(n),
    loadMapDir(getParam<string>("loadMapDir", ".")),
    loadFormat(getParam<string>("loadFormat", ".")),
    saveFormat(getParam<string>("saveFormat", "."))
{
    char* p=(char*)loadMapDir.data();

    struct dirent *ptr;
    DIR *dir;
    dir = opendir(p);

    while((ptr = readdir(dir)) != NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;

        fileName = ptr->d_name;

        globalPath = loadMapDir + fileName;

        string::size_type idx;

        idx = globalPath.find(loadFormat);

        if(idx == string::npos )
            continue;
        else  // replace
        {
            cout<<"PROCESSING:  "<<globalPath<<endl;
            mapCloud = DP::load(globalPath);
            globalPath.replace(idx, saveFormat.length(), saveFormat);
            mapCloud.save(globalPath);
        }
    }
    closedir(dir);
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "formatTransfer");
    ros::NodeHandle n;

    formatTransfer formattransfer(n);

    // ugly code

    return 0;
}
