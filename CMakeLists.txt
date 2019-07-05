cmake_minimum_required(VERSION 2.8.3)
project(map_compression)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  cmake_modules
  tf
  tf_conversions
  libpointmatcher_ros
  pcl_ros
)

find_package(libpointmatcher REQUIRED)

add_definitions(-std=c++11 -O2 -g -DNDEBUG)

catkin_package (
#	CATKIN_DEPENDS roscpp
)

include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${EIGEN_INCLUDE_DIRS}
  ${libpointmatcher_INCLUDE_DIRS}
)

add_executable(mapCheck src/tools/mapCheck.cpp)
target_link_libraries(mapCheck
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(formatTransfer src/tools/formatTransfer.cpp)
target_link_libraries(formatTransfer
${catkin_LIBRARIES}
)

add_executable(mapCompresser src/learn_program/mapCompresser.cpp)
target_link_libraries(mapCompresser
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(loadFinalResults src/learn_program/loadFinalResults.cpp)
target_link_libraries(loadFinalResults
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(randomSampleMap src/comparisons/randomSampleMap.cpp)
target_link_libraries(randomSampleMap
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapCutterTraj src/learn_program/mapCutterTraj.cpp)
target_link_libraries(mapCutterTraj
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(generateAllFeatures src/learn_program/generateAllFeatures.cpp)
target_link_libraries(generateAllFeatures
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(errorDistribution src/learn_program/errorDistribution.cpp)
target_link_libraries(errorDistribution
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(compareDistribution src/learn_program/compareDistribution.cpp)
target_link_libraries(compareDistribution
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)


#add_executable(locTest src/loc_test/locTest.cpp)
#target_link_libraries(locTest
#${catkin_LIBRARIES}
#${libpointmatcher_LIBRARIES}
#)

add_executable(mapScoringIndex src/map_generation/mapScoringIndex.cpp)
target_link_libraries(mapScoringIndex
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(genVisMatrix src/map_generation/genVisMatrix.cpp)
target_link_libraries(genVisMatrix
${catkin_LIBRARIES}
)

add_executable(loadProResult src/learn_program/loadProResult.cpp)
target_link_libraries(loadProResult
${catkin_LIBRARIES}
)

add_executable(scanRegister src/map_generation/scanRegister.cpp)
target_link_libraries(scanRegister
${catkin_LIBRARIES}
)

add_executable(genWeightVector src/map_generation/genWeightVector.cpp)
target_link_libraries(genWeightVector
${catkin_LIBRARIES}
)

add_executable(mapFilter src/map_generation/mapFilter.cpp)
target_link_libraries(mapFilter
${catkin_LIBRARIES}
)

add_executable(clusterMap src/comparisons/clusterMap.cpp)
target_link_libraries(clusterMap
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(voxelMap src/comparisons/voxelMap.cpp)
target_link_libraries(voxelMap
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(splitCloudForSaliency src/comparisons/splitCloudForSaliency.cpp)
target_link_libraries(splitCloudForSaliency
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(saliencyMap src/comparisons/saliencyMap.cpp)
target_link_libraries(saliencyMap
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mergeCloudForSaliency src/comparisons/mergeCloudForSaliency.cpp)
target_link_libraries(mergeCloudForSaliency
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)


