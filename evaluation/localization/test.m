
for i=1:length(yqLearned2pose)
    
        
    transErrors_2(i,:) = getTransError(yqpose(i,:), yqLearned2pose(i,:));
    transErrors_5(i,:) = getTransError(yqpose(i,:), yqLearned5pose(i,:));
    transErrors_10(i,:) = getTransError(yqpose(i,:), yqLearned10pose(i,:));
    transErrors_20(i,:) = getTransError(yqpose(i,:), yqLearned20pose(i,:));
     
end

boxplot([transErrors_20, transErrors_10, transErrors_5, transErrors_2], 'Labels',{'20%','10%','5%','2%'}, 'OutlierSize', 9, 'Whisker', 20, 'Widths', 0.5);
% 
% for i=1:length(yqLearned2pose)
%             
%     rotmErrors_2(i,:) = getRotmError(yqpose(i,:), yqLearned2pose(i,:));
%     rotmErrors_5(i,:) = getRotmError(yqpose(i,:), yqLearned5pose(i,:));
%     rotmErrors_10(i,:) = getRotmError(yqpose(i,:), yqLearned10pose(i,:));
%     rotmErrors_20(i,:) = getRotmError(yqpose(i,:), yqLearned20pose(i,:));
%      
% end
% 
% boxplot([rotmErrors_20, rotmErrors_10, rotmErrors_5, rotmErrors_2], 'Labels',{'20%','10%','5%','2%'}, 'OutlierSize', 9, 'Whisker', 20, 'Widths', 0.5);
% xlabel('Ratio');











