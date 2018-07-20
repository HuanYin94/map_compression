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
    
    ground_truth_new = [ground_Truth(:,1:4), ground_Truth(:,9:12), zeros(length(ground_Truth), 3), ones(length(ground_Truth), 1)];

    dlmwrite(savePoseName, ground_truth_new, 'delimiter', '\t');
    

end

