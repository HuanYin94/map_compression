function [ ] = just_save( predictData, threshold, saveFileName, isSave )
%JUST_SAVE Summary of this function goes here
%   Detailed explanation goes here

    % the first column represtns the ZERO probability                    
    saveIndex = double(predictData < threshold);
    
    sum(saveIndex) / length(saveIndex)
    
    if(isSave)
        dlmwrite(saveFileName, saveIndex);
    end
end

