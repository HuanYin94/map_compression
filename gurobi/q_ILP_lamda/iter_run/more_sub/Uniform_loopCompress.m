function [ epsilon_soft, time_sum  ] = Uniform_loopCompress( lamda, qFile, visFilesDir, splitLength, totalNum, bValue, saveDir, savedPointsFile )
%LOOPCOMPRESS Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(visFilesDir,fileExt));  
    
        
    q_file_t = fopen(qFile);
    observe_value = fscanf(q_file_t, '%d');
    fclose(q_file_t);
    observe_value(find(observe_value > 100)) = 100;  % remove some unusual counts
    q_value = (1 - mapminmax(observe_value', 0, 1))';  % actually same as in CVPR_W 2013
   
    time_sum = 0;
    saveCnt = 0;
    epsilon_soft = [];
    
    for i=0:splitLength:length(files)-1
        disp('-----------------------------------------------------------------------');
        start = i; 
        finish = i + splitLength - 1;
        % for the last section
        if finish > (length(files) - splitLength) % last section will be longer than others
            finish = length(files) - 1;
        end
        disp(start);
        disp(finish);
        
        % core
        [compressIndex, time_sec, epsilon_part] = section_compress(lamda, q_value, visFilesDir, start, finish, totalNum, bValue, savedPointsFile);
        time_sum = time_sum + time_sec;
        saveName = [saveDir, num2str(saveCnt), '.txt'];
        dlmwrite(saveName, compressIndex, 'precision', '%d');
        
        % soft part
        epsilon_soft  = [epsilon_soft;epsilon_part];
        
        disp('Compressed & Saved')
        saveCnt = saveCnt + 1;
        
        % added fucked!
        if finish == length(files) - 1 
            break; % jump out the fucking looping
        end
        
    end
    
    disp('All time of programming:');
    disp(time_sum)
    
end

