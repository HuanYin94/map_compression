## Map compression in 3D LiDAR, for journal

# The order of leaned map compression:

## scanRegister:
   aligning the raw scans (after the input filter) to generate the dnese LiDAR map, using ground truth poses and 

## mapFilter: 
   filter the map, downsapling and generate some new features

## mapScoringIndex:
   score the map, we have session-counts

## genWeightVector:
   generate weight vector q for programming

## genVisMatrix:
   generate visibility matrix that saved in many .txt, repeat the socring process actually

## GO TO GUROBI TO OPTIMIZE 

## loadProResult
   load the programming results from gurobi_compress directory

## mapCutterTraj
   cut the map to train & tedt part for random forest

## generateAllFeatures
   generate the features and labels we want

## GO TO MATLAB TO BUILD FOREST 

## loadFinalResults
   load the learned results from matlab.txt

# Others:

## mapCheck
   See the map and trajecotory



