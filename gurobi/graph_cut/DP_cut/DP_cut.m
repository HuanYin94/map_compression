function [ DP_cost, DP_time, av_cost, av_time ] = DP_cut( cutCostsFile, cut_num, search_k)
%DP_CUT Summary of this function goes here
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
    
    tic;
    best_values = [];
    best_cuts = [];
    search_domain = floor(cut_length / search_k);
    
    for i = 1:length(initial_cuts)
        
        domain = cut_costs(initial_cuts(i,:)-search_domain:initial_cuts(i,:)+search_domain,:);
        
        [min_value, min_cut] = min(domain);
        
        best_values(i,:) = min_value;
        
        best_cuts(i,:) = initial_cuts(i,:) + (min_cut - search_domain - 1);
        
    end
        
    DP_cost = sum(best_values);
    DP_time = toc;
    
    
    
    
    
    
    tic
    % get average cost sum
    av_sum = 0;
    for i = 1:length(initial_cuts)
       av_sum = av_sum + cut_costs(initial_cuts(i));
    end
    av_cost = av_sum;
    av_time = toc;
    
end

