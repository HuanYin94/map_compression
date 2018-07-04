function [  ] = loopCompress( filesDir, splitLength, totalNum, bValue, saveDir )
%LOOPCOMPRESS Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(filesDir,fileExt));  
    
    saveCnt = 0;
    
    for i=0:splitLength:length(files)
        disp('--------------------------------------');
        start = i;
        finish = i + splitLength - 1;
        % for the last section
        if finish > length(files)
            finish = length(files) - 1;
        end
        disp(start);
        disp(finish);
        
        % core
        compressIndex = section(filesDir, start, finish, totalNum, bValue);
        
        saveName = [saveDir, num2str(saveCnt), '.txt'];
        dlmwrite(saveName, compressIndex);
        
        disp('Compressed & Saved')
        saveCnt = saveCnt + 1;
        
    end
    
end

