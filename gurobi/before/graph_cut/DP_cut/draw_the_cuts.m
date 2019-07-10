function [ ] = draw_the_cuts( cutCostsFile, cut_place_list )
%DRAW_THE_CUTS Summary of this function goes here
%   Detailed explanation goes here

    file_t = fopen(cutCostsFile);
    cut_costs = fscanf(file_t, '%d');
    fclose(file_t);
    
    % plot the cuttings
    figure
    plot(cut_costs, 'k-', 'LineWidth', 2);
    grid on;
    hold on;
    plot(cut_place_list, cut_costs(cut_place_list),'r*', 'MarkerSize', 10);
    xlabel('Edge', 'FontWeight', 'bold', 'FontSize', 12);
    ylabel('Cost', 'FontWeight', 'bold', 'FontSize', 12);
    
    

end

