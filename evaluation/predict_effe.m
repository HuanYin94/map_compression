function [  ] = predict_effe( testData, predictData, threshold, saveFileName )
%PREDICT_EFFE Summary of this function goes here
%   Detailed explanation goes here
    
    % loading manually

    % for test file, the lastcolumn is the ground truth
    gt_index = find(testData == 1);
    
    % the first column represtns the ZERO probability
    pr_index = find(predictData < threshold);
    
    sameNum = intersect(pr_index, gt_index);
    
    disp(length(gt_index))
    
    disp(length(pr_index))
    
    prediction = length(sameNum) / length(pr_index)
    recall = length(sameNum) / length(gt_index)
    
    saveIndex = double(predictData < threshold);
    dlmwrite(saveFileName, saveIndex);
    
end

