function [ precision, recall ] = precision_recall( Y_result, Y_Gnd )
%PRECISION_RECALL Summary of this function goes here
%   Detailed explanation goes here

    TP=0;
    FP=0;
    FN=0;
    
    for i=1:length(Y_result)
       
        if Y_result(i)==1 && Y_Gnd(i) == 1
            TP=TP+1;
        elseif Y_result(i)==1 && Y_Gnd(i) == 0
            FP=FP+1;
        elseif Y_result(i)==0 && Y_Gnd(i) == 1
            FN=FN+1;
        end
            
        
    end
    
    precision = TP / (TP+FP);
    recall = TP / (TP+FN);

end

