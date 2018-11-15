function [  ] = Uniform_loopCompress( lamda, qFile, filesDir, cut_num, totalNum, bValue, saveDir )
%LOOPCOMPRESS Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(filesDir,fileExt));  
    
    splitLength =  floor(length(files) / cut_num);
        
    q_file_t = fopen(qFile);
    observe_value = fscanf(q_file_t, '%d');
    fclose(q_file_t);
   q_max = max(observe_value);
   q_value = q_max - observe_value ;  % same as CVPR_W 2013
   
    time_sum = 0;
    saveCnt = 0;
    for i=0:splitLength:length(files)
        disp('-----------------------------------------------------------------------');
        start = i;
        finish = i + splitLength - 1;
        % for the last section
        if finish > length(files)
            finish = length(files) - 1;
        end
        disp(start);
        disp(finish);
        
        % core
        [compressIndex, time_sec] = section_compress(lamda, q_value, filesDir, start, finish, totalNum, bValue);
        time_sum = time_sum + time_sec;
        saveName = [saveDir, num2str(saveCnt), '.txt'];
        dlmwrite(saveName, compressIndex, 'precision', '%d');
        
        disp('Compressed & Saved')
        saveCnt = saveCnt + 1;
        
    end
    
    disp('All time of programming:');
    disp(time_sum)
    
end

