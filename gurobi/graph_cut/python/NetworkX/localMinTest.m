function [ ] = localMinTest( cutCostsFile, MinPeakDistance, MinPeakWidth)
%GETLOCALMIN Summary of this function goes here
%   Detailed explanation goes here
    
    file_t = fopen(cutCostsFile);
    cut_costs = fscanf(file_t, '%d');
    fclose(file_t);
    
     % inverse is from maximum to minimum
    [local_mins, mins_ids] = findpeaks(-cut_costs, 'MinPeakDistance', MinPeakDistance, 'MinPeakWidth', MinPeakWidth);
    
    length(local_mins)
    
    sum(-local_mins)

end

