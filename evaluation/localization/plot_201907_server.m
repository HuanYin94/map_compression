

load('pose_kitti_b1000_ILP.txt');
load('pose_kitti_b1000_learn.txt');
load('pose_kitti_b1000_voxel.txt');
load('pose_kitti_b1000_cluster.txt');
load('pose_kitti_b1000_observe.txt');
load('pose_kitti_b1000_saliency.txt');


plot(pose_kitti_b1000_ILP(:,4), pose_kitti_b1000_ILP(:,8));
hold on;
plot(pose_kitti_b1000_learn(:,4), pose_kitti_b1000_learn(:,8));
hold on;
plot(pose_kitti_b1000_voxel(:,4), pose_kitti_b1000_voxel(:,8));
hold on;
% plot(pose_kitti_b1000_cluster(:,4), pose_kitti_b1000_cluster(:,8));
% hold on;
% plot(pose_kitti_b1000_observe(:,4), pose_kitti_b1000_observe(:,8));
% hold on;
% plot(pose_kitti_b1000_saliency(:,4), pose_kitti_b1000_saliency(:,8));
% hold on;

legend('ILP', 'learn', 'voxel', 'cluster', 'observe', 'saliency');

axis equal;




% 
% load('pose_kitti_b1000_random_1.txt');
% load('pose_kitti_b1000_random_2.txt');
% load('pose_kitti_b1000_random_3.txt');
% load('pose_kitti_b1000_random_4.txt');
% load('pose_kitti_b1000_random_5.txt');

% figure
% 
% plot(pose_kitti_b1000_random_1(:,4), pose_kitti_b1000_random_1(:,8));
% hold on;
% plot(pose_kitti_b1000_random_2(:,4), pose_kitti_b1000_random_2(:,8));
% hold on;
% plot(pose_kitti_b1000_random_3(:,4), pose_kitti_b1000_random_3(:,8));
% hold on;
% plot(pose_kitti_b1000_random_4(:,4), pose_kitti_b1000_random_4(:,8));
% hold on;
% plot(pose_kitti_b1000_random_5(:,4), pose_kitti_b1000_random_5(:,8));
% hold on;
% 
% axis equal;

