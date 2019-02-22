% 
% 
% 
% for i =1:length(clusterb1500pose) 
% 
%     rotError_cluster_2000(i,:) = getRotmError(clusterb2000pose(i,:), locTest_gt(i,:));
%     trsError_cluster_2000(i,:) =  getTransError(clusterb2000pose(i,:), locTest_gt(i,:));
%     
%     rotError_cluster_1500(i,:) = getRotmError(clusterb1500pose(i,:), locTest_gt(i,:));
%     trsError_cluster_1500(i,:) =  getTransError(clusterb1500pose(i,:), locTest_gt(i,:));
% end
% 
% for i =1:length(Learnedb1500pose) 
% 
%     rotError_learned_2000(i,:) = getRotmError(Learnedb2000pose(i,:), locTest_gt(i,:));
%     trsError_learned_2000(i,:) =  getTransError(Learnedb2000pose(i,:), locTest_gt(i,:));
%      
%     rotError_learned_1500(i,:) = getRotmError(Learnedb1500pose(i,:), locTest_gt(i,:));
%     trsError_learned_1500(i,:) =  getTransError(Learnedb1500pose(i,:), locTest_gt(i,:));
% 
%     rotError_learned_1000(i,:) = getRotmError(Learnedb1000pose(i,:), locTest_gt(i,:));
%     trsError_learned_1000(i,:) =  getTransError(Learnedb1000pose(i,:), locTest_gt(i,:));
%     
%     rotError_learned_500(i,:) = getRotmError(Learnedb500pose(i,:), locTest_gt(i,:));
%     trsError_learned_500(i,:) =  getTransError(Learnedb500pose(i,:), locTest_gt(i,:));
%     
% end
% 
% for i =1:length(Progb1500pose) 
% 
%     rotError_ILP_2000(i,:) = getRotmError(Progb2000pose(i,:), locTest_gt(i,:));
%     trsError_ILP_2000(i,:) =  getTransError(Progb2000pose(i,:), locTest_gt(i,:));
%     
%     rotError_ILP_1500(i,:) = getRotmError(Progb1500pose(i,:), locTest_gt(i,:));
%     trsError_ILP_1500(i,:) =  getTransError(Progb1500pose(i,:), locTest_gt(i,:));
% 
%     rotError_ILP_1000(i,:) = getRotmError(Progb1000pose(i,:), locTest_gt(i,:));
%     trsError_ILP_1000(i,:) =  getTransError(Progb1000pose(i,:), locTest_gt(i,:));
%     
%     rotError_ILP_500(i,:) = getRotmError(Progb500pose(i,:), locTest_gt(i,:));
%     trsError_ILP_500(i,:) =  getTransError(Progb500pose(i,:), locTest_gt(i,:));
% end
% 
% for i =1:length(randomb1500pose) 
%     
%     rotError_random_2000(i,:) = getRotmError(randomb2000pose(i,:), locTest_gt(i,:));
%     trsError_random_2000(i,:) =  getTransError(randomb2000pose(i,:), locTest_gt(i,:));
%     
%     rotError_random_1500(i,:) = getRotmError(randomb1500pose(i,:), locTest_gt(i,:));
%     trsError_random_1500(i,:) =  getTransError(randomb1500pose(i,:), locTest_gt(i,:));
% 
%     rotError_random_1000(i,:) = getRotmError(randomb1000pose(i,:), locTest_gt(i,:));
%     trsError_random_1000(i,:) =  getTransError(randomb1000pose(i,:), locTest_gt(i,:));
%     
%     rotError_random_500(i,:) = getRotmError(randomb500pose(i,:), locTest_gt(i,:));
%     trsError_random_500(i,:) =  getTransError(randomb500pose(i,:), locTest_gt(i,:));
% end
% 
% for i =1:length(voxelb1500pose) 
%     
%     rotError_voxel_2000(i,:) = getRotmError(voxelb2000pose(i,:), locTest_gt(i,:));
%     trsError_voxel_2000(i,:) =  getTransError(voxelb2000pose(i,:), locTest_gt(i,:));
%     
%     rotError_voxel_1500(i,:) = getRotmError(voxelb1500pose(i,:), locTest_gt(i,:));
%     trsError_voxel_1500(i,:) =  getTransError(voxelb1500pose(i,:), locTest_gt(i,:));
% 
%     rotError_voxel_1000(i,:) = getRotmError(voxelb1000pose(i,:), locTest_gt(i,:));
%     trsError_voxel_1000(i,:) =  getTransError(voxelb1000pose(i,:), locTest_gt(i,:));
%     
%     rotError_voxel_500(i,:) = getRotmError(voxelb500pose(i,:), locTest_gt(i,:));
%     trsError_voxel_500(i,:) =  getTransError(voxelb500pose(i,:), locTest_gt(i,:));
% end
% 


%%




figure

subplot(1,4,1)
h = boxplot([trsError_ILP_2000, trsError_learned_2000, trsError_voxel_2000, trsError_random_2000, trsError_cluster_2000], 'Labels',{'Mp','Ml','Mv','Mr', 'Ms'}, 'OutlierSize', 8);
title('b=2000');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',25)  
set(gca,'ytick', [0:0.05:0.5])

subplot(1,4,2)
h = boxplot([trsError_ILP_1500, trsError_learned_1500, trsError_voxel_1500, trsError_random_1500, 999*ones(length(trsError_random_1500),1)], 'Labels',{'Mp','Ml','Mv','Mr', 'Ms'}, 'OutlierSize', 8);
title('b=1500');
ylim([0 0.5]);
set(gca,'FontSize',25)  
set(gca,'ytick', [0:0.05:0.5])



subplot(1,4,3)
h = boxplot([trsError_ILP_1000, trsError_learned_1000, trsError_voxel_1000, trsError_random_1000, 999*ones(length(trsError_ILP_500),1)], 'Labels',{'Mp','Ml','Mv','Mr', 'Ms'}, 'OutlierSize', 8);
title('b=1000');
ylim([0 0.5]);
set(gca,'FontSize',25)  
set(gca,'ytick', [0:0.05:0.5])


subplot(1,4,4)
h = boxplot([trsError_ILP_500, trsError_learned_500, 999*ones(length(rotError_ILP_500),1), 999*ones(length(trsError_ILP_500),1), 999*ones(length(trsError_ILP_500),1)], 'Labels',{'Mp','Ml','Mv','Mr', 'Ms'}, 'OutlierSize', 8);
title('b=500');
ylim([0 0.5]);
set(gca,'FontSize',25)  
set(gca,'ytick', [0:0.05:0.5])









figure
subplot(1,4,1)
h = boxplot([rotError_ILP_2000, rotError_learned_2000, rotError_voxel_2000, rotError_random_2000, trsError_cluster_2000], 'Labels',{'Mp','Ml','Mv','Mr', 'Ms'}, 'OutlierSize', 8);
title('b=2000');
ylim([0 1.0]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 30);
set(gca,'FontSize',25)  
set(gca,'ytick', [0:0.1:1.0]) 


subplot(1,4,2)
h = boxplot([rotError_ILP_1500, rotError_learned_1500, rotError_voxel_1500, rotError_random_1500, 999*ones(length(rotError_random_1500),1)], 'Labels',{'Mp','Ml','Mv','Mr', 'Ms'}, 'OutlierSize', 8);
title('b=1500');
ylim([0 1.0]);
set(gca,'FontSize',25)  
set(gca,'ytick', [0:0.1:1.0]) 




subplot(1,4,3)
h = boxplot([rotError_ILP_1000, rotError_learned_1000, rotError_voxel_1000, rotError_random_1000, 999*ones(length(rotError_random_1000),1)], 'Labels',{'Mp','Ml','Mv','Mr', 'Ms'}, 'OutlierSize', 8);
title('b=1000');
ylim([0 1.0]);
set(gca,'FontSize',25)  
set(gca,'ytick', [0:0.1:1.0]) 



subplot(1,4,4)
h = boxplot([rotError_ILP_500, rotError_learned_500, 999*ones(length(rotError_ILP_500),1),  999*ones(length(rotError_ILP_500),1), 999*ones(length(rotError_ILP_500),1)], 'Labels',{'Mp','Ml','Mv','Mr', 'Ms'}, 'OutlierSize', 8);
title('b=500');
ylim([0 1.0]);
set(gca,'FontSize',25)  
set(gca,'ytick', [0:0.1:1.0]) 










