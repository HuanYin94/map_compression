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

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/PCLPointCloud2.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/transforms.h>
#include <pcl/io/pcd_io.h>

#include <pcl/filters/approximate_voxel_grid.h>

#define protected public
#include <pcl/filters/voxel_grid_covariance.h>
#undef protected

using namespace std;
using namespace PointMatcherSupport;

class mapFeater
{
    typedef PointMatcher<float> PM;
    typedef PM::DataPoints DP;
    typedef PM::Matches Matches;

    typedef typename Nabo::NearestNeighbourSearch<float> NNS;
    typedef typename NNS::SearchType NNSearchType;

public:
    mapFeater(ros::NodeHandle &n);
    ~mapFeater();
    ros::NodeHandle& n;

    DP loadCloud;
    vector<vector<double>> initPoses;
    shared_ptr<NNS> featureNNSPose;
    shared_ptr<NNS> featureNNSPoint;

    string loadMapName;
    string trajICPName;
    int totalICPnum;
    int voxelSize;
    int minPointsPerVoxel;
    double minRatio;
    string saveVTKName;
    int kSearch;
    string saveFileName;

    void wait();
    void process();
    double getRangeOfPoint(Eigen::Vector3d p0, Eigen::Vector3d p1);
    double getAngleOfPoint(double range, double z0, double z1);


};

mapFeater::~mapFeater()
{}

mapFeater::mapFeater(ros::NodeHandle& n):
    n(n),
    loadMapName(getParam<string>("loadMapName", ".")),
    trajICPName(getParam<string>("trajICPName", ".")),
    totalICPnum(getParam<int>("totalICPnum", 0)),
    voxelSize(getParam<int>("voxelSize", 0)),
    minPointsPerVoxel(getParam<int>("minPointsPerVoxel", 0)),
    minRatio(getParam<double>("minRatio", 0)),
    saveVTKName(getParam<string>("saveVTKName", ".")),
    kSearch(getParam<int>("kSearch", 0)),
    saveFileName(getParam<string>("saveFileName", "."))
{
    // load map cloud
    loadCloud = DP::load(loadMapName);

    // read initial transformation
    int x, y;
    double temp;
    vector<double> test;
    ifstream in(trajICPName);
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

    this->process();

    this->wait();


}

void mapFeater::wait()
{
    ros::Rate r(1);

    while(ros::ok())
    {
    }
}

void mapFeater::process()
{
    // build kd-tree of poses
    // convert!
    PM::Matrix poses;
    poses.resize(3, totalICPnum);
    for(int p=0; p<totalICPnum; p++)
    {
        for(int q=0; q<3; q++)
        {
            poses(q, p) = initPoses[p][q*4 +3];
        }
    }
    cout<<"pose matrix:  "<<poses.rows()<<"  "<<poses.cols()<<endl;

    /** FEATURE ENIGEERING**/
    {
        // kd-tree of poses & points, search in trajectory and search in itself
        featureNNSPose.reset(NNS::create(poses, poses.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));
        featureNNSPoint.reset(NNS::create(loadCloud.features, loadCloud.features.rows() - 1, NNS::KDTREE_LINEAR_HEAP, NNS::TOUCH_STATISTICS));

        //nearest pose
        PM::Matches matches_pose(
            Matches::Dists(1, loadCloud.features.cols()),
            Matches::Ids(1, loadCloud.features.cols())
        );

        //k nearest points
        PM::Matches matches_point(
            Matches::Dists(kSearch, loadCloud.features.cols()),
            Matches::Ids(kSearch, loadCloud.features.cols())
        );

        featureNNSPose->knn(loadCloud.features, matches_pose.ids, matches_pose.dists, 1, 0);
        featureNNSPoint->knn(loadCloud.features, matches_point.ids, matches_point.dists, kSearch, 0);

        //prepare
        ofstream saveFeatures(saveFileName);
        int rowLineSession = loadCloud.getDescriptorStartingRow("session");

        // go through
        for(int m=0; m<loadCloud.features.cols(); m++)
        {
            int session = loadCloud.descriptors(rowLineSession, m);

            /**
             * @brief thePoint
             * kd-tree is simpler?
             */
            /*
            Eigen::Vector3d thePoint;
            thePoint(0)=loadCloud.features(0,m);thePoint(1)=loadCloud.features(1,m);thePoint(2)=loadCloud.features(2,m);

            Eigen::Vector3d thePose;
            thePose(0)=poses(0,matches_pose.ids(0, m));thePose(1)=poses(1,matches_pose.ids(0, m));thePose(2)=poses(2,matches_pose.ids(0, m));

            // the one
            double theRange = this->getRangeOfPoint(thePose, thePoint);
            double theAngle = this->getAngleOfPoint(theRange, thePose(2), thePoint(2));

            vector<double> rangeVector;
            vector<double> angleVector;
            vector<double> distVector;
            for(int k=0; k<kSearch; k++)
            {
                Eigen::Vector3d aPoint;
                aPoint(0)=loadCloud.features(0,matches_point.ids(k,m));
                aPoint(1)=loadCloud.features(1,matches_point.ids(k,m));
                aPoint(2)=loadCloud.features(2,matches_point.ids(k,m));

                rangeVector.push_back(this->getRangeOfPoint(thePose, aPoint));
                angleVector.push_back(this->getAngleOfPoint(rangeVector.at(k), thePose(2), aPoint(2)));
                distVector.push_back(this->getRangeOfPoint(thePoint, aPoint));
            }
            */


            // now, think differently for the same goal
            double theRange = sqrt(matches_pose.dists(0, m));
            double theAngle = this->getAngleOfPoint(theRange,
                                                    poses(2,matches_pose.ids(0, m)),
                                                    loadCloud.features(2,m));

            vector<double> rangeVector;
            vector<double> angleVector;
            vector<double> distVector;
            for(int k=0; k<kSearch; k++)
            {
                int mapID = matches_point.ids(k, m);

                rangeVector.push_back(sqrt(matches_pose.dists(0, mapID)));
                angleVector.push_back(this->getAngleOfPoint(rangeVector.at(k),
                                                            poses(2,matches_pose.ids(0, mapID)),
                                                            loadCloud.features(2,mapID)));
                distVector.push_back(sqrt(matches_point.dists(k, m)));
            }



            ///other features
            // std of Range
            double meanRange = std::accumulate(std::begin(rangeVector), std::end(rangeVector), 0.0) /  rangeVector.size();
            double accumRanges = 0;
            std::for_each (std::begin(rangeVector), std::end(rangeVector), [&](const double d0) {
                accumRanges  += (d0-meanRange)*(d0-meanRange);
            });
            double stdevRanges = sqrt(accumRanges/(rangeVector.size()-1));

            // std of Angles
            double meanAngle = std::accumulate(std::begin(angleVector), std::end(angleVector), 0.0) /  angleVector.size();
            double accumAngles = 0;
            std::for_each (std::begin(angleVector), std::end(angleVector), [&](const double d0) {
                accumAngles  += (d0-meanAngle)*(d0-meanAngle);
            });
            double stdevAngles = sqrt(accumAngles/(angleVector.size()-1));

            // mean of dists
            double meanDist = std::accumulate(std::begin(distVector), std::end(distVector), 0.0) /  distVector.size();


            //save in the file
            {
                saveFeatures<<theRange<<"  "
                           <<theAngle<<"  "
                          <<stdevRanges<<"  "
                            <<stdevAngles<<"  "
                              <<meanDist<<"  "
                                <<session<<endl;

                cout<<m<<"  "<<theRange<<"  "
                           <<theAngle<<"  "
                          <<stdevRanges<<"  "
                            <<stdevAngles<<"  "
                              <<meanDist<<"  "
                                <<session<<endl;
            }

        }

        cout<<"Finished"<<endl;

    }



    /** DAMN THE EIGEN_VALUES OF PCL-NDT**/
    /*
    //demo to show the eigenValues
    {
        cout<<"all Points num:  "<<loadCloud.features.cols()<<endl;
        //ehtz:DP to pcl
        pcl::PCLPointCloud2 loadCloudPCL;
        pcl_conversions::toPCL(PointMatcher_ros::pointMatcherCloudToRosMsg<float>(loadCloud, "global", ros::Time(0)), loadCloudPCL);
        pcl::PointCloud<pcl::PointXYZ>::Ptr loadCloudPCL_(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::fromPCLPointCloud2(loadCloudPCL, *loadCloudPCL_);
        cout<<"pcl num:  "<<loadCloudPCL_->size()<<endl;

        //voxelize and get eigens
        pcl::VoxelGridCovariance<pcl::PointXYZ> voxel;
        voxel.setInputCloud(loadCloudPCL_);
        voxel.setLeafSize(voxelSize, voxelSize, voxelSize);
        voxel.setMinPointPerVoxel(minPointsPerVoxel);
        voxel.setDownsampleAllData(false);
        voxel.setSaveLeafLayout(true);
        voxel.setCovEigValueInflationRatio(minRatio);
        voxel.filter(true);
        cout<<"after filter, pcl num:  "<<loadCloudPCL_->size()<<endl;

    //    //voxels & Leaves
    //    vector<pcl::VoxelGridCovariance::Leaf* > k_leaves;
    //    vector<float> k_sqr_distances;

        cout<<"leaves num:  "<<voxel.leaves_.size()<<endl;

        loadCloud.addDescriptor("ratio1", PM::Matrix::Zero(1, loadCloud.features.cols()));
        loadCloud.addDescriptor("ratio2", PM::Matrix::Zero(1, loadCloud.features.cols()));
        loadCloud.addDescriptor("ratio3", PM::Matrix::Zero(1, loadCloud.features.cols()));
        int rowLineR1 = loadCloud.getDescriptorStartingRow("ratio1");
        int rowLineR2 = loadCloud.getDescriptorStartingRow("ratio2");
        int rowLineR3 = loadCloud.getDescriptorStartingRow("ratio3");

        // new cloud to save
        DP saveCloud;
        saveCloud =loadCloud.createSimilarEmpty();
        int saveCount = 0;

        for(int i=0; i<loadCloud.features.cols(); i++)
        {
            cout<<i<<"  "<<endl;
            Eigen::Vector3f point;
            point[0]=loadCloud.features(0,i);point[1]=loadCloud.features(1,i);point[2]=loadCloud.features(2,i);
            pcl::VoxelGridCovariance<pcl::PointXYZ>::LeafConstPtr pointLeaf = voxel.getLeaf(point);

            Eigen::Vector3d evals_ = pointLeaf->getEvals();

            loadCloud.descriptors(rowLineR1,i) += evals_[1] / evals_[0];
            loadCloud.descriptors(rowLineR2,i) += evals_[2] / evals_[1];
            loadCloud.descriptors(rowLineR3,i) += evals_[2] / evals_[0];

            if(evals_[0]==0 || evals_[1]==0 || evals_[2]==0)
            {
                continue;
            }
            else
            {
                saveCloud.setColFrom(saveCount, loadCloud, i);
                saveCount++;
            }

        }

        saveCloud.conservativeResize(saveCount);
        cout<<"save points num:  "<<saveCloud.descriptors.cols()<<endl;
        saveCloud.save(saveVTKName);
        cout<<"save finished"<<endl;

    }
    */



}

double mapFeater::getRangeOfPoint(Eigen::Vector3d p0, Eigen::Vector3d p1)
{
    return sqrt(pow(p0(0)-p1(0),2) + pow(p0(1)-p1(1),2) + pow(p0(2)-p1(2),2));
}

double mapFeater::getAngleOfPoint(double range, double z0, double z1)
{
    double height = abs(z0-z1);
    return std::atan(height / range);
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "mapFeater");
    ros::NodeHandle n;

    mapFeater mapfeater(n);

    // ugly code

    return 0;
}
