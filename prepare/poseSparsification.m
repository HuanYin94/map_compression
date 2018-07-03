function [ keepIndex ] = poseSparsification( icpPose, meanDis, saveIndexFileName )
%POSESPARCIFICATION Summary of this function goes here
%   Detailed explanation goes here
    
    keepIndex = [1];
    icpPoseNew(1,:) = icpPose(1,:); % for plotting
    
    cnt = 1;
    for i = 2:length(icpPose)
        pose1 = [icpPose(i,4), icpPose(i,8)];
        pose2 = [icpPose(keepIndex(cnt),4), icpPose(keepIndex(cnt),8)];
        dis = norm(pose1 - pose2);
        
        if dis > meanDis
            cnt = cnt + 1;
            keepIndex(cnt,:) = i;
            icpPoseNew(cnt,:) = icpPose(i,:);
        end
    end
    
    % save the index to the file
    dlmwrite(saveIndexFileName, keepIndex, 'delimiter', '\t');

end

