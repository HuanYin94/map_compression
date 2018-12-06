function [ min_cost, totalNum  ] = get_min_cost( compressResultsDir, qFile, lamda, epsilon_soft )
%GET_MIN_COST Summary of this function goes here
%   Detailed explanation goes here
    
    q_file_t = fopen(qFile);
    observe_value = fscanf(q_file_t, '%d');
    fclose(q_file_t);
    observe_value(find(observe_value > 100)) = 100;  % remove some unusual counts
    q_value = (1 - mapminmax(observe_value', 0, 1))';  % actually same as in CVPR_W 2013
    
    fileExt = '*.txt';
    files = dir(fullfile(compressResultsDir,fileExt));  
    
    % get the union cost 
    saved_ID = [];
    for i = 0:length(files)-1
        
        fileCnt = num2str(i);
        fileName = [compressResultsDir, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        point_ID = fscanf(file_t, '%d');
        fclose(file_t);
        
        saved_ID = union(saved_ID, point_ID);
    end
    
    % sum the q_value * if_compress
    min_cost = 0;
    for i = 1:1:length(saved_ID)
        min_cost = min_cost + q_value(saved_ID(i));
    end
    
    % sum the soft part 
    for i = 1:length(files)
        min_cost = min_cost + lamda*epsilon_soft(i,:);
    end
    
    totalNum = length(saved_ID);
    
end

