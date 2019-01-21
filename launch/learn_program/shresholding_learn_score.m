function [ predict  ] = shresholding_learn_score( predict_score, threhold )
%SHRESHOLDING_LEARN_SCORE Summary of this function goes here
%   Detailed explanation goes here
    
    predict_score = predict_score(:,2);
    
    predict = double(predict_score > threhold);
    
    sum(predict)
    
    
    
end

