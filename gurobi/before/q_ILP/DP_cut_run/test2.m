function [  ] = test2( vis, compressIndex, totalNum )
%TEST2 Summary of this function goes here
%   Detailed explanation goes here

    row = zeros(1, totalNum);
    for i=1:length(vis)
        row(1, vis(i)+1) = 1;
    end
    
    column = zeros(totalNum, 1);
    for i=1:length(compressIndex)
        column(compressIndex(i),1) = 1;
    end
    
    b = row*column
end

