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
    search_domain_start = floor(cut_length / search_k);
    search_domain_finish = floor(cut_length / search_k);
    
    sum_best_value = 0;
    
    for i = 1:length(initial_cuts)
        
        start = initial_cuts(i,:)-search_domain_start;
        finish = initial_cuts(i,:)+search_domain_finish;
        
        if start <= 0
            start = 1;
            search_domain_start = initial_cuts(i,:) - start;
        end
        
        if finish > length(cut_costs)-1
            finish  = length(cut_costs)-1;
            search_domain_finish = finish - initial_cuts(i,:);
        end
        
        domain = cut_costs(start:finish,:);
        
        % observe the minimum
%         [min_value, min_cut] = min(domain);
        % obserbe the maximum
        [min_value, min_cut] = max(domain);
        
        best_values(i,:) = min_value;
        
        if min_cut < initial_cuts(i,:)
            best_cuts(i,:) = initial_cuts(i,:) + (min_cut - search_domain_start - 1);
        else
            best_cuts(i,:) = initial_cuts(i,:) + (min_cut - search_domain_finish - 1);
        end
        
        if i > 1 && best_cuts(i,:) == best_cuts(i-1,:)
            best_cuts(i,:) = best_cuts(i,:) + 1;
            best_values(i,:) = domain(min_cut + 1);
        end
        
        sum_best_value = sum_best_value + best_values(i,:);
        
    end
    
    dlmwrite(saveFileAddress, best_cuts);
    sum_best_value
    
end

