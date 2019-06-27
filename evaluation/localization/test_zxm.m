% load('/media/yh/YH/zxm/pose_0.2.txt');
% load('/media/yh/YH/zxm/pose_0.15.txt');
% load('/media/yh/YH/zxm/pose_0.10.txt');
% load('/media/yh/YH/zxm/pose_0.05.txt');

% plot(pose_0_10(:,4), pose_0_10(:,8))
% 
% 
% for i =1:length(pose_0_2) % segments limit
%  
%     rotError_2(i,:) = getRotmError(pose_0_2(i,:), locTest_gt(i,:));
%     trsError_2(i,:) = getTransError(pose_0_2(i,:), locTest_gt(i,:));
%     
%     rotError_15(i,:) = getRotmError(pose_0_15(i,:), locTest_gt(i,:));
%     trsError_15(i,:) = getTransError(pose_0_15(i,:), locTest_gt(i,:));
% 
% end

figure
h = boxplot([trsError_2, trsError_15, 999*ones(length(pose_0_2),1), 999*ones(length(pose_0_2),1)], 'Labels',{'0.20','0.15','0.10','0.05'}, 'OutlierSize', 4);
title('trans error');
ylim([0 0.5]);
set(gca,'FontSize',10)  
set(gca,'ytick', [0:0.05:0.5])

figure
h = boxplot([rotError_2, rotError_15, 999*ones(length(rotError_2),1),  999*ones(length(rotError_2),1)], 'Labels',{'0.20','0.15','0.10','0.05'}, 'OutlierSize', 4);
title('rot error');
ylim([0 1.0]);
set(gca,'FontSize',10)  
set(gca,'ytick', [0:0.1:1.0]) 

