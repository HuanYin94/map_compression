function [  ] = DP_save_rows( cutCostsFile, cut_num,  search_k, saveFileAddress )
%DP_SAVE_ROWS Summary of this function goes here
%   Detailed explanation goes here

    file_t = fopen(cutCostsFile);
    cut_costs = fscanf(file_t, '%d');
    fclose(file_t);
    
    % get the initial averaging-cut
    
    cut_length = length(cut_costs) / cut_num;
    
    for i = 1:cut_num-1
       initial_cuts(i,:) = floor(i*cut_length); 
    end
    
    % start Dynamic Programming
    % find the best/min value in the domain near cut_points as initial
    % values
    
    best_values = [];
    best_cuts = [];
    search_domain = floor(cut_length / search_k);
    
    for i = 1:length(initial_cuts)
        
        domain = cut_costs(initial_cuts(i,:)-search_domain:initial_cuts(i,:)+search_domain,:);
        
        [min_value, min_cut] = min(domain);
        
        best_values(i,:) = min_value;
        
        best_cuts(i,:) = initial_cuts(i,:) + (min_cut - search_domain - 1);
        
    end
    
    dlmwrite(saveFileAddress, best_cuts);
    
end

