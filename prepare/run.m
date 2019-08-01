
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
% 
% icp0301_new = poseSparsification( icp0301, '/home/yh/mapModel/2018/10.18/keep_0301.txt', 1.0);
% getDistribution(icp0301);
% hold on;
% getDistribution(icp0301_new);

%% Re-index for exp , 2019.07

% icp0301_new = poseSparsification_yq( '/home/yh/mapModel/2019/07.11/keep_0301.txt', '/home/yh/mapModel/2019/07.11/index_0301.txt', icp0301, 0.2);

% icp0301_new = poseSparsification_yq( '/home/yh/mapModel/2019/07.11/keep_0901.txt', '/home/yh/mapModel/2019/07.11/index_0901.txt', icp0901, 0.2);

% park_new = poseSparsification_yq( '/home/yh/mapModel/2019/07.23/keep_park.txt', '/home/yh/mapModel/2019/07.23/index_park.txt', parkpose, 0.2);

% kitti_new = poseSparsification_kitti( '/home/yh/mapModel/2019/07.21/keep_kitti.txt', '/home/yh/mapModel/2019/07.21/index_kitti.txt', Untitled, 0.5);


% kitti_new = poseSparsification_kitti( '/home/yh/mapModel/2019/07.27/keep_kitti.txt', '/home/yh/mapModel/2019/07.27/index_kitti.txt', X07, 0.5);








