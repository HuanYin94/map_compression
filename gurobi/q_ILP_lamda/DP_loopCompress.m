function [ epsilon_soft ] = DP_loopCompress( lamda, qFile, filesDir, cutFile, totalNum, bValue, saveDir )
%LOOPCOMPRESS Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(filesDir,fileExt));  
    
    saveCnt = 0;
    
    q_file_t = fopen(qFile);
    observe_value = fscanf(q_file_t, '%d');
    fclose(q_file_t);
    observe_value(find(observe_value > 100)) = 100;  % remove some unusual counts
    q_value = (1 - mapminmax(observe_value', 0, 1))';  % actually same as in CVPR_W 2013
    
    % read cut places
    cut_file_t = fopen(cutFile);
    cut_places = fscanf(cut_file_t, '%d');
    fclose(cut_file_t);
    
    time_sum = 0;
    saveCnt = 0;   
    
    cutCnt = 1;
    splitLength = cut_places(cutCnt);
    i = 0;
    epsilon_soft = [];

    while 1  
        disp('-----------------------------------------------------------------------');
        start = i;
        finish = i + splitLength - 1;
        if finish > length(files)
            finish = length(files) - 1;
        end
        disp(start);
        disp(finish);
        
         % core
        [compressIndex, time_sec, epsilon_part] = section_compress(lamda, q_value, filesDir, start, finish, totalNum, bValue);
        time_sum = time_sum + time_sec;
        saveName = [saveDir, num2str(saveCnt), '.txt'];
        dlmwrite(saveName, compressIndex, 'precision', '%d');
        
        % soft part
        epsilon_soft  = [epsilon_soft;epsilon_part];
        
        disp('Compressed & Saved')
        saveCnt = saveCnt + 1;      
         i  = i + splitLength;  
        
        % ge the next cutLength
        cutCnt = cutCnt + 1;
        disp(cutCnt)
        if cutCnt <= length(cut_places)
            splitLength = cut_places(cutCnt) - cut_places(cutCnt-1);
        elseif  cutCnt == length(cut_places)  + 1;
            splitLength = 9999;
        else
            break;  % jump out the fucking while
        end
%         disp(spliLength);      

    end

    disp('All time of programming:');
    disp(time_sum)
    
end

