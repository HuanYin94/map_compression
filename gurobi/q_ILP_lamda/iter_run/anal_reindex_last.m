function [ compressIndex_new ] = anal_reindex_last(compressDir,  reIndexFile )
%ANAL_REINDEX_LAST Summary of this function goes here
%   Detailed explanation goes here

    fileExt = '*.txt';
    compress_files = dir(fullfile(compressDir,fileExt)); 
    compressIndex = [];
    for i = 0:length(compress_files)-1
        
        fileCnt = num2str(i);
        fileName = [compressDir, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        point_ID = fscanf(file_t, '%d');
        fclose(file_t);
        
        compressIndex = union(compressIndex, point_ID);
    
    end
    
    reIndex = dlmread(reIndexFile);  
    
    isInIndex = ismember(reIndex(:,2), compressIndex);
    isSaved = find(isInIndex==1);
    old_index = reIndex(:,1);
    compressIndex_new = old_index(isSaved);
    
end

