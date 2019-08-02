
% 
% load('pose_yq_b1000_ILP.txt');
% load('pose_yq_b1000_learn.txt');
% load('pose_yq_b1000_voxel.txt');
% load('pose_yq_b1000_cluster.txt');
% load('pose_yq_b1000_observe.txt');
% load('pose_yq_b1000_saliency.txt');
% 
% load('pose_yq_b2000_ILP.txt');
% load('pose_yq_b2000_learn.txt');
% load('pose_yq_b2000_voxel.txt');
% load('pose_yq_b2000_cluster.txt');
% load('pose_yq_b2000_observe.txt');
% load('pose_yq_b2000_saliency.txt');
% 
% load('pose_yq_b3000_ILP.txt');
% load('pose_yq_b3000_learn.txt');
% load('pose_yq_b3000_voxel.txt');
% load('pose_yq_b3000_cluster.txt');
% load('pose_yq_b3000_observe.txt');
% load('pose_yq_b3000_saliency.txt');
% 
% load('pose_yq_b4000_ILP.txt');
% load('pose_yq_b4000_learn.txt');
% load('pose_yq_b4000_voxel.txt');
% load('pose_yq_b4000_cluster.txt');
% load('pose_yq_b4000_observe.txt');
% load('pose_yq_b4000_saliency.txt');
% 
% 
% load('pose_yq_b1000_random_1.txt');
% load('pose_yq_b1000_random_2.txt');
% load('pose_yq_b1000_random_3.txt');
% load('pose_yq_b1000_random_4.txt');
% load('pose_yq_b1000_random_5.txt');
% 
% 
% load('pose_yq_b2000_random_1.txt');
% load('pose_yq_b2000_random_2.txt');
% load('pose_yq_b2000_random_3.txt');
% load('pose_yq_b2000_random_4.txt');
% load('pose_yq_b2000_random_5.txt');
% 
% 
% load('pose_yq_b3000_random_1.txt');
% load('pose_yq_b3000_random_2.txt');
% load('pose_yq_b3000_random_3.txt');
% load('pose_yq_b3000_random_4.txt');
% load('pose_yq_b3000_random_5.txt');
% 
% 
% load('pose_yq_b4000_random_1.txt');
% load('pose_yq_b4000_random_2.txt');
% load('pose_yq_b4000_random_3.txt');
% load('pose_yq_b4000_random_4.txt');
% load('pose_yq_b4000_random_5.txt');

%%

% 
% for i =1:length(pose_yq_b4000_ILP)
% 
%     rotError_4000_ILP(i,:) = getRotmError(pose_yq_b4000_ILP(i,:), yq(i,:));
%     trsError_4000_ILP(i,:) = getTransError(pose_yq_b4000_ILP(i,:), yq(i,:));
%     
%     rotError_4000_learn(i,:) = getRotmError(pose_yq_b4000_learn(i,:), yq(i,:));
%     trsError_4000_learn(i,:) = getTransError(pose_yq_b4000_learn(i,:), yq(i,:));
%     
%     rotError_4000_observe(i,:) = getRotmError(pose_yq_b4000_observe(i,:), yq(i,:));
%     trsError_4000_observe(i,:) = getTransError(pose_yq_b4000_observe(i,:), yq(i,:));
%     
%     rotError_4000_voxel(i,:) = getRotmError(pose_yq_b4000_voxel(i,:), yq(i,:));
%     trsError_4000_voxel(i,:) = getTransError(pose_yq_b4000_voxel(i,:), yq(i,:));
%    
%     
%     rotError_4000_random_1(i,:) = getRotmError(pose_yq_b4000_random_1(i,:), yq(i,:));
%     trsError_4000_random_1(i,:) = getTransError(pose_yq_b4000_random_1(i,:), yq(i,:)); 
%     
%     rotError_4000_random_2(i,:) = getRotmError(pose_yq_b4000_random_2(i,:), yq(i,:));
%     trsError_4000_random_2(i,:) = getTransError(pose_yq_b4000_random_2(i,:), yq(i,:));
%     
%     rotError_4000_random_3(i,:) = getRotmError(pose_yq_b4000_random_3(i,:), yq(i,:));
%     trsError_4000_random_3(i,:) = getTransError(pose_yq_b4000_random_3(i,:), yq(i,:));
%     
%     rotError_4000_random_4(i,:) = getRotmError(pose_yq_b4000_random_4(i,:), yq(i,:));
%     trsError_4000_random_4(i,:) = getTransError(pose_yq_b4000_random_4(i,:), yq(i,:));
%     
%     rotError_4000_random_5(i,:) = getRotmError(pose_yq_b4000_random_5(i,:), yq(i,:));
%     trsError_4000_random_5(i,:) = getTransError(pose_yq_b4000_random_5(i,:), yq(i,:));
% 
% end
% 
% 
% 
% for i =1:length(pose_yq_b3000_ILP)
% 
%     rotError_3000_ILP(i,:) = getRotmError(pose_yq_b3000_ILP(i,:), yq(i,:));
%     trsError_3000_ILP(i,:) = getTransError(pose_yq_b3000_ILP(i,:), yq(i,:));
%     
%     rotError_3000_learn(i,:) = getRotmError(pose_yq_b3000_learn(i,:), yq(i,:));
%     trsError_3000_learn(i,:) = getTransError(pose_yq_b3000_learn(i,:), yq(i,:));
%     
%     rotError_3000_voxel(i,:) = getRotmError(pose_yq_b3000_voxel(i,:), yq(i,:));
%     trsError_3000_voxel(i,:) = getTransError(pose_yq_b3000_voxel(i,:), yq(i,:));
%     
%     rotError_3000_observe(i,:) = getRotmError(pose_yq_b3000_observe(i,:), yq(i,:));
%     trsError_3000_observe(i,:) = getTransError(pose_yq_b3000_observe(i,:), yq(i,:));
%     
%     
%     
%     rotError_3000_random_1(i,:) = getRotmError(pose_yq_b3000_random_1(i,:), yq(i,:));
%     trsError_3000_random_1(i,:) = getTransError(pose_yq_b3000_random_1(i,:), yq(i,:)); 
%     
%     rotError_3000_random_2(i,:) = getRotmError(pose_yq_b3000_random_2(i,:), yq(i,:));
%     trsError_3000_random_2(i,:) = getTransError(pose_yq_b3000_random_2(i,:), yq(i,:));
%     
%     rotError_3000_random_3(i,:) = getRotmError(pose_yq_b3000_random_3(i,:), yq(i,:));
%     trsError_3000_random_3(i,:) = getTransError(pose_yq_b3000_random_3(i,:), yq(i,:));
%     
%     rotError_3000_random_4(i,:) = getRotmError(pose_yq_b3000_random_4(i,:), yq(i,:));
%     trsError_3000_random_4(i,:) = getTransError(pose_yq_b3000_random_4(i,:), yq(i,:));
%     
%     rotError_3000_random_5(i,:) = getRotmError(pose_yq_b3000_random_5(i,:), yq(i,:));
%     trsError_3000_random_5(i,:) = getTransError(pose_yq_b3000_random_5(i,:), yq(i,:));
% 
% end
% % 
% % 
% 
% for i =1:length(pose_yq_b2000_ILP)
% 
%     rotError_2000_ILP(i,:) = getRotmError(pose_yq_b2000_ILP(i,:), yq(i,:));
%     trsError_2000_ILP(i,:) = getTransError(pose_yq_b2000_ILP(i,:), yq(i,:));
%     
%     rotError_2000_learn(i,:) = getRotmError(pose_yq_b2000_learn(i,:), yq(i,:));
%     trsError_2000_learn(i,:) = getTransError(pose_yq_b2000_learn(i,:), yq(i,:));
%     
%     
%     rotError_2000_random_1(i,:) = getRotmError(pose_yq_b2000_random_1(i,:), yq(i,:));
%     trsError_2000_random_1(i,:) = getTransError(pose_yq_b2000_random_1(i,:), yq(i,:)); 
%     
%     rotError_2000_random_2(i,:) = getRotmError(pose_yq_b2000_random_2(i,:), yq(i,:));
%     trsError_2000_random_2(i,:) = getTransError(pose_yq_b2000_random_2(i,:), yq(i,:));
%     
%     rotError_2000_random_3(i,:) = getRotmError(pose_yq_b2000_random_3(i,:), yq(i,:));
%     trsError_2000_random_3(i,:) = getTransError(pose_yq_b2000_random_3(i,:), yq(i,:));
%     
%     rotError_2000_random_4(i,:) = getRotmError(pose_yq_b2000_random_4(i,:), yq(i,:));
%     trsError_2000_random_4(i,:) = getTransError(pose_yq_b2000_random_4(i,:), yq(i,:));
%     
%     rotError_2000_random_5(i,:) = getRotmError(pose_yq_b2000_random_5(i,:), yq(i,:));
%     trsError_2000_random_5(i,:) = getTransError(pose_yq_b2000_random_5(i,:), yq(i,:));
% 
% end
% 
% 
% for i =1:length(pose_yq_b1000_ILP)
% 
%     rotError_1000_ILP(i,:) = getRotmError(pose_yq_b1000_ILP(i,:), yq(i,:));
%     trsError_1000_ILP(i,:) = getTransError(pose_yq_b1000_ILP(i,:), yq(i,:));
%     
%     rotError_1000_learn(i,:) = getRotmError(pose_yq_b3000_learn(i,:), yq(i,:));
%     trsError_1000_learn(i,:) = getTransError(pose_yq_b3000_learn(i,:), yq(i,:));
% 
% end


% % % % 
% % % % % for i=1:length(pose_yq_b4000_saliency)
% % % % %     rotError_4000_saliency(i,:) = getRotmError(pose_yq_b4000_saliency(i,:), yq(i,:));
% % % % %     trsError_4000_saliency(i,:) = getTransError(pose_yq_b4000_saliency(i,:), yq(i,:));
% % % % % end
% % % % % 
% % % % % for i=1:length(pose_yq_b4000_cluster)
% % % % %      rotError_4000_cluster(i,:) = getRotmError(pose_yq_b4000_cluster(i,:), yq(i,:));
% % % % %     trsError_4000_cluster(i,:) = getTransError(pose_yq_b4000_cluster(i,:), yq(i,:));
% % % % % end
% % % % % 
% % % % % for i=1:length(pose_yq_b2000_voxel)
% % % % %      rotError_2000_voxel(i,:) = getRotmError(pose_yq_b2000_voxel(i,:), yq(i,:));
% % % % %     trsError_2000_voxel(i,:) = getTransError(pose_yq_b2000_voxel(i,:), yq(i,:));
% % % % % end



% 
% 
%%


% 
% rotError_1000_random = (rotError_1000_random_1 + rotError_1000_random_2 + rotError_1000_random_3 + rotError_1000_random_4 + rotError_1000_random_5)/5;
% rotError_2000_random = (rotError_2000_random_1 + rotError_2000_random_2 + rotError_2000_random_3 + rotError_2000_random_4 + rotError_2000_random_5)/5;
% rotError_3000_random = (rotError_3000_random_1 + rotError_3000_random_2 + rotError_3000_random_3 + rotError_3000_random_4 + rotError_3000_random_5)/5;
% rotError_4000_random = (rotError_4000_random_1 + rotError_4000_random_2 + rotError_4000_random_3 + rotError_4000_random_4 + rotError_4000_random_5)/5;
% 
% 
% 
% trsError_1000_random = (trsError_1000_random_1 + trsError_1000_random_2 + trsError_1000_random_3 + trsError_1000_random_4 + trsError_1000_random_5)/5;
% trsError_2000_random = (trsError_2000_random_1 + trsError_2000_random_2 + trsError_2000_random_3 + trsError_2000_random_4 + trsError_2000_random_5)/5;
% trsError_3000_random = (trsError_3000_random_1 + trsError_3000_random_2 + trsError_3000_random_3 + trsError_3000_random_4 + trsError_3000_random_5)/5;
% trsError_4000_random = (trsError_4000_random_1 + trsError_4000_random_2 + trsError_4000_random_3 + trsError_4000_random_4 + trsError_4000_random_5)/5;
% 

% 
% % 
% % 
% 
 %%
% 


figure
subplot(1,4,1)
h = boxplot([trsError_4000_ILP, trsError_4000_learn, trsError_4000_random, trsError_4000_voxel, 999*ones(2400,1), trsError_4000_observe,  999*ones(2400,1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Mc', 'Mt', 'Ms'}, 'OutlierSize', 8);
title('b=4000');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',18)  
set(gca,'ytick', [0:0.05:0.5])

subplot(1,4,2)
h = boxplot([trsError_3000_ILP, trsError_3000_learn, trsError_3000_random, trsError_3000_voxel, 999*ones(2400,1), trsError_3000_observe,  999*ones(2400,1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Mc', 'Mt', 'Ms'}, 'OutlierSize', 8);
title('b=3000');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',18)  
set(gca,'ytick', [0:0.05:0.5])


subplot(1,4,3)
h = boxplot([trsError_2000_ILP, trsError_2000_learn, trsError_2000_random, 999*ones(2400,1), 999*ones(2400,1), 999*ones(2400,1),  999*ones(2400,1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Mc', 'Mt', 'Ms'}, 'OutlierSize', 8);
title('b=2000');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',18)  
set(gca,'ytick', [0:0.05:0.5])


subplot(1,4,4)
h = boxplot([trsError_1000_ILP, trsError_1000_learn,  999*ones(2400,1), 999*ones(2400,1),  999*ones(2400,1), 999*ones(2400,1),  999*ones(2400,1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Mc', 'Mt', 'Ms'}, 'OutlierSize', 8);
title('b=1000');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',18)  
set(gca,'ytick', [0:0.05:0.5])








figure
subplot(1,4,1)
h = boxplot([rotError_4000_ILP, rotError_4000_learn, rotError_4000_random, rotError_4000_voxel, 999*ones(2400,1), rotError_4000_observe, 999*ones(2400,1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Mc', 'Mt', 'Ms'}, 'OutlierSize', 8);
title('b=4000');
ylim([0 1.0]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',18)  
set(gca,'ytick', [0:0.1:1.0]) 

subplot(1,4,2)
h = boxplot([rotError_3000_ILP, rotError_3000_learn, rotError_3000_random, rotError_3000_voxel, 999*ones(2400,1), rotError_3000_observe,  999*ones(2400,1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Mc', 'Mt', 'Ms'}, 'OutlierSize', 8);
title('b=3000');
ylim([0 1.0]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',18)  
set(gca,'ytick', [0:0.1:1.0]) 

subplot(1,4,3)
h = boxplot([rotError_2000_ILP,rotError_2000_learn, rotError_2000_random, 999*ones(2400,1) 999*ones(2400,1), 999*ones(2400,1),  999*ones(2400,1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Mc', 'Mt', 'Ms'}, 'OutlierSize', 8);
title('b=2000');
ylim([0 1.0]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',18)  
set(gca,'ytick', [0:0.1:1.0]) 


subplot(1,4,4)
h = boxplot([rotError_1000_ILP, rotError_1000_learn, 999*ones(2400,1), 999*ones(2400,1), 999*ones(2400,1), 999*ones(2400,1),  999*ones(2400,1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Mc', 'Mt', 'Ms'}, 'OutlierSize', 8);
title('b=1000');
ylim([0 1.0]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',18)  
set(gca,'ytick', [0:0.1:1.0]) 






