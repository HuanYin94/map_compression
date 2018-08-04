
meanDis09 = getMeanDistance_kitti(kitti09);
ForKITTIPoses( '/home/yh/mapModel/2018/08.02/kitti09_pose.txt', '/home/yh/mapModel/2018/08.02/kitti09_keep.txt', kitti09, meanDis09);

% meanDis0301 = getMeanDistance_yq(icp0301);
% poseSparsification( icp0301, meanDis0301, '/home/yh/mapModel/2018/08.03/keep_0301.txt');