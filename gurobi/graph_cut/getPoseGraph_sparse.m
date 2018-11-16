function [  ] = getPoseGraph_sparse( visFilesDir, windowLength, saveAddress,  sparse_separate  )
%GETPOSEGRAPH_SPARSE Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(visFilesDir,fileExt)); 
    
    pathLength= length(files);
    
    % the nodes we need to save 
    node_list = [];
    cnt = 1;
    for i=0:sparse_separate:pathLength-1
        node_list(cnt, :) = i;
    end     
    
    % build the graph from the vis matrix and the sparse nodes
    visFilesDir
    
    
end

