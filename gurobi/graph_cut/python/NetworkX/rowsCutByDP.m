function [  ] = rowsCutByDP( cutCostsFile, maxLength )
%ROWSCUTBYDP Summary of this function goes here
%   Detailed explanation goes here

    file_t = fopen(cutCostsFile);
    cut_costs = fscanf(file_t, '%d');
    fclose(file_t);
    
    section_len = maxLength / 2;
    
    % select 
    
end

