function [ epsilon_soft, time_sum  ] = Uniform_loopCompress_Cells( lamda, qFile, visCells, splitLength, totalNum, bValue, saveDir )
%LOOPCOMPRESS Summary of this function goes here
%   Detailed explanation goes here  
        
    q_file_t = fopen(qFile);
    observe_value = fscanf(q_file_t, '%d');
    fclose(q_file_t);
    observe_value(find(observe_value > 100)) = 100;  % remove some unusual counts
    q_value = (1 - mapminmax(observe_value', 0, 1))';  % actually same as in CVPR_W 2013
   
    time_sum = 0;
    saveCnt = 0;
    epsilon_soft = [];
    
    for i=0:splitLength:length(visCells)-1
        disp('-----------------------------------------------------------------------');
        start = i; 
        finish = i + splitLength - 1;
        % for the last section
        if finish > (length(visCells) - splitLength) % last section will be longer than others
            finish = length(visCells) - 1;
        end
        disp(start);
        disp(finish);
        
        % core
        [compressIndex, time_sec, epsilon_part] = section_compress_Cells(lamda, q_value, visCells, start, finish, totalNum, bValue);
        time_sum = time_sum + time_sec;
        saveName = [saveDir, num2str(saveCnt), '.txt'];
        dlmwrite(saveName, compressIndex, 'precision', '%d');
        
        % soft part
        epsilon_soft  = [epsilon_soft;epsilon_part];
        
        disp('Compressed & Saved')
        saveCnt = saveCnt + 1;
        
        % added fucked!
        if finish == length(visCells) - 1 
            break; % jump out the fucking looping
        end
        
    end
    
    disp('All time of programming:');
    disp(time_sum)
    
end

