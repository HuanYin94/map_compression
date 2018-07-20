function [ meanDis ] = getMeanDistance_kitti( ground_truth )
%GETMEANDISTANCE Summary of this function goes here
%   Detailed explanation goes here
    
    % in kitti dataset

    sumDis = 0;
    
    for i = 2:length(ground_truth)
        pose1 = [ground_truth(i,4), ground_truth(i,12)];
        pose2 = [ground_truth(i-1,4), ground_truth(i-1,12)];
        dis = norm(pose1 - pose2);
        sumDis = sumDis + dis;
    end
    
    meanDis = sumDis / (length(ground_truth) - 1);

end

