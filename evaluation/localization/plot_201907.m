

load('pose_kitti_b4000_ILP.txt');
load('pose_kitti_b3000_ILP.txt');
load('pose_kitti_b2000_ILP.txt');
load('pose_kitti_b1000_ILP.txt');

plot(pose_kitti_b4000_ILP(:,4), pose_kitti_b4000_ILP(:,8));
hold on;
plot(pose_kitti_b3000_ILP(:,4), pose_kitti_b3000_ILP(:,8));
hold on;
plot(pose_kitti_b2000_ILP(:,4), pose_kitti_b2000_ILP(:,8));
hold on;
plot(pose_kitti_b1000_ILP(:,4), pose_kitti_b1000_ILP(:,8));
hold on;
axis equal;
