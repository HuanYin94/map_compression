
% icp030001_new = poseSparsification( icp030001, '/home/yh/mapModel/2018/08.03/keep_0301.txt', 0.20);
% getDistribution(icp030001);
% hold on;
% getDistribution(icp030001_new);

% [pose10_new, pose10_keep] = ForKITTIPoses( '/home/yh/mapModel/2018/08.02/kitti10_pose.txt', '/home/yh/mapModel/2018/08.02/kitti10_keep.txt', pose10, 0.70);
% 
% getDistribution(pose10_new);
% hold on;
% getDistribution(pose10_keep);


%% test

icp0901_new = poseSparsification( icp0901, '/home/yh/mapModel/2018/08.03/test/keep_0901.txt', 0.20);
getDistribution(icp0901);
hold on;
getDistribution(icp0901_new);