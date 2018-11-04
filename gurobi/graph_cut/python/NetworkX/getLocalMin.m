function [ ] = getLocalMin( cutCostsFile, MinPeakDistance, MinPeakWidth)
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
    plot(mins_ids, cut_costs(mins_ids),'r*', 'MarkerSize', 15)
    
    for i=1:length(mins_ids)-1
       sec_len(i) = (mins_ids(i+1) - mins_ids(i)); 
    end
    
    figure
    plot(sec_len)

end

