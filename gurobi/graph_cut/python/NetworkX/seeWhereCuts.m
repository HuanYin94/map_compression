function [  ] = seeWhereCuts( cutCostsFile, poses, keepIndex, ignoreSides, testNum)
%SEEWHERECUTS Summary of this function goes here
%   Detailed explanation goes here

    file_t = fopen(cutCostsFile);
    cut_costs = fscanf(file_t, '%d');
    fclose(file_t);
    
    sorted_costs = sort(cut_costs);
    
   index_need_plot = [];
   index_cnt = 1;
    for i=1:testNum
    
        cost = sorted_costs(i);
        
        index = find(cut_costs == cost);
        index = index(1);
        if (index > ignoreSides) & (index < (length(keepIndex)-ignoreSides))
            index_need_plot(index_cnt) = index;
            index_cnt = index_cnt + 1;
        else
            continue;
        end
        
    end
    
    display(length(index_need_plot))
    
    for i=1:length(index_need_plot)
        pose_need_to_plot(i, :) = [poses(keepIndex(index_need_plot(i)), 4), poses(keepIndex(index_need_plot(i)), 8)];
    end
    
    figure;
    plot(poses(:,4), poses(:,8), 'k.');
    
    hold on;
    scatter(pose_need_to_plot(:,1), pose_need_to_plot(:,2), 20, 'filled');

        
end

