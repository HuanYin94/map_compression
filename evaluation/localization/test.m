% 
% for i=1:length(yqLearned2pose)
%     
%         
%     transErrors_2(i,:) = getTransError(yqpose(i,:), yqLearned2pose(i,:));
%     transErrors_5(i,:) = getTransError(yqpose(i,:), yqLearned5pose(i,:));
%     transErrors_10(i,:) = getTransError(yqpose(i,:), yqLearned10pose(i,:));
%     transErrors_20(i,:) = getTransError(yqpose(i,:), yqLearned20pose(i,:));
%      
% end
% 
% boxplot([transErrors_20, transErrors_10, transErrors_5, transErrors_2], 'Labels',{'20%','10%','5%','2%'}, 'OutlierSize', 9, 'Whisker', 30, 'Widths', 0.5);
% ylim([0 0.6]);
% xlabel('Ratio', 'FontWeight', 'bold', 'FontSize', 12);
% ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 12);
% grid on;

% for i=1:length(yqLearned2pose)
%             
%     rotmErrors_2(i,:) = getRotmError(yqpose(i,:), yqLearned2pose(i,:));
%     rotmErrors_5(i,:) = getRotmError(yqpose(i,:), yqLearned5pose(i,:));
%     rotmErrors_10(i,:) = getRotmError(yqpose(i,:), yqLearned10pose(i,:));
%     rotmErrors_20(i,:) = getRotmError(yqpose(i,:), yqLearned20pose(i,:));
%      
% end
% 
% boxplot([rotmErrors_20, rotmErrors_10, rotmErrors_5, rotmErrors_2], 'Labels',{'20%','10%','5%','2%'}, 'OutlierSize', 9, 'Whisker', 30, 'Widths', 0.5);
% ylim([0 3.55]);
% xlabel('Ratio', 'FontWeight', 'bold', 'FontSize', 12);
% ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 12);
% grid on;

%% random added test

% random2mean = mean([yqRandom15pose, yqRandom25pose, yqRandom35pose, yqRandom45pose, yqRandom55pose], 5);
% random5mean = mean([yqRandom15pose, yqRandom25pose, yqRandom35pose, yqRandom45pose, yqRandom55pose], 5);
% random10mean = mean([yqRandom110pose, yqRandom210pose, yqRandom310pose, yqRandom410pose, yqRandom510pose], 5);
% random20mean = mean([yqRandom120pose, yqRandom220pose, yqRandom320pose, yqRandom420pose, yqRandom520pose], 5);
% 
% for i=1:length(yqRandom15pose)
% 
%     % random 5
%     rotmErrors_5_R(i,:) = getRotmError(yqpose(i,:), random5mean(i,:));
%     transErrors_5_R(i,:) = getTransError(yqpose(i,:), random5mean(i,:));
%     % random 10
%     rotmErrors_10_R(i,:) = getRotmError(yqpose(i,:), random10mean(i,:));
%     transErrors_10_R(i,:) = getTransError(yqpose(i,:), random10mean(i,:));
%     % random 20
%     rotmErrors_20_R(i,:) = getRotmError(yqpose(i,:), random20mean(i,:));
%     transErrors_20_R(i,:) = getTransError(yqpose(i,:), random20mean(i,:));
%     
%     % learned 2
%     transErrors_2_L(i,:) = getTransError(yqpose(i,:), yqLearned2pose(i,:));
%     rotmErrors_2_L(i,:) = getRotmError(yqpose(i,:), yqLearned2pose(i,:));
%     % learned 5
%     transErrors_5_L(i,:) = getTransError(yqpose(i,:), yqLearned5pose(i,:));
%     rotmErrors_5_L(i,:) = getRotmError(yqpose(i,:), yqLearned5pose(i,:));
%     % learned 10
%     transErrors_10_L(i,:) = getTransError(yqpose(i,:), yqLearned10pose(i,:));
%     rotmErrors_10_L(i,:) = getRotmError(yqpose(i,:), yqLearned10pose(i,:));
%     % learned 20
%     transErrors_20_L(i,:) = getTransError(yqpose(i,:), yqLearned20pose(i,:));
%     rotmErrors_20_L(i,:) = getRotmError(yqpose(i,:), yqLearned20pose(i,:));
% 
% end
% 
% for i=1:length(yqpose)
%     
%     % program 20%
%     transErrors_20_P(i,:) = getTransError(yqpose(i,:), yqILP20pose(i,:));
%     rotmErrors_20_P(i,:) = getRotmError(yqpose(i,:), yqILP20pose(i,:));
%     % program 20%
%     transErrors_10_P(i,:) = getTransError(yqpose(i,:), yqILP10pose(i,:));
%     rotmErrors_10_P(i,:) = getRotmError(yqpose(i,:), yqILP10pose(i,:));
%     % program 20%
%     transErrors_5_P(i,:) = getTransError(yqpose(i,:), yqILP5pose(i,:));
%     rotmErrors_5_P(i,:) = getRotmError(yqpose(i,:), yqILP5pose(i,:));
%     % program 20%
%     transErrors_2_P(i,:) = getTransError(yqpose(i,:), yqILP2pose(i,:));
%     rotmErrors_2_P(i,:) = getRotmError(yqpose(i,:), yqILP2pose(i,:));
%     
% end

% subplot(1,4,1)
% h = boxplot([transErrors_20_P, transErrors_20_L, transErrors_20_R], 'Labels',{'Mp','Ml','Mr'}, 'OutlierSize', 8, 'Widths', 0.4);
% title('20%');
% ylim([0 0.5]);
% ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 12);
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.05:0.5])
% 
% subplot(1,4,2)
% h = boxplot([transErrors_10_P, transErrors_10_L, transErrors_10_R], 'Labels',{'Mp','Ml','Mr'}, 'OutlierSize', 8, 'Widths', 0.4);
% title('10%');
% ylim([0 0.5]);
% set(gca,'yticklabel',[])
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.05:0.5])
% 
% subplot(1,4,3)
% h = boxplot([transErrors_5_P, transErrors_5_L, transErrors_5_R], 'Labels',{'Mp','Ml','Mr'}, 'OutlierSize', 8,  'Widths', 0.4);
% title('5%');
% ylim([0 0.5]);
% set(gca,'yticklabel',[])
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.05:0.5])
% 
% subplot(1,4,4)
% h = boxplot([transErrors_2_P, transErrors_2_L, 999*ones(length(transErrors_2_L),1)], 'Labels',{'Mp', 'Ml', 'Mr(F)'}, 'OutlierSize', 8, 'Widths', 0.4);
% title('2%');
% ylim([0 0.5]);
% set(gca,'yticklabel',[])
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.05:0.5])




% 
% figure
% subplot(1,4,1)
% h=boxplot([rotmErrors_20_P, rotmErrors_20_L, rotmErrors_20_R], 'Labels',{'Mp','Ml','Mr'}, 'OutlierSize', 8, 'Widths', 0.4);
% title('20%');
% ylim([0 1.5]);
% ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 12);
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.15:1.5])
% 
% subplot(1,4,2)
% h=boxplot([rotmErrors_10_P, rotmErrors_10_L, rotmErrors_10_R], 'Labels',{'Mp','Ml','Mr'}, 'OutlierSize', 8, 'Widths', 0.4);
% title('10%');
% ylim([0 1.5]);
% set(gca,'yticklabel',[])
% set(gca,'yticklabel',[])
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.15:1.5])
% 
% subplot(1,4,3)
% h=boxplot([rotmErrors_5_P, rotmErrors_5_L, rotmErrors_5_R], 'Labels',{'Mp','Ml','Mr'}, 'OutlierSize', 8, 'Widths', 0.4);
% title('5%');
% ylim([0 1.5]);
% set(gca,'yticklabel',[])
% set(gca,'yticklabel',[])
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.15:1.5])
% 
% subplot(1,4,4)
% h=boxplot([rotmErrors_2_P, rotmErrors_2_L, 999*ones(length(rotmErrors_2_L),1)], 'Labels',{'Mp','Ml','Mr(F)'}, 'OutlierSize', 8, 'Widths', 0.4);
% title('2%');
% ylim([0 1.5]);
% set(gca,'yticklabel',[])
% set(gca,'yticklabel',[])
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.15:1.5])

%% random time

% mean((yqRandom120time + yqRandom220time + yqRandom320time + yqRandom420time + yqRandom520time) / 5)
% 
% mean((yqRandom110time + yqRandom210time + yqRandom310time + yqRandom410time + yqRandom510time) / 5)
% 
% mean((yqRandom15time + yqRandom25time + yqRandom35time + yqRandom45time + yqRandom55time) / 5)
% 
% mean((yqRandom12time(1:300) + yqRandom22time(1:300) + yqRandom32time(1:300) + yqRandom42time(1:300) + yqRandom52time(1:300)) / 5)


%% add clustering and voxel map, 2018.08.31




% for i=1:length(yqpose)
% 
%     % random 20
%     rotmErrors_20_S(i,:) = getRotmError(yqpose(i,:), yqcluster20pose(i,:));
%     transErrors_20_S(i,:) = getTransError(yqpose(i,:), yqcluster20pose(i,:));
% 
% end
% 
% for i=1:length(yqpose)
% 
%     % random 20
%     rotmErrors_20_V(i,:) = getRotmError(yqpose(i,:), yqvoxel20pose(i,:));
%     transErrors_20_V(i,:) = getTransError(yqpose(i,:), yqvoxel20pose(i,:));
%     
%     % random 10
%     rotmErrors_10_V(i,:) = getRotmError(yqpose(i,:), yqvoxel10pose(i,:));
%     transErrors_10_V(i,:) = getTransError(yqpose(i,:), yqvoxel10pose(i,:));
%     
%     % random 5
%     rotmErrors_5_V(i,:) = getRotmError(yqpose(i,:), yqvoxel5pose(i,:));
%     transErrors_5_V(i,:) = getTransError(yqpose(i,:), yqvoxel5pose(i,:));
% end
% 

% subplot(1,4,1)
% h = boxplot([transErrors_20_P, transErrors_20_L, transErrors_20_R, transErrors_20_V, transErrors_20_S], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
% title('20%');
% ylim([0 0.5]);
% ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 12);
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.05:0.5])
% 
% subplot(1,4,2)
% h = boxplot([transErrors_10_P, transErrors_10_L, transErrors_10_R, transErrors_10_V, 999*ones(length(transErrors_2_L),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
% title('10%');
% ylim([0 0.5]);
% set(gca,'yticklabel',[])
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.05:0.5])
% 
% subplot(1,4,3)
% h = boxplot([transErrors_5_P, transErrors_5_L, transErrors_5_R, transErrors_5_V, 999*ones(length(transErrors_2_L),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
% title('5%');
% ylim([0 0.5]);
% set(gca,'yticklabel',[])
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.05:0.5])
% 
% subplot(1,4,4)
% h = boxplot([transErrors_2_P, transErrors_2_L, 999*ones(length(transErrors_2_L),1), 999*ones(length(transErrors_2_L),1), 999*ones(length(transErrors_2_L),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
% title('2%');
% ylim([0 0.5]);
% set(gca,'yticklabel',[])
% set(gca,'FontSize',12)
% set(gca,'ytick', [0:0.05:0.5])











subplot(1,4,1)
h = boxplot([rotmErrors_20_P, rotmErrors_20_L, rotmErrors_20_R, rotmErrors_20_V, rotmErrors_20_S], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('20%');
ylim([0 1.5]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.15:1.5])

subplot(1,4,2)
h = boxplot([rotmErrors_10_P, rotmErrors_10_L, rotmErrors_10_R, rotmErrors_10_V, 999*ones(length(rotmErrors_2_L),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('10%');
ylim([0 1.5]);
set(gca,'yticklabel',[])
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.15:1.5])

subplot(1,4,3)
h = boxplot([rotmErrors_5_P, rotmErrors_5_L, rotmErrors_5_R, rotmErrors_5_V, 999*ones(length(rotmErrors_2_L),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('5%');
ylim([0 1.5]);
set(gca,'yticklabel',[])
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.15:1.5])

subplot(1,4,4)
h = boxplot([rotmErrors_2_P, rotmErrors_2_L, 999*ones(length(rotmErrors_2_L),1), 999*ones(length(rotmErrors_2_L),1), 999*ones(length(rotmErrors_2_L),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('2%');
ylim([0 1.5]);
set(gca,'yticklabel',[])
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.15:1.5])










