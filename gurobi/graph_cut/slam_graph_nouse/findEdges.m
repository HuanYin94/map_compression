function [ poseLists ] = findEdges( trees, pathLen)
%FINDEDGES Summary of this function goes here
%   Detailed explanation goes here
    
    % get the smallest edges of trees to get ?
    treeLen = length(trees);
    
    pCnt = 1;
    
    for i=1:treeLen
       if trees(i, 1) < pathLen && trees(i, 2) < pathLen
           poseLists(pCnt, :) = trees(i, :);
       end
    end
    
    
end