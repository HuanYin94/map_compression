function [ pointByPose ] = getPointMat( filesDir)
%XXXX Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(filesDir,fileExt)); 
    
    pathLength= length(files);
    
    pointByPose = [];
    
    for i = 0:pathLength-1
        i
        
        fileCnt = num2str(i);
        fileName = [filesDir, fileCnt, '.txt'];     
        file_t = fopen(fileName);
        
        pointID = fscanf(file_t, '%d');
        fclose(file_t);    
        
        % add this pose to some point        
        for p = 1:length(pointID)
           pointByPose(pointID(p), i+1) = i+1; 
        end
        
    end

end

