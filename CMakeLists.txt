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

add_executable(mapAccuLaser src/mapAccuLaser.cpp)
target_link_libraries(mapAccuLaser
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapCutter src/mapCutter.cpp)
target_link_libraries(mapCutter
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapFeater src/mapFeater.cpp)
target_link_libraries(mapFeater
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapAccuLaserMultiple src/mapAccuLaserMultiple.cpp)
target_link_libraries(mapAccuLaserMultiple
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapSessioner src/mapSessioner.cpp)
target_link_libraries(mapSessioner
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(mapAccuLaserRange src/mapAccuLaserRange.cpp)
target_link_libraries(mapAccuLaserRange
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(labelLaser src/labelLaser.cpp)
target_link_libraries(labelLaser
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(sessionFilter src/sessionFilter.cpp)
target_link_libraries(sessionFilter
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(generateSlidingSamples src/generateSlidingSamples.cpp)
target_link_libraries(generateSlidingSamples
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(generateCRFFeatures src/generateCRFFeatures.cpp)
target_link_libraries(generateCRFFeatures
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(savePointNetFeatures src/savePointNetFeatures.cpp)
target_link_libraries(savePointNetFeatures
${catkin_LIBRARIES}
${libpointmatcher_LIBRARIES}
)

add_executable(savePointNetBinsFeatures src/savePointNetBinsFeatures.cpp)
target_link_libraries(savePointNetBinsFeatures
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

