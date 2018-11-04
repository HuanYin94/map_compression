function [ ] = getLocalMin( poses, keepIndex, cutCostsFile, MinPeakDistance, MinPeakWidth, saveFileDir)
%GETLOCALMIN Summary of this function goes here
%   Detailed explanation goes here
    
    file_t = fopen(cutCostsFile);
    cut_costs = fscanf(file_t, '%d');
    fclose(file_t);
    
     % inverse is from maximum to minimum
    [local_mins, mins_ids] = findpeaks(-cut_costs, 'MinPeakDistance', MinPeakDistance, 'MinPeakWidth', MinPeakWidth);
    
    figure
    plot(cut_costs, 'k.')
    hold on;
    plot(mins_ids, cut_costs(mins_ids),'r*', 'MarkerSize', 10)
    xlabel('Poses', 'FontSize', 12, 'FontWeight', 'bold')
    ylabel('Cut Cost', 'FontSize', 12, 'FontWeight', 'bold')

    
    % plot section lengthes
    for i=1:length(mins_ids)-1
       sec_len(i) = (mins_ids(i+1) - mins_ids(i)); 
    end
    figure
    plot(sec_len)
    
    % plot cut places
    for i=1:length(mins_ids)
        pose_need_to_plot(i, :) = [poses(keepIndex(mins_ids(i)), 4), poses(keepIndex(mins_ids(i)), 8)];
    end
    figure;
    plot(poses(:,4), poses(:,8), 'k.');
    hold on;
    scatter(pose_need_to_plot(:,1), pose_need_to_plot(:,2), 100, 'r*');
    xlabel('X-axis', 'FontSize', 12, 'FontWeight', 'bold')
    ylabel('Y-axis', 'FontSize', 12, 'FontWeight', 'bold')
    
    
    % save for next processes
    list = [];
    pose_cnt = 1;
    cut_cnt = 1;
    
    for s=0:(length(keepIndex)-1)
        list(pose_cnt, :) = s;
        pose_cnt = pose_cnt + 1;
        
        if  cut_cnt <= length(mins_ids) && s == mins_ids(cut_cnt) 
            
            % save 
            fileName_i = [saveFileDir, num2str(cut_cnt), '.txt']     
            dlmwrite(fileName_i, list);
            
            % reset sum values
            pose_cnt = 1;
            cut_cnt = cut_cnt + 1;
            list = [];
        end
    end
    
    % save the ;ast section without cut at the end
    if (cut_cnt - 1) == length(mins_ids)
        fileName_i = [saveFileDir, num2str(cut_cnt), '.txt']     
        dlmwrite(fileName_i, list);
    end

end

