function [  ] = getCutCosts( cutCostsFile,  MinPeakDistance, MinPeakWidth)
%GETCUTCOSTS Summary of this function goes here
%   Detailed explanation goes here
    
    file_t = fopen(cutCostsFile);
    cut_costs = fscanf(file_t, '%d');
    fclose(file_t);
    
     % inverse is from maximum to minimum
    [local_mins, mins_ids] = findpeaks(-cut_costs, 'MinPeakDistance', MinPeakDistance, 'MinPeakWidth', MinPeakWidth);
    
    all_cost_local_mins = -sum(local_mins)
    
    % average cut
    
    section_num = length(local_mins) + 1;
    
    section_len = floor(length(cut_costs) / section_num);
    
    all_cost_average = 0;
    for i = section_len:section_len:length(cut_costs)
        all_cost_average = all_cost_average + cut_costs(i);
    end
    all_cost_average
end

