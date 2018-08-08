function [ ] = getDistribution( gt_pose )
%GETDISTRIBUTION_YQ Summary of this function goes here
%   Detailed explanation goes here

    for i=2:length(gt_pose)
        
        pose1 = [gt_pose(i,4), gt_pose(i,8)];
        pose2 = [gt_pose(i-1,4), gt_pose(i-1,8)];
        
        distances(i-1) = norm(pose1 - pose2);
        
    end
    
    histogram(distances, 'Normalization', 'probability');
        
    variance = mean(distances)

end

