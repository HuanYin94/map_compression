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

#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>
#include <pcl/conversions.h>

using namespace PointMatcherSupport;
using namespace pcl;
using namespace pcl::io;

using namespace std;

class clusterMap
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;

public:
    clusterMap(ros::NodeHandle &n);
    ~clusterMap();
    ros::NodeHandle& n;

    string loadMapName;
    string saveMapName;

    DP mapCloud;
    DP saveCloud;

    double disThreshold;

    int minNum;
    int maxNum;
    int expectedNum;

    void process();

};

clusterMap::~clusterMap()
{}

clusterMap::clusterMap(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    saveMapName(getParam<string>("saveMapName", ".")),
    disThreshold(getParam<double>("disThreshold", 0)),
    expectedNum(getParam<int>("expectedNum", 0)),
    minNum(getParam<int>("minNum", 0)),
    maxNum(getParam<int>("maxNum", 0))
{

    // load
    mapCloud = DP::load(loadMapName);

    // process
    this->process();

}

void clusterMap::process()
{
    pcl::PCLPointCloud2 pcl_map;
    pcl_conversions::toPCL(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(mapCloud, "world", ros::Time(0)), pcl_map);
    pcl::PointCloud<pcl::PointXYZ>::Ptr mapCloud_pcl(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromPCLPointCloud2(pcl_map,*mapCloud_pcl);

    // Creating the KdTree object for the search method of the extraction
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
    tree->setInputCloud (mapCloud_pcl);

    std::vector<pcl::PointIndices> cluster_indices;
    pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
    ec.setClusterTolerance (disThreshold);
    ec.setMinClusterSize (minNum);
    ec.setMaxClusterSize (maxNum);
    ec.setSearchMethod (tree);
    ec.setInputCloud (mapCloud_pcl);
    ec.extract(cluster_indices);

    mapCloud.addDescriptor("cluster", PM::Matrix::Zero(1, mapCloud.features.cols()));
    int rowLineCluster = mapCloud.getDescriptorStartingRow("cluster");
    int j = 0;
    int sumKept = 0;
    for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it)
    {
        cout<<j<<"  Size:  "<<it->indices.size()<<endl;
        for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); ++pit)
        {
            mapCloud.descriptors(rowLineCluster, *pit) = 1;
        }
        j = j + 1;

        sumKept += it->indices.size();
        if(sumKept > expectedNum)
            break;
    }

    DP newMapCloud = mapCloud.createSimilarEmpty();
    int cnt=0;
    for(int m=0; m<mapCloud.features.cols(); m++)
    {
        if(mapCloud.descriptors(rowLineCluster, m) == 1)
        {
            newMapCloud.setColFrom(cnt, mapCloud, m);
            cnt++;
        }
    }
    newMapCloud.conservativeResize(cnt);

    newMapCloud.save(saveMapName);

    cout<<"Remains:  "<<newMapCloud.features.cols()<<endl;
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "clusterMap");
    ros::NodeHandle n;

    clusterMap clustermap(n);

    // ugly code

    return 0;
}
