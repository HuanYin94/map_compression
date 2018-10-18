
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

% icp0901_new = poseSparsification( icp0901, '/home/yh/mapModel/2018/08.03/test/keep_0901.txt', 0.20);
% getDistribution(icp0901);
% hold on;
% getDistribution(icp0901_new);

%% Chery test'

% chery_pose = poseSparsification( globalpose, '/home/yh/mapModel/2018/09.22/keep_chery.txt', 0.20);
% getDistribution(globalpose);
% hold on;
% getDistribution(chery_pose);

%% New YQ data pepare, the former are too dense\

icp0301_new = poseSparsification( icp0301, '/home/yh/mapModel/2018/10.18/keep_0301.txt', 1.0);
getDistribution(icp0301);
hold on;
getDistribution(icp0301_new);



