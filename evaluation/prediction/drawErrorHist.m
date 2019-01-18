function [  ] = drawErrorHist( path )
%DRAWERRORHIST Summary of this function goes here
%   Detailed explanation goes here
    
    errors  = dlmread(path);

    edges = [0 : 0.05 : 1];
    histogram(errors, edges, 'Normalization', 'probability', 'EdgeColor', [0.4940    0.1840    0.5560], 'FaceColor', [0.4940    0.1840    0.5560], 'FaceAlpha', 0.8, 'EdgeAlpha', 0.8);
    grid on;
    xlabel('Matched Distance (m)', 'FontWeight', 'bold', 'FontSize', 14);
    ylabel('Ratio', 'FontWeight', 'bold', 'FontSize', 14);
    set(gca,'XTick',0:0.1:1);
    set(gca,'YTick',0:0.2:1);
    axis([0.0, 1.0, 0, 1.0]); 
    set(gca,'FontSize',12);

% 
%     edges = [0 : 0.05 : 1];
%     histogram(errors, edges, 'Normalization', 'probability', 'EdgeColor', [0.4940    0.1840    0.5560], 'FaceColor', [0.4940    0.1840    0.5560], 'FaceAlpha', 0.8, 'EdgeAlpha', 0.8);
%     grid on;
%     xlabel('Matched Distance (m)', 'FontWeight', 'bold', 'FontSize', 14);
%     ylabel('Ratio', 'FontWeight', 'bold', 'FontSize', 14);
%     set(gca,'XTick',0:0.1:1);
%     set(gca,'YTick',0:0.2:1.0);
%     axis([0.0, 1.0, 0, 1.0]); 
%     set(gca,'FontSize',12);


end

