

load('pose_kitti_b1000_ILP.txt');
load('pose_kitti_b1000_learn.txt');
load('pose_kitti_b1000_random.txt');
load('pose_kitti_b1000_voxel.txt');
load('pose_kitti_b1000_cluster.txt');
load('pose_kitti_b1000_observe.txt');
load('pose_kitti_b1000_saliency.txt');


plot(pose_kitti_b1000_ILP(:,4), pose_kitti_b1000_ILP(:,8));
hold on;
plot(pose_kitti_b1000_learn(:,4), pose_kitti_b1000_learn(:,8));
hold on;
plot(pose_kitti_b1000_random(:,4), pose_kitti_b1000_random(:,8));
hold on;
plot(pose_kitti_b1000_voxel(:,4), pose_kitti_b1000_voxel(:,8));
hold on;
plot(pose_kitti_b1000_cluster(:,4), pose_kitti_b1000_cluster(:,8));
hold on;
plot(pose_kitti_b1000_observe(:,4), pose_kitti_b1000_observe(:,8));
hold on;
plot(pose_kitti_b1000_saliency(:,4), pose_kitti_b1000_saliency(:,8));
hold on;

legend('ILP', 'learn', 'random', 'voxel', 'cluster', 'observe', 'saliency');

axis equal;
