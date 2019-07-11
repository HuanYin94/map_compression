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

class renameLaserScans
{

public:
    renameLaserScans(ros::NodeHandle &n);
    ~renameLaserScans();
    ros::NodeHandle& n;

    bool isChery;
    bool isKITTI;

    string indexFileName;
    string saveScanDir;
    string loadScanDir;

    vector<int> indexVector;

    void process(int cnt);
    void filecopy(FILE *dest, FILE *src);
};

renameLaserScans::~renameLaserScans()
{}

renameLaserScans::renameLaserScans(ros::NodeHandle& n):
    n(n),
    isChery(getParam<bool>("isChery", false)),
    isKITTI(getParam<bool>("isKITTI", false)),
    indexFileName(getParam<string>("indexFileName", ".")),
    loadScanDir(getParam<string>("loadScanDir", ".")),
    saveScanDir(getParam<string>("saveScanDir", "."))
{

    int temp;
    ifstream in(indexFileName);
    for(int y=0; y<999999; y++)
    {
        if(in.eof()) break;
        in >> temp;
        indexVector.push_back(temp);
    }
    in.close();

    indexVector.pop_back();

    for(int cnt=0; cnt<indexVector.size(); cnt++)
    {
        this->process(cnt);

        cout<<cnt<<"    "<<indexVector.size()<<endl;
    }

}

void renameLaserScans::process(int cnt)
{

    int oldIndex = indexVector.at(cnt);
    int newIndex = cnt;

    string veloNameOld, veloNameNew;

    if(!isChery)
    {

        stringstream newSs, oldSs;
        if(isKITTI)
        {
            oldSs<<setw(6)<<setfill('0')<<oldIndex;
            newSs<<setw(6)<<setfill('0')<<newIndex;
        }
        else
        {
            oldSs<<setw(10)<<setfill('0')<<oldIndex;
            newSs<<setw(10)<<setfill('0')<<newIndex;
        }

        string oldStr, newStr;
        oldSs >> oldStr;
        newSs >> newStr;

        veloNameOld = loadScanDir + oldStr + ".bin";
        veloNameNew = saveScanDir + newStr + ".bin";
    }
    else
    {
        veloNameOld = loadScanDir + std::to_string(oldIndex) + ".vtk";
        veloNameNew = saveScanDir + std::to_string(newIndex) + ".vtk";
    }

//    int result = rename( veloNameOld.c_str() , veloNameNew.c_str() );
//    if ( result == 0 )
//    {
//        puts ( "File successfully renamed  ");
//    }
//    else
//    {
//        cout<<veloNameOld<<endl;
//        cout<<veloNameNew<<endl;
//        perror( "Error renaming file  ");
//    }



//    std::ifstream srce( veloNameOld.c_str(), std::ios::binary ) ;
//    std::ofstream dest( veloNameNew.c_str(), std::ios::binary ) ;
//    dest << srce.rdbuf() ;



//    FILE * infile  = fopen(veloNameOld.c_str(),  "rb");
//    FILE * outfile = fopen(veloNameNew.c_str(), "wb");
//    filecopy(outfile, infile);
//    fclose(infile);
//    fclose(outfile);





//    FILE * infile  = fopen(veloNameOld.c_str(), "rb");
//    FILE * outfile = fopen(veloNameNew.c_str(), "wb");

//    char  buffer[1024];

//    fseek(infile, 8, SEEK_SET); /* omit first 8 bytes */

//    size_t count_in;

//    /* copy from input to output */
//    while (count_in = fread(buffer, 1, sizeof(buffer), infile))
//        fwrite(buffer, 1, count_in, outfile);

//    fclose(infile);
//    fclose(outfile);

    string command = "cp " + veloNameOld + " " + veloNameNew;
    char command_[256];
    strcpy(command_, command.c_str());
    system(command_);


}

void renameLaserScans::filecopy(FILE *dest, FILE *src)
{
    char ch;
    rewind(src);
    rewind(dest);
    while((ch=fgetc(src)) != EOF)
    {
        fputc(ch, dest);
    }
    fflush(dest);
    rewind(src);
    rewind(dest);
}

int main(int argc, char **argv)
{

    ros::init(argc, argv, "renameLaserScans");
    ros::NodeHandle n;

    renameLaserScans renameLaserScans_(n);

    // ugly code

    return 0;
}
