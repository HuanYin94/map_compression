function [ ground_Truth_new, ground_Truth_keep ] = poseSparsification_kitti( savePoseName, saveIndexName, ground_Truth, meanDis )
%FORKITTI Summary of this function goes here
%   Detailed explanation goes here

    % rotation new
    % KITTI dataset: from left-camera coordinate to LiDAR coordinate
    % help from Zhongxinag Zhou, new transformation new 
    Tr  = [0, -1, 0, 0; 0, 0, -1, 0; 1, 0, 0, 0; 0, 0, 0, 1];
    for i = 1:length(ground_Truth)
       ground_Truth_Tr = [ground_Truth(i, 1:4); ground_Truth(i, 5:8); ground_Truth(i, 9:12); 0, 0, 0, 1]; 
       ground_Truth_Tr_new = inv(Tr) * ground_Truth_Tr * Tr;
       ground_Truth_new(i,:) = [ground_Truth_Tr_new(1, :), ground_Truth_Tr_new(2, :), ground_Truth_Tr_new(3, :), ground_Truth_Tr_new(4, :) ];
    end
        
    keepIndex = [1];    
    ground_Truth_keep(1,:) = ground_Truth_new(1,:);
    
    cnt = 1;
    for i = 2:length(ground_Truth_new)
        pose1 = [ground_Truth_new(i,4), ground_Truth_new(i,8)];
        pose2 = [ground_Truth_new(keepIndex(cnt),4), ground_Truth_new(keepIndex(cnt),8)];
        dis = norm(pose1 - pose2);
        
        if dis > meanDis
            cnt = cnt + 1;
            keepIndex(cnt,:) = i;
            ground_Truth_keep(cnt,:) = ground_Truth_new(i,:);
        end
    end
    
    keepIndex = keepIndex - 1;  % from the first scan
    
    length(keepIndex)
    
    % save the index to the file
    dlmwrite(saveIndexName, keepIndex, 'delimiter', '\t');
    dlmwrite(savePoseName, ground_Truth_keep, 'delimiter', '\t');

    
end

