
% meanDis00 = getMeanDistance_kitti(kitti00);
% ForKITTIPoses( '/home/yh/mapModel/2018/08.02/kitti00_pose.txt', '/home/yh/mapModel/2018/08.02/KITTI_prepare/kitti00_keep.txt', kitti00, meanDis00);

meanDis0301 = getMeanDistance_yq(icp0301);
poseSparsification( icp0301, meanDis0301, '/home/yh/mapModel/2018/08.03/keep_0301.txt');