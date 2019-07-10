function [ local_cost, local_time, cut_num ] = localMinTest( cutCostsFile, MinPeakDistance)
%GETLOCALMIN Summary of this function goes here
%   Detailed explanation goes here
    
    file_t = fopen(cutCostsFile);
    cut_costs = fscanf(file_t, '%d');
    fclose(file_t);
    
    tic;
     % inverse is from maximum to minimum
    [local_mins, mins_ids] = findpeaks(-cut_costs, 'MinPeakDistance', MinPeakDistance);
%     [local_mins, mins_ids] = findpeaks(-cut_costs, 'MinPeakDistance', MinPeakDistance, 'MinPeakWidth', MinPeakWidth);

    local_cost = sum(-local_mins);
    local_time = toc;    
    
    cut_num = length(local_mins) + 1;
end

