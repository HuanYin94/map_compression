## Welcome
Code of a manuscript, submitted to IEEE Transactions on Intelligent Transportation Systems, under 2nd review.

If you need the manuscript or have some questions on this project, please contact `zjuyinhuan@gmail.com` ([Huan Yin](https://yinhuan.site/)) .

# 3D LiDAR Map Compression for Efficient Localization on Resource Constrained Vehicles 

![image](https://github.com/ZJUYH/map_compression/blob/master/pics/system.png)

## Prepare to build

This project is compiled on `ros` and `Ubuntu`. 
And use `catkin_make` in your workspace to build this project.
Some dependencies are needed to be prepared:
* [pcl](http://pointclouds.org/)
* [libpointmatcher](https://github.com/ethz-asl/libpointmatcher)
* [Gurobi](http://www.gurobi.com/) 

## Introduction to all folders

* cfg  
`contains configuration files:  .rviz & filter.yaml, for visualization & configuration`

> + `gurobi`  
integer linear programming code, written in Matlab

> + `launch`  
roslaunch files to start-up ros nodes, C++

> + `prepare`  
sparcify the dense poses, generate keep.txt

> + `random_forest`  
train & test, learning part, copy from GitHub, thanks to [ranger](https://github.com/your/project/tags)

> + `src`  
.cpp files using ros api & libpointmatcher

> + `evaluation`  
prediction and localization evaluation matlab files

> + `pics`  
some pictures of results long time ago

## The purpose of files in each folder for map compression

### prepare folder

> + `prepare poses and scans`  
get the sparser poses for robot stopping cases, magic number used for uniform density of poses

### map_generation in src folder

> + `scanRegister.launch`  
aligning the raw scans (after the input filter) to generate the dnese LiDAR map, using ground truth poses and 

> + `mapFilter.launch`  
filter the map, downsapling and generate surface normals

> + `mapScoringIndex.launch`  
score the map, and then we have session-counts (score of each points in maps)

> + `genWeightVector.launch`  
generate weight vector q for programming, from the previous score step

> + `genVisMatrix.launch`  
generate visibility matrix that saved in many .txt, repeat the socring process actually (ugly but lazy to merge)

### learn_progm in src folder

> + `loadProResult.launch`  
load the programming results from gurobi_compress directory, generate *ground truth* saliency points, Mp

> + `mapCutterTraj.launch`  
cut the map to train & test parts for random forest, some maps are not needed

> + `generateAllFeatures.launch`  
generate the features and labels we want used for training

> + `loadFinalResults.launch`  
load the learned results from prediction.txt, Ml

> + `errorDistribution.launch`  
get the difference metrics between learned and programmed maps 

> + `mapCompresser.launch`  
compress the map by the description name, salient or salient_predicted?	

### loc_test in src folder

> + `locTest.launch`  
localization test using ICP rgistration, save poses and cost times

### comparisons in src folder

> + `clusterMap.launch`  
cluster segments according densities, Ms

> + `randomSampleMap.launch`  
random sample the origin full, Mr

> + `voxelMap.launch`  
voxel grid filter, Mv

### tools in src folder

> + `mapCheck.launch`  
See the map and trajecotory

> + `formatTransfer.launch`  
from vtk to ply for point clouds

### others

> + `Traing & testing with trees`  
use ranger C++ to build tree models

> + `tuning the probabilistic value of predictions`  
from 0~1 to 0 or 1

   
