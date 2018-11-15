% function [ opti_cost ] = get_the_optimal( cutCostsFile, cut_num )
function [ opti_cuts ] = get_the_optimal( cutCostsFile, cut_num )

%GET_THE_OPTIMAL Summary of this function goes here
%   Detailed explanation goes here

    file_t = fopen(cutCostsFile);
    cut_costs = fscanf(file_t, '%d');
    fclose(file_t);
    
    
    cut_costs_sorted = sort(cut_costs);
    
    opti_cost = sum(cut_costs_sorted(1:cut_num-1));
    
    
    
    
    % get the optimal cuts, sequential~!
    cut_costs_sorted_top = cut_costs_sorted(1:cut_num-1);
    
    for i=1:(cut_num-1)
    
        cut_value = cut_costs_sorted_top(i); 
        
        opti_cuts(i,:) = find(cut_costs == cut_value);
        
    end
end

