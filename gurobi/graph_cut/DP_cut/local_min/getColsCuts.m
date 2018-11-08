function [  ] = getColsCuts( visMatrixDir, rowsCutDir, saveFileDir )
%GETCOLSCUTS Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files_pose = dir(fullfile(rowsCutDir, fileExt));
    
    rosCutLength= length(files_pose);
    
    for i=1:rosCutLength
        i
        fileName_pose = [rowsCutDir, num2str(i), '.txt'];     
        file_pose = fopen(fileName_pose);
        pose_ID = fscanf(file_pose, '%d');
        
        points_for_section = [];  % all the ovserved Points
        for j = 1:length(pose_ID)
            fileName_point = [visMatrixDir, num2str(pose_ID(j)), '.txt'];
            file_point = fopen(fileName_point);
            point_ID = fscanf(file_pose, '%d');
            fclose(file_pose);
            
            % 
            points_for_section = union(points_for_section, point_ID);
        end
        
        fileName_savePoint = [saveFileDir, num2str(i), '.txt'];
        dlmwrite(fileName_savePoint, points_for_section, 'precision', 10);
        
    end

end

