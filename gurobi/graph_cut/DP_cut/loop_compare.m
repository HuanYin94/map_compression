function [ ] = loop_compare( cutCostsFile, max_cut_num, search_k, max_local_dis )
%LOOP_COMPARE Summary of this function goes here
%   Detailed explanation goes here
    
    % for DP and av cut

    DP_cost_list = []; DP_time_list = [];
    av_cost_list = []; av_time_list = [];

    for i = 2:max_cut_num
        [ DP_cost, DP_time, av_cost, av_time ] = DP_cut_k( cutCostsFile, i, search_k);
        
        DP_cost_list(i, :) = DP_cost;
        DP_time_list(i, :) = DP_time * 1000;
        av_cost_list(i, :) = av_cost;
        av_time_list(i, :) = av_time * 1000;
        
    end
    
    % for local min cut
    local_cost_list = []; local_time_list = []; cut_num_list = [];
    i = 2;
    exp_num = 2;
    for l = max_local_dis:-1:1
        [ local_cost, local_time, cut_num ] = localMinTest( cutCostsFile, l);
        
        if cut_num >= exp_num
            local_cost_list(i, :) = local_cost;
            local_time_list(i, :) = local_time * 1000;
            cut_num_list(i, :) = cut_num;
            exp_num = exp_num + 1;
            i = i + 1;
        end
        
        if cut_num >= max_cut_num
           break; 
        end

    end
    
    figure;
    plot(DP_cost_list, 'r-', 'LineWidth', 2);
    hold on;
    plot(av_cost_list, 'k-', 'LineWidth', 2);
    hold on;
    plot(cut_num_list, local_cost_list, 'c-', 'LineWidth', 2);
    grid on;
    axis([2 100 0 270000000]);
    xlabel('Section Num', 'FontWeight', 'bold', 'FontSize', 12);
    ylabel('Cut Cost', 'FontWeight', 'bold', 'FontSize', 12);
    legend('Dynamic Programming', 'Uniform Cutting', 'Local Minimum');
    
    
    
    
    figure;
    plot(DP_time_list, 'r-', 'LineWidth', 2);
    hold on;
    plot(av_time_list, 'k-', 'LineWidth', 2);
    hold on;
    plot(cut_num_list, local_time_list, 'c-', 'LineWidth', 2);
    grid on;
    axis([2 100 0 2]);
    xlabel('Section Num', 'FontWeight', 'bold', 'FontSize', 12);
    ylabel('Time Cost (ms)', 'FontWeight', 'bold', 'FontSize', 12);
    legend('Dynamic Programming', 'Uniform Cutting', 'Local Minimum');

end

