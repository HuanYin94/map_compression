cmake_minimum_required(VERSION 2.8.3)
project(learned_map)

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

add_executable(mapCheck src/mapCheck.cpp)
target_link_libraries(mapCheck
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapCompresser src/mapCompresser.cpp)
target_link_libraries(mapCompresser
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(sessionFilter src/sessionFilter.cpp)
target_link_libraries(sessionFilter
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(loadFinalResults src/loadFinalResults.cpp)
target_link_libraries(loadFinalResults
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(randomSampleMap src/randomSampleMap.cpp)
target_link_libraries(randomSampleMap
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(saveAllFeatures src/saveAllFeatures.cpp)
target_link_libraries(saveAllFeatures
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapCutterTraj src/mapCutterTraj.cpp)
target_link_libraries(mapCutterTraj
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(generateOtherFeatures src/generateOtherFeatures.cpp)
target_link_libraries(generateOtherFeatures
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(errorDistribution src/errorDistribution.cpp)
target_link_libraries(errorDistribution
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(DBSCANFilter src/DBSCANFilter.cpp)
target_link_libraries(DBSCANFilter
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(locTest src/locTest.cpp)
target_link_libraries(locTest
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapCompare src/mapCompare.cpp)
target_link_libraries(mapCompare
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapSummer src/mapSummer.cpp)
target_link_libraries(mapSummer
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapScoringIndex src/mapScoringIndex.cpp)
target_link_libraries(mapScoringIndex
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(matchOut src/matchOut.cpp)
target_link_libraries(matchOut
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(test_gurobi src/test_gurobi.cpp)
target_link_libraries(test_gurobi
${catkin_LIBRARIES}
)


