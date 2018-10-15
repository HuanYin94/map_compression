function [ sameNum ] = poseOverlap( filesDir )
%POSEOVERLAP Summary of this function goes here
%   Detailed explanation goes here
    
    % get the overlap weighting between consecutive poses
    
    fileExt = '*.txt';
    files = dir(fullfile(filesDir,fileExt)); 
    
    pathLength= length(files);
    
    sameNum = [];
        
    for i = 0:pathLength-1
        i
        fileCnt = num2str(i);
        fileName = [filesDir, fileCnt, '.txt'];     
        file_t = fopen(fileName);
                
        if i == 0
            pointID_previous = fscanf(file_t, '%d');
            continue;
        end 
        
        pointID = fscanf(file_t, '%d');
        
        IN = intersect(pointID_previous, pointID);
        pointID_previous = pointID;
        
        sameNum(i,:) = length(IN);
        
        fclose all;
    end
    
    
end

