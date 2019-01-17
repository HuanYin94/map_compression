% % all except b = 500 for ILP & learn

%%  averaging error of random

% %% get all the errors

for i =1:length(locTest_gt)
 
    rotError_ILP_2000(i,:) = getRotmError(yq_ILP_b2000_pose(i,:), locTest_gt(i,:));
    trsError_ILP_2000(i,:) =  getTransError(yq_ILP_b2000_pose(i,:), locTest_gt(i,:));
    
    rotError_predict_2000(i,:) = getRotmError(yq_predict_b2000_pose(i,:), locTest_gt(i,:));
    trsError_predict_2000(i,:) =  getTransError(yq_predict_b2000_pose(i,:), locTest_gt(i,:));
    
    rotError_voxel_2000(i,:) = getRotmError(yq_voxel_b2000_pose(i,:), locTest_gt(i,:));
    trsError_voxel_2000(i,:) =  getTransError(yq_voxel_b2000_pose(i,:), locTest_gt(i,:));
    
    rotError_cluster_2000(i,:) = getRotmError(yq_cluster_b2000_pose(i,:), locTest_gt(i,:));
    trsError_cluster_2000(i,:) =  getTransError(yq_cluster_b2000_pose(i,:), locTest_gt(i,:));
    
    
    
    rotError_random_2000_1(i,:) = getRotmError(yq_random_b2000_pose_1(i,:), locTest_gt(i,:));
    trsError_random_2000_1(i,:) =  getTransError(yq_random_b2000_pose_1(i,:), locTest_gt(i,:));
    
    rotError_random_2000_2(i,:) = getRotmError(yq_random_b2000_pose_2(i,:), locTest_gt(i,:));
    trsError_random_2000_2(i,:) =  getTransError(yq_random_b2000_pose_2(i,:), locTest_gt(i,:));
    
    rotError_random_2000_3(i,:) = getRotmError(yq_random_b2000_pose_3(i,:), locTest_gt(i,:));
    trsError_random_2000_3(i,:) =  getTransError(yq_random_b2000_pose_3(i,:), locTest_gt(i,:));
    
    rotError_random_2000_4(i,:) = getRotmError(yq_random_b2000_pose_4(i,:), locTest_gt(i,:));
    trsError_random_2000_4(i,:) =  getTransError(yq_random_b2000_pose_4(i,:), locTest_gt(i,:));
    
    rotError_random_2000_5(i,:) = getRotmError(yq_random_b2000_pose_5(i,:), locTest_gt(i,:));
    trsError_random_2000_5(i,:) =  getTransError(yq_random_b2000_pose_5(i,:), locTest_gt(i,:));
    
end




for i =1:length(locTest_gt)
 
    rotError_ILP_1500(i,:) = getRotmError(yq_ILP_b1500_pose(i,:), locTest_gt(i,:));
    trsError_ILP_1500(i,:) =  getTransError(yq_ILP_b1500_pose(i,:), locTest_gt(i,:));

    rotError_predict_1500(i,:) = getRotmError(yq_predict_b2000_pose(i,:), locTest_gt(i,:));
    trsError_predict_1500(i,:) =  getTransError(yq_predict_b2000_pose(i,:), locTest_gt(i,:));
 
    rotError_voxel_1500(i,:) = getRotmError(yq_voxel_b1500_pose(i,:), locTest_gt(i,:));
    trsError_voxel_1500(i,:) =  getTransError(yq_voxel_b1500_pose(i,:), locTest_gt(i,:));
    
    rotError_random_1500_1(i,:) = getRotmError(yq_random_b1500_pose_1(i,:), locTest_gt(i,:));
    trsError_random_1500_1(i,:) =  getTransError(yq_random_b1500_pose_1(i,:), locTest_gt(i,:));
    
    rotError_random_1500_2(i,:) = getRotmError(yq_random_b1500_pose_2(i,:), locTest_gt(i,:));
    trsError_random_1500_2(i,:) =  getTransError(yq_random_b1500_pose_2(i,:), locTest_gt(i,:));
    
    rotError_random_1500_3(i,:) = getRotmError(yq_random_b1500_pose_3(i,:), locTest_gt(i,:));
    trsError_random_1500_3(i,:) =  getTransError(yq_random_b1500_pose_3(i,:), locTest_gt(i,:));
    
    rotError_random_1500_4(i,:) = getRotmError(yq_random_b1500_pose_4(i,:), locTest_gt(i,:));
    trsError_random_1500_4(i,:) =  getTransError(yq_random_b1500_pose_4(i,:), locTest_gt(i,:));
    
    rotError_random_1500_5(i,:) = getRotmError(yq_random_b1500_pose_5(i,:), locTest_gt(i,:));
    trsError_random_1500_5(i,:) =  getTransError(yq_random_b1500_pose_5(i,:), locTest_gt(i,:));
    
end





for i =1:length(locTest_gt)
 
    rotError_ILP_1000(i,:) = getRotmError(yq_ILP_b1000_pose(i,:), locTest_gt(i,:));
    trsError_ILP_1000(i,:) =  getTransError(yq_ILP_b1000_pose(i,:), locTest_gt(i,:));
    

    rotError_predict_1000(i,:) = getRotmError(yq_predict_b1000_pose(i,:), locTest_gt(i,:));
    trsError_predict_1000(i,:) =  getTransError(yq_predict_b1000_pose(i,:), locTest_gt(i,:));
       
    rotError_voxel_1000(i,:) = getRotmError(yq_voxel_b1000_pose(i,:), locTest_gt(i,:));
    trsError_voxel_1000(i,:) =  getTransError(yq_voxel_b1000_pose(i,:), locTest_gt(i,:));
    
    rotError_random_1000_1(i,:) = getRotmError(yq_random_b1000_pose_1(i,:), locTest_gt(i,:));
    trsError_random_1000_1(i,:) =  getTransError(yq_random_b1000_pose_1(i,:), locTest_gt(i,:));
    
    rotError_random_1000_2(i,:) = getRotmError(yq_random_b1000_pose_2(i,:), locTest_gt(i,:));
    trsError_random_1000_2(i,:) =  getTransError(yq_random_b1000_pose_2(i,:), locTest_gt(i,:));
    
    rotError_random_1000_3(i,:) = getRotmError(yq_random_b1000_pose_3(i,:), locTest_gt(i,:));
    trsError_random_1000_3(i,:) =  getTransError(yq_random_b1000_pose_3(i,:), locTest_gt(i,:));
    
    rotError_random_1000_4(i,:) = getRotmError(yq_random_b1000_pose_4(i,:), locTest_gt(i,:));
    trsError_random_1000_4(i,:) =  getTransError(yq_random_b1000_pose_4(i,:), locTest_gt(i,:));
    
    rotError_random_1000_5(i,:) = getRotmError(yq_random_b1000_pose_5(i,:), locTest_gt(i,:));
    trsError_random_1000_5(i,:) =  getTransError(yq_random_b1000_pose_5(i,:), locTest_gt(i,:));
    
end




for i =1:length(locTest_gt)
 
    rotError_ILP_500(i,:) = getRotmError(yq_ILP_b500_pose(i,:), locTest_gt(i,:));
    trsError_ILP_500(i,:) =  getTransError(yq_ILP_b500_pose(i,:), locTest_gt(i,:));

    rotError_predict_500(i,:) = getRotmError(yq_predict_b500_pose(i,:), locTest_gt(i,:));
    trsError_predict_500(i,:) =  getTransError(yq_predict_b500_pose(i,:), locTest_gt(i,:));  
    
    rotError_voxel_500(i,:) = getRotmError(yq_voxel_b500_pose(i,:), locTest_gt(i,:));
    trsError_voxel_500(i,:) =  getTransError(yq_voxel_b500_pose(i,:), locTest_gt(i,:));
    
end

%% get the mean of errors

trsError_random_1000 = mean([trsError_random_1000_1, trsError_random_1000_2, trsError_random_1000_3, trsError_random_1000_4, trsError_random_1000_5], 2);
trsError_random_1500 = mean([trsError_random_1500_1, trsError_random_1500_2, trsError_random_1500_3, trsError_random_1500_4, trsError_random_1500_5], 2);
trsError_random_2000 = mean([trsError_random_2000_1, trsError_random_2000_2, trsError_random_2000_3, trsError_random_2000_4, trsError_random_2000_5], 2);

rotError_random_1000 = mean([rotError_random_1000_1, rotError_random_1000_2, rotError_random_1000_3, rotError_random_1000_4, rotError_random_1000_5], 2);
rotError_random_1500 = mean([rotError_random_1500_1, rotError_random_1500_2, rotError_random_1500_3, rotError_random_1500_4, rotError_random_1500_5], 2);
rotError_random_2000 = mean([rotError_random_2000_1, rotError_random_2000_2, rotError_random_2000_3, rotError_random_2000_4, rotError_random_2000_5], 2);

%%  averaging pose of random, only random calc. I was wrong before.

%% draw pictures

figure
subplot(1,4,1)
h = boxplot([trsError_ILP_2000, trsError_predict_2000, trsError_random_2000, trsError_voxel_2000, trsError_cluster_2000], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('b=2000');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.05:0.5])


subplot(1,4,2)
h = boxplot([trsError_ILP_1500, trsError_predict_1500, trsError_random_1500, trsError_voxel_1500, 999*ones(length(trsError_random_1500),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('b=1500');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.05:0.5])



subplot(1,4,3)
h = boxplot([trsError_ILP_1000, trsError_predict_1000, trsError_random_1000, trsError_voxel_1000, 999*ones(length(trsError_random_1000),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('b=1000');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.05:0.5])


subplot(1,4,4)
h = boxplot([trsError_ILP_500, trsError_predict_500, trsError_voxel_500, 999*ones(length(trsError_random_1000),1), 999*ones(length(trsError_random_1000),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('b=500');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.05:0.5])




figure
subplot(1,4,1)
h = boxplot([rotError_ILP_2000, rotError_predict_2000, rotError_random_2000, rotError_voxel_2000, rotError_cluster_2000], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('b=2000');
ylim([0 1.5]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.15:1.5])


subplot(1,4,2)
h = boxplot([rotError_ILP_1500, rotError_predict_1500, rotError_random_1500, rotError_voxel_1500, 999*ones(length(rotError_random_1500),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('b=1500');
ylim([0 1.5]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.15:1.5])




subplot(1,4,3)
h = boxplot([rotError_ILP_1000, rotError_predict_1000, rotError_random_1000, rotError_voxel_1000, 999*ones(length(rotError_random_1000),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('b=1000');
ylim([0 1.5]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.15:1.5])



subplot(1,4,4)
h = boxplot([rotError_ILP_500, rotError_predict_500, 999*ones(length(rotError_random_1000),1), rotError_voxel_500, 999*ones(length(rotError_random_1000),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('b=500');
ylim([0 1.5]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.15:1.5])









