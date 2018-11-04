function [  ] = getColsCuts( visMatrixDir, rowsCutDir, saveFileDir )
%GETCOLSCUTS Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files_pose = dir(fullfile(rowsCutDir, fileExt));
    
    rosCutLength= length(files_pose);
    
    for i=1:rosCutLength
        
        fileName_pose = [rowsCutDir, num2str(i), '.txt'];     
        file_pose = fopen(fileName_pose);
        poseID = fscanf(file_pose, '%d');
        
        for j = 1:length(poseID)
            fileName_point = [visMatrixDir, num2str(poseID(j)), '.txt'];     
            file_point = fopen(fileName_point);
            poseID = fscanf(file_pose, '%d');
    
            % qiu jiao ji
            
        end
        
    end

end

