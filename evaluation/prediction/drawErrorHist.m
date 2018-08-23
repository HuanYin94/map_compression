function [  ] = drawErrorHist( errors )
%DRAWERRORHIST Summary of this function goes here
%   Detailed explanation goes here
    
%     errors(find(errors>1.0))=1.0;
    edges = [0 : 0.05 : 1];
    histogram(errors, edges, 'Normalization', 'probability', 'EdgeColor', [0.4940    0.1840    0.5560], 'FaceColor', [0.4940    0.1840    0.5560], 'FaceAlpha', 0.8, 'EdgeAlpha', 0.8);
    grid on;
    xlabel('Matched Distance (m)', 'FontWeight', 'bold', 'FontSize', 12);
    ylabel('Ratio', 'FontWeight', 'bold', 'FontSize', 12);
    axis([0.0, 1.0, 0, 0.7]); 
%     set(gca,'xtick', [0:0.1:1])
%     set(gca,'ytick', [0:0.1:0.5])




end

