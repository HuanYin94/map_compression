function [  ] = precision( gtLabel, testLabel )
%PRECISION Summary of this function goes here
%   Detailed explanation goes here
    

    gtIndex = find(gtLabel == 1);
    testIndex = find(testLabel == 1);

    sameNum = intersect(gtIndex, testIndex);

    prediction = length(sameNum) / length(testIndex)
    recall = length(sameNum) / length(gtIndex)
    
end

