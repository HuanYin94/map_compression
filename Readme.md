# 3D LiDAR Map Compression for Efficient Localization on Resource Constrained Vehicles 

## Introduction
Code of a manuscript, submitted to IEEE Transactions on Intelligent Transportation Systems. [video](https://youtu.be/sdETOor2Jck)

If have some questions on this project, please contact [Huan Yin](https://yinhuan.site/) `zjuyinhuan@gmail.com` 

	@article{yin20193d,
	  title={3D LiDAR Map Compression for Efficient Localization on Resource Constrained Vehicles},
	  author={Yin, Huan and Wang, Yue and Tang, Li and Ding, Xiaqing and Huang, Shoudong and Xiong, Rong},
	  journal={IEEE Transactions on Intelligent Transportation Systems},
	  status={submitted}
	}

## System
<img src="https://github.com/ZJUYH/map_compression/blob/master/images/system.png" width= 1000>

## Examples
After compression by RF, selected points are colored in red on the origin map  
<img src="https://github.com/ZJUYH/map_compression/blob/master/images/learn_local_2.png" width= 300>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="https://github.com/ZJUYH/map_compression/blob/master/images/learn_local_3.png" width= 300>  



## Prepare to build

This project is compiled on `ros` and `Ubuntu`. 
And use `catkin_make` in your workspace to build this project.
Some dependencies are needed to be prepared:
* [pcl](http://pointclouds.org/) 
* [libpointmatcher](https://github.com/ethz-asl/libpointmatcher)  (an old version in 2018.09)
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

* `images`  
some pictures of results long time ago

## Files in each folder for map compression

### prepare folder

* `prepare poses and scans`  
Maltab files, get the sparser poses for robot stopping cases, distance threshold used for more uniform of poses  
__input:__  origin poses or trajectories  
__output:__ uniform poses or trajectories  

### gurobi folder

* `before`   
previous try on programming, no use  

* `q_ILP_lamda`   
implementation for weighted ILP on map compression, including spliting and merging iteratively  

### random_forest folder

* `build`   
training and testing in bash files

### tools in src folder

* `formatTransfer`  
change the cloud files in a folder (.vtk to .ply for example)   
__input:__　the folder address  
__output:__ new format in this folder  

* `mapCheck`  
check the map in Rviz  
__input:__map and poses (trajectory)  

### map_generation in src folder

* `scanRegister`  
use origin raw scans and popses to get the origin full map, filter raw scans to reduce large size  
__input:__　uniform poses and origin scans  
__output:__	dense origin full map  
  
* `mapFilter`  
filter the dense origin full map using Octree Grid, and generate surface normals  
__input:__ dense origin full map  
__output:__ origin full map  

* `mapScoring`  
score each laser point in map according to the strategy in paper, which is also for observation filter as a comparison
__input:__ origin full map, laser scans and poses  
__output:__ scored origin full map  

* `genWeightVector`  
get the weight vector for integer linear programming  
__input:__ scored origin full map  
__output:__ all points' weight in a saved txt  

* `genVisMatrix`  
get the Visbility Matrix for integer linear programming  
__input:__ origin full map, laser scans and poses (same as `mapScoring`)  
__output:__ saved vis-files one by one in a folder  

### learn_program in src folder

* `mapCutterTraj`  
split/Cut the whole Map into two parts: train or test, according to the nearest search on trajectory  
__input:__　oringn full map, poses and cutPoition  
__output:__	train map, test map and labeled full map   
  
* `generateAllFeatures`  
generate point feature for training or testing  
__input:__　oringn full map, poses  
__output:__	map with features in descriptors and features in a saved txt file  
  
* `loadFinalResults`  
annotate the origin full map with the final learned result   
__input:__ origin full map and learned result in txt file  
__output:__	labeled learned result in cloud file  

* `loadProResult`  
annotate the origin full map with the supervised result by programming  
__input:__　origin file map and programmed results in files in a folder  
__output:__	labeled programmed result in cloud file  

* `mapCompresser`  
generate compressed map according to the annotated map on descriptor  
__input:__　annotated map by learning or programming, and the descriptor name  
__output:__	compressed map  

* `compareDistribution`  
get the nearest distances between two compressed clouds  
__input:__　two maps  
__output:__	distances in a saved txt file   
  
* `errorDistribution`  
get the nearest distances between salient & salient_predicted in cloud (learning & programming)  
__input:__　 one map with two descriptors (s & s_predicted)  
__output:__ distances in a saved txt file  

### loc_test in src folder

* `locTest`  
localization test by registration   
__input:__　 point cloud map, laser scans, trajectory(for position initial)   
__output:__ pose saved txt, time saved txt, icp iteration saved txt  

### comparisons in src folder

* `clusterMap`  
achieve map compression by clustering point clouds  
__input:__　 map, and some params   
__output:__ saved clustered map   

* `voxelMap`  
achieve map compression by voxel grid (similar as octree grid)  
__input:__　 map   
__output:__ saved sparse map  


* `randomSampleMap`  
achieve map compression by random selection  
__input:__　 map   
__output:__ saved random map   

* `selectByObserver`  
achieve map compression by thresholding the observation count  
__input:__　 map   
__output:__ saved threshold map   

* `splitCloudForSaliency`  
split map to sub-maps for geometry saliency detection  
__input:__　 map, and the trajectory   
__output:__ sub-maps in a folder   

* `saliencyMap`  
genrate saliency map on point clouds  
__input:__　 the folder contains sub-maps,    
__output:__ saliency evaluation saved in txt files ina folder   

* `mergeCloudForSaliency`  
merge the saliency and sub-maps into a whole map  
__input:__　 map, trajectory and saliency files  
__output:__ one global map with saliency evaluation   

## License 

[MIT](https://mit-license.org/)





