## Welcome
Code of a manuscript, submitted to IEEE Transactions on Intelligent Transportation Systems, under 2nd review.

If you need the manuscript or have some questions on this project, please contact `zjuyinhuan@gmail.com` ([Huan Yin](https://yinhuan.site/)) . :blush:

# 3D LiDAR Map Compression for Efficient Localization on Resource Constrained Vehicles 

![image](https://github.com/ZJUYH/map_compression/blob/master/pics/system.png)

## Prepare to build

This project is compiled on `ros` and `Ubuntu`. 
And use `catkin_make` in your workspace to build this project.
Some dependencies are needed to be prepared:
* [pcl](http://pointclouds.org/) (relies heavily)
* [libpointmatcher](https://github.com/ethz-asl/libpointmatcher)
* [Gurobi](http://www.gurobi.com/) 

## Introduction to all folders

* `cfg`  
contains configuration files:  .rviz & filter.yaml, for visualization & configuration`

* `gurobi`  
integer linear programming code, written in Matlab

* `launch`  
roslaunch files to start-up ros nodes, C++

* `prepare`  
sparcify the dense poses, generate keep.txt

* `random_forest`  
train & test, learning part, copy from GitHub, thanks to [ranger](https://github.com/your/project/tags)

* `src`  
.cpp files using ros api & libpointmatcher

* `evaluation`  
prediction and localization evaluation matlab files

* `pics`  
some pictures of results long time ago

## The purpose of files in each folder for map compression

### prepare folder

* `prepare poses and scans`  
get the sparser poses for robot stopping cases, distance threshold used for more uniform of poses  
__input:__  origin poses or trajectories  
__output:__ uniform poses or trajectories

### map generation in src folder

* `scanRegister`  
use origin raw scans and popses to get the origin full map, filter raw scans to reduce large size  
__input:__　uniform poses and origin scans
__output:__	dense origin full map
  
* `mapFilter`
filter the dense origin full map using Octree Grid, and generate surface normals
__input:__ dense origin full map 
__output:__ origin full map 

* `mapScoring`
score each laer point in map according to the strategy in paper
__input:__ origin full map, laser scans and poses
__output:__ scored origin full map

* `mapFilter`
filter the dense origin full map using Octree Grid, and generate surface normals
__input:__
__output:__
   
