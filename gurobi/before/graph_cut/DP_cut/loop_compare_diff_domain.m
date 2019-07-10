function [ ] = loop_compare_diff_domain( cutCostsFile, max_cut_num, search_domain_1, search_domain_2, search_domain_3, max_local_dis )
%LOOP_COMPARE_DIFF_SEARCH Summary of this function goes here
%   Detailed explanation goes here

    DP_cost_list_1 = []; DP_time_list_1 = [];
    DP_cost_list_2 = []; DP_time_list_2 = [];
    DP_cost_list_3 = []; DP_time_list_3 = [];

    av_cost_list = []; av_time_list = [];

    for i = 2:max_cut_num
        [ DP_cost_1, DP_time_1, av_cost, av_time ] = DP_cut_domain( cutCostsFile, i, search_domain_1);
        [ DP_cost_2, DP_time_2, av_cost, av_time ] = DP_cut_domain( cutCostsFile, i, search_domain_2);
        [ DP_cost_3, DP_time_3, av_cost, av_time ] = DP_cut_domain( cutCostsFile, i, search_domain_3);

        
        DP_cost_list_1(i, :) = DP_cost_1;
        DP_time_list_1(i, :) = DP_time_1 * 1000;
        
        DP_cost_list_2(i, :) = DP_cost_2;
        DP_time_list_2(i, :) = DP_time_2 * 1000;
        
        DP_cost_list_3(i, :) = DP_cost_3;
        DP_time_list_3(i, :) = DP_time_3 * 1000;
        
        av_cost_list(i, :) = av_cost;
        av_time_list(i, :) = av_time * 1000;
        
    end
    
    
%     % for local min cut
%     local_cost_list = []; local_time_list = []; cut_num_list = [];
%     i = 2;
%     exp_num = 2;
%     for l = max_local_dis:-1:1
%         [ local_cost, local_time, cut_num ] = localMinTest( cutCostsFile, l);
%         
%         if cut_num >= exp_num
%             local_cost_list(i, :) = local_cost;
%             local_time_list(i, :) = local_time * 1000;
%             cut_num_list(i, :) = cut_num;
%             exp_num = exp_num + 1;
%             i = i + 1;
%         end
%         
%         if cut_num >= max_cut_num
%            break; 
%         end
% 
%     end
    
    
    % optimal cut costs, but not unniform obviously
    opti_cost_list = [];
    for i = 2:max_cut_num
       opti_cost_list(i, :) = get_the_optimal(cutCostsFile, i); 
    end
    
    figure;
    plot(DP_cost_list_1, 'g-', 'LineWidth', 2);
    hold on;
    plot(DP_cost_list_2, 'b-', 'LineWidth', 2);
    hold on;
    plot(DP_cost_list_3, 'r-', 'LineWidth', 2);
    hold on;
    plot(av_cost_list, 'k-', 'LineWidth', 2);
    hold on;
    plot(opti_cost_list, 'm-', 'LineWidth', 2)
%     plot(cut_num_list, local_cost_list, 'm-', 'LineWidth', 2);
    grid on;
    axis([2 max_cut_num 0 max(av_cost_list)]);
    xlabel('Section Num', 'FontWeight', 'bold', 'FontSize', 12);
    ylabel('Cut Cost', 'FontWeight', 'bold', 'FontSize', 12);
%     legend('DP 5', 'DP 10', 'DP 30', 'Uniform Cutting', 'Local Minimum');
    legend('DP 5', 'DP 10', 'DP 30', 'Uniform Cutting', 'Minest');

    
    
    
    
%     figure;
%     plot(DP_time_list_1, 'g-', 'LineWidth', 2);
%     hold on;
%     plot(DP_time_list_2, 'b-', 'LineWidth', 2);
%     hold on;
%     plot(DP_time_list_3, 'r-', 'LineWidth', 2);
%     hold on;
%     plot(av_time_list, 'k-', 'LineWidth', 2);
%     hold on;
% %     plot(cut_num_list, local_time_list, 'c-', 'LineWidth', 2);
%     grid on;
%     axis([2 100 0 2]);
%     xlabel('Section Num', 'FontWeight', 'bold', 'FontSize', 12);
%     ylabel('Time Cost (ms)', 'FontWeight', 'bold', 'FontSize', 12);
% %     legend('Dynamic Programming', 'Uniform Cutting', 'Local Minimum');
%     legend('DP 5', 'DP 10', 'DP 20', 'Uniform Cutting');

    
    
    
end

