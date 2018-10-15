function [  ] = seeOverlap( samer, pose, keepIndex )
%SEEOVERLAP Summary of this function goes here
%   Detailed explanation goes here
    
    
    for i = 2:length(keepIndex)
       icpKeep(i-1,:) = pose(keepIndex(i), :);        
    end
    
%     plot(icpKeep(:, 4), icpKeep(:, 8), 'k.');
    
    colors = mapminmax(samer', 0, 1);
    
%     colors = log2(colors);
    
    scatter(icpKeep(:,4), icpKeep(:,8), 12, colors, 'filled');
    colorbar;
    

end

