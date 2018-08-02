function [ meanDis ] = getMeanDistance_yq( icpPose )
%GETMEANDISTANCE Summary of this function goes here
%   Detailed explanation goes here
    
    % in YQ campus

    sumDis = 0;
    
    for i = 2:length(icpPose)
        pose1 = [icpPose(i,4), icpPose(i,8)];
        pose2 = [icpPose(i-1,4), icpPose(i-1,8)];
        dis = norm(pose1 - pose2);
        sumDis = sumDis + dis;
    end
    
    meanDis = sumDis / (length(icpPose) - 1);

end

