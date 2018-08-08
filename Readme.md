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
  train & test, learning part, copy from GitHub, thanks to [ranger]((https://github.com/your/project/tags))
### 6. src
  .cpp files using ros api & [libpointmatcher](https://github.com/ethz-asl/libpointmatcher)

## The order of leaned map compression:

### 0. prepare poses
   get the sparser poses for robot stopping cases

### 1. scanRegister
   aligning the raw scans (after the input filter) to generate the dnese LiDAR map, using ground truth poses and 

### 2. mapFilter
   filter the map, downsapling and generate some new features

### 3. mapScoringIndex
   score the map, we have session-counts

### 4. genWeightVector
   generate weight vector q for programming

### 5. genVisMatrix
   generate visibility matrix that saved in many .txt, repeat the socring process actually

### 6. Optimize Using Gurobi Software API

### 7. loadProResult
   load the programming results from gurobi_compress directory

### 8. mapCutterTraj
   cut the map to train & tedt part for random forest

### 9. generateAllFeatures
   generate the features and labels we want

### 10. GO TO MATLAB TO BUILD FOREST 

### 11. loadFinalResults
   load the learned results from matlab.txt

## Other TOOLS:

### 1. mapCheck
   See the map and trajecotory



