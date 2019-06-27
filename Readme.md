```
- Code of a manuscript, submitted to IEEE Transactions on Intelligent Transportation Systems
- Written by [Huan Yin][https://yinhuan.site]
```

# 3D LiDAR Map Compression for Efficient Localization on Resource Constrained Vehicles 

![image](https://github.com/ZJUYH/map_compression/blob/master/pics/system.png)

## RELIES & BUILD


## Introduction to every folder

> + `cfg`  
.rviz & filter.yaml, visualization & configuration

> + `gurobi`  
programming code, Matlab

> + `launch`  
launch ros nodes, C++

> + `prepare`  
sparcify the dense poses, generate keep.txt, Matlab

> + `random_forest`  
train & test, learning part, copy from GitHub, thanks to [ranger](https://github.com/your/project/tags)

> + `src`  
.cpp files using ros api & [libpointmatcher](https://github.com/ethz-asl/libpointmatcher)

> + `evaluation`  
prediction and localization evaluation matlab files

> + `pics`  
some pictures of results

## The purpose of files for map compression



### The order of programming/learning based map compression:

> + `prepare poses and scans`  
get the sparser poses for robot stopping cases, magic number used for uniform density of poses

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

> + `Optimize using Gurobi software API`  
Matlab files, partitioning and optimization

> + `loadProResult.launch`  
load the programming results from gurobi_compress directory, generate *ground truth* saliency points, Mp

> + `mapCutterTraj.launch`  
cut the map to train & test parts for random forest, some maps are not needed

> + `generateAllFeatures.launch`  
generate the features and labels we want used for training

> + `Traing & testing with trees`  
use ranger C++ to build tree models

> + `tuning the probabilistic value of predictions`  
from 0~1 to 0 or 1

> + `loadFinalResults.launch`  
load the learned results from prediction.txt, Ml

> + `errorDistribution.launch`  
get the difference metrics between learned and programmed maps 

> + `mapCompresser.launch`  
compress the map by the description name, salient or salient_predicted?	

> + `locTest.launch`  
localization test using ICP rgistration, save poses and cost times

### Other maps

> + `clusterMap.launch`  
cluster segments according densities, Ms

> + `randomSampleMap.launch`  
random sample the origin full, Mr

> + `voxelMap.launch`  
voxel grid filter, Mv

### Other tools

> + `mapCheck.launch`  
See the map and trajecotory

> + `formatTransfer.launch`  
from vtk to ply for point clouds
   
