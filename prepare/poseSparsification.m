function [ icpPoseNew ] = poseSparsification( icpPose, saveIndexFileName, expDis )
%POSESPARCIFICATION Summary of this function goes here
%   Detailed explanation goes here
    
    % YQ dataset, data preparation

    keepIndex = [1];
    icpPoseNew(1,:) = icpPose(1,:); % for plotting

    cnt = 1;
    for i = 2:length(icpPose)
        pose1 = [icpPose(i,4), icpPose(i,8)];
        pose2 = [icpPose(keepIndex(cnt),4), icpPose(keepIndex(cnt),8)];
        dis = norm(pose1 - pose2);
        
        if dis > expDis
            cnt = cnt + 1;
            keepIndex(cnt,:) = i;
            icpPoseNew(cnt,:) = icpPose(i,:);
        end
    end
    
    keepIndex = keepIndex - 1; % from zero
    
    length(keepIndex)
    
    % save the index to the file
    dlmwrite(saveIndexFileName, keepIndex+8399, 'delimiter', '\t');

end

