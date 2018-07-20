function [  ] = loopCompress( lamda, qFile, filesDir, splitLength, totalNum, bValue, saveDir )
%LOOPCOMPRESS Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(filesDir,fileExt));  
    
    saveCnt = 0;
    
    q_file_t = fopen(qFile);
    q_value = fscanf(q_file_t, '%d');
    fclose(q_file_t);
    q_value(find(q_value>100)) = 50;
    q_value = (1 - mapminmax(q_value', 0, 1))'; % inverse the weighting as the observation counts
    
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
        compressIndex = section(lamda, q_value, filesDir, start, finish, totalNum, bValue);
        
        saveName = [saveDir, num2str(saveCnt), '.txt'];
        dlmwrite(saveName, compressIndex, 'precision', '%d');
        
        disp('Compressed & Saved')
        saveCnt = saveCnt + 1;
        
    end
    
end

