function [ compressIndex_new ] = anal_reindex_middle( compressIndex, reIndexFile )
%ANAL_REINDEX Summary of this function goes here
%   Detailed explanation goes here

    reIndex = dlmread(reIndexFile);  

    isInIndex = ismember(reIndex(:,2), compressIndex);
    isSaved = find(isInIndex==1);
    old_index = reIndex(:,1);
    compressIndex_new = old_index(isSaved);
    
end

