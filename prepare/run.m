
meanDis10 = getMeanDistance_kitti(kitti10);
ForKITTIPoses( '/home/yh/mapModel/2018/08.02/kitti10_pose.txt', '/home/yh/mapModel/2018/08.02/kitti10_keep.txt', kitti10, meanDis10);

% meanDis0301 = getMeanDistance_yq(icp0301);
% poseSparsification( icp0301, meanDis0301, '/home/yh/mapModel/2018/08.03/keep_0301.txt');