function [ gndPoseNew ] = poseSparse_yq( gndPose, expDis, saveNewPoseName )
%POSESPARSE_YQ Summary of this function goes here
%   Detailed explanation goes here

    % YQ dataset, data preparation

    keepIndex = [1];
    gndPoseNew(1,:) = gndPose(1,:); % for plotting

    cnt = 1;
    for i = 2:length(gndPose)
        pose1 = [gndPose(i,4), gndPose(i,8)];
        pose2 = [gndPose(keepIndex(cnt),4), gndPose(keepIndex(cnt),8)];
        dis = norm(pose1 - pose2);
        
        if dis > expDis
            cnt = cnt + 1;
            keepIndex(cnt,:) = i;
            gndPoseNew(cnt,:) = gndPose(i,:);
        end
    end
    
%     keepIndex = keepIndex - 1; % from zero
    
    length(keepIndex)
    
    dlmwrite(saveNewPoseName, gndPoseNew, 'delimiter', '\t');


end

