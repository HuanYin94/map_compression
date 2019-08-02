% load('pose_yq_b1000_0.05.txt');
% load('pose_yq_b1000_0.1.txt');
% load('pose_yq_b1000_0.2.txt');
% load('pose_yq_b1000_0.3.txt');
% load('pose_yq_b1000_0.4.txt');
% 
% for i =1:1000
% 
%      rotError_1000_0_05(i,:) = getRotmError(pose_yq_b1000_0_05(i,:), yq(i,:));
%      trsError_1000_0_05(i,:) = getTransError(pose_yq_b1000_0_05(i,:), yq(i,:));
%      
%       rotError_1000_0_1(i,:) = getRotmError(pose_yq_b1000_0_1(i,:), yq(i,:));
%      trsError_1000_0_1(i,:) = getTransError(pose_yq_b1000_0_1(i,:), yq(i,:));
%    
%       rotError_1000_0_2(i,:) = getRotmError(pose_yq_b1000_0_2(i,:), yq(i,:));
%      trsError_1000_0_2(i,:) = getTransError(pose_yq_b1000_0_2(i,:), yq(i,:));
% 
%       rotError_1000_0_3(i,:) = getRotmError(pose_yq_b1000_0_3(i,:), yq(i,:));
%      trsError_1000_0_3(i,:) = getTransError(pose_yq_b1000_0_3(i,:), yq(i,:));
%      
% end
% 
% for i=1:500
%     
%           rotError_1000_0_4(i,:) = getRotmError(pose_yq_b1000_0_4(i,:), yq(i,:));
%      trsError_1000_0_4(i,:) = getTransError(pose_yq_b1000_0_4(i,:), yq(i,:));
%     
% end



mean(trsError_1000_0_05)
mean(trsError_1000_0_1)
mean(trsError_1000_0_2)
% mean(trsError_1000_0_3)
% mean(trsError_1000_0_4)


% mean(rotError_1000_0_05)
% mean(rotError_1000_0_1)
% mean(rotError_1000_0_2)
% mean(rotError_1000_0_3)
% mean(rotError_1000_0_4)
% 




