function [  ] = ForKITTIPoses( savePoseName, saveIndexName, ground_Truth, meanDis )
%FORKITTI Summary of this function goes here
%   Detailed explanation goes here

    keepIndex = [1];    
    
    cnt = 1;
    for i = 2:length(ground_Truth)
        pose1 = [ground_Truth(i,4), ground_Truth(i,12)];
        pose2 = [ground_Truth(keepIndex(cnt),4), ground_Truth(keepIndex(cnt),12)];
        dis = norm(pose1 - pose2);
        
        if dis > meanDis
            cnt = cnt + 1;
            keepIndex(cnt,:) = i;
        end
    end
    
    % save the index to the file
    dlmwrite(saveIndexName, keepIndex, 'delimiter', '\t');
    
    % rotation new
    % KITTI dataset: from left-camera coordinate to LiDAR coordinate
    % help from Zhongxinag Zhou, new transformation new 
    Tr  = [0, -1, 0, 0; 0, 0, -1, 0; 1, 0, 0, 0; 0, 0, 0, 1];
    for i = 1:length(ground_Truth)
       ground_Truth_Tr = [ground_Truth(i, 1:4); ground_Truth(i, 5:8); ground_Truth(i, 9:12); 0, 0, 0, 1]; 
       ground_Truth_Tr_new = inv(Tr) * ground_Truth_Tr * Tr;
       ground_Truth_new(i,:) = [ground_Truth_Tr_new(1, :), ground_Truth_Tr_new(2, :), ground_Truth_Tr_new(3, :), ground_Truth_Tr_new(4, :) ];
    end
        
    dlmwrite(savePoseName, ground_Truth_new, 'delimiter', '\t');
    
end

