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

class sessionFilter
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    sessionFilter(ros::NodeHandle &n);
    ~sessionFilter();
    ros::NodeHandle& n;

    ros::Publisher mapCloudPub;
    ros::Publisher salientCloudPub;

    DP mapCloud;
    DP salientCloud;

    string loadMapName;
    int staticInt;
    int rangeSearch;
    int minStaticPoints;
    string saveMapName;
    int kSearch;
    bool ifCluster;

    shared_ptr<NNS> featureNNS;

    void wait();
    void process();

};

sessionFilter::~sessionFilter()
{}

sessionFilter::sessionFilter(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveMapName(getParam<string>("saveMapName", ".")),
    staticInt(getParam<int>("staticInt", 0)),
    rangeSearch(getParam<int>("rangeSearch", 0)),
    minStaticPoints(getParam<int>("minStaticPoints", 0)),
    kSearch(getParam<int>("kSearch", 0)),
    ifCluster(getParam<bool>("ifCluster", false))
{
    mapCloudPub = n.advertise<sensor_msgs::PointCloud2>("mapCloud", 2, true);
    salientCloudPub = n.advertise<sensor_msgs::PointCloud2>("salientCloud", 2, true);

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();

    mapCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "global", ros::Time(0)));
    salientCloudPub.publish(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(salientCloud, "global", ros::Time(0)));

    this->wait();
}

void sessionFilter::wait()
{
    ros::Rate r(1);

    while(ros::ok())
    {
    }
}

void sessionFilter::process()
{

    //check if "salient" description exists
    if(!mapCloud.descriptorExists("salient"))
    {
        mapCloud.addDescriptor("salient", PM::Matrix::Zero(1, mapCloud.features.cols()));
    }

    int rowLineSession = mapCloud.getDescriptorStartingRow("session");
    int rowLineSalient = mapCloud.getDescriptorStartingRow("salient");

    featureNNS.reset(NNS::create(mapCloud.features, mapCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
    PM::Matches matches(
        Matches::Dists(kSearch, mapCloud.features.cols()),
        Matches::Ids(kSearch, mapCloud.features.cols())
    );
    featureNNS->knn(mapCloud.features, matches.ids, matches.dists, kSearch, 0);

    if(ifCluster)
    {
        for(int m=0; m<mapCloud.features.cols(); m++)
        {
            if(mapCloud.descriptors(rowLineSession,m) < this->staticInt) // not the sailent
            {
                mapCloud.descriptors(rowLineSalient,m) = 0;
            }
            else
            {
                int brother_cnt = 0;
                for(int k=0; k<kSearch; k++)
                {
                    if(matches.dists(k, m)<rangeSearch && mapCloud.descriptors(rowLineSession, matches.ids(k, m)) > staticInt)
                    {
                        brother_cnt ++;
                    }
                }


               // is the sailent?
                brother_cnt>minStaticPoints ? mapCloud.descriptors(rowLineSalient,m)=1 : mapCloud.descriptors(rowLineSalient,m)=0;

            }
        }
    }
    else /// origin session filter
    {
        for(int m=0; m<mapCloud.features.cols(); m++)
        {
            if(mapCloud.descriptors(rowLineSession,m) < this->staticInt) // not the sailent
            {
                mapCloud.descriptors(rowLineSalient,m) = 0;
            }
            else            // is
            {
                mapCloud.descriptors(rowLineSalient,m) = 1;
            }
        }
    }


    // salient cloud to show
    salientCloud = mapCloud.createSimilarEmpty();
    int count = 0;
    for(int i=0; i<mapCloud.features.cols(); i++)
    {
        if(mapCloud.descriptors(rowLineSalient, i) == 1)
        {
            salientCloud.setColFrom(count, mapCloud, i);
            count++;
        }

    }

    salientCloud.conservativeResize(count);
    cout<<"Salient Size:  "<<salientCloud.features.cols()<<endl;

    // save
    salientCloud.save(saveMapName);
    cout<<"saved"<<endl;

}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "sessionFilter");
    ros::NodeHandle n;

    sessionFilter sessionfilter(n);

    // ugly code

    return 0;
}
