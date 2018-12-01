function [ min_cost  ] = get_min_cost( minQ, maxQ, compressResultsDir, qFile, lamda, epsilon_soft )
%GET_MIN_COST Summary of this function goes here
%   Detailed explanation goes here
    
    q_file_t = fopen(qFile);
    observe_value = fscanf(q_file_t, '%d');
    fclose(q_file_t);
    
    % for the qvalue with no maxQ & minQ
    maxNum = max(observe_value);
    if maxNum < maxQ
        observe_value(find(observe_value == maxNum, 1)) = maxQ;
    end
    
    minNum =  min(observe_value);
    if minNum > minQ
        observe_value(find(observe_value == minNum, 1)) = minQ;
    end
    
    % actually not needed in the demo yq_500
%     observe_value(find(observe_value > maxQ)) = maxQ;  % remove some unusual counts
    
    q_value = (1 - mapminmax(observe_value', 0, 1))';  % actually same as in CVPR_W 2013
    
%% calculate
    
    
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
    length(saved_ID)
%     
%     % sum the soft part 
%     for i = 1:length(files)
%         min_cost = min_cost + lamda*epsilon_soft(i,:);
%     end
    
end

