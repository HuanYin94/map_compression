```diff
- To be continued, for a jounal publication. - yh
```

## Map compression in 3D LiDAR, for journal

## DIRECTORIES:

### 1. cfg
   .rviz & filter.yaml, visualization & configuration
### 2. gurobi
   programming code, Matlab
### 3. launch
  launch ros nodes, C++
### 4. prepare
  sparcify the dnese poses, generate keep.txt, Matlab
### 5. random_forest
  train & test, learning part, copy from GitHub, thanks to [ranger](https://github.com/your/project/tags)
### 6. src
  .cpp files using ros api & [libpointmatcher](https://github.com/ethz-asl/libpointmatcher)

## The order of leaned map compression:

### 0. prepare poses
   get the sparser poses for robot stopping cases, magic number used for uniform density of poses

### 1. scanRegister
   aligning the raw scans (after the input filter) to generate the dnese LiDAR map, using ground truth poses and 

### 2. mapFilter
   filter the map, downsapling and generate surface normals

### 3. mapScoringIndex
   score the map, we have session-counts

### 4. genWeightVector
   generate weight vector q for programming

### 5. genVisMatrix
   generate visibility matrix that saved in many .txt, repeat the socring process actually (ugly but lazy to merge)

### 6. Optimize using Gurobi software API

### 7. loadProResult
   load the programming results from gurobi_compress directory, generate *ground truth* saliency points

### 8. mapCutterTraj
   cut the map to train & test parts for random forest

### 9. generateAllFeatures
   generate the features and labels we want used for training

### 10. Traing & testing with trees

### 11. loadFinalResults
   load the learned results from prediction.txt

## Other TOOLS:

### 1. mapCheck
   See the map and trajecotory



