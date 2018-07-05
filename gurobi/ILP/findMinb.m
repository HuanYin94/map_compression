function [  ] = findMinb( Dir )
%FINDMINB Summary of this function goes here
%   Detailed explanation goes here

    Matches = [];
    
    fileExt = '*.txt';
    files = dir(fullfile(Dir,fileExt)); 
    
    for i=0:1:length(files)-1
        fileCnt = num2str(i);
        fileName = [Dir, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        pointID = fscanf(file_t, '%d');
        fclose(file_t);
        
        Matches(i+1) = length(pointID);
    end
    
    min(Matches)
    

end

