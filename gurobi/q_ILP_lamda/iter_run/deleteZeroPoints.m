function [ save_totalNum ] = deleteZeroPoints( weightFile, visFilesDir, compressResultsDir, saveNewVisDir, saveReIndexFile, saveNewQFile )
%DELETEZEROPOINTS Summary of this function goes here
%   Detailed explanation goes here
    
    % get the union result
    
    fileExt = '*.txt';
    files = dir(fullfile(compressResultsDir,fileExt)); 
    for i = 0:length(files)-1
        
        fileCnt = num2str(i);
        fileName = [compressResultsDir, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        point_ID = fscanf(file_t, '%d');
        fclose(file_t);
        
        all_ID = union(all_ID, point_ID);
    
    end
    
    save_totalNum = length(all_ID);
    
    % get the one2one indexes relations and save
    indexList = [];
    for i= 1:length(all_ID)
        indexList(i,1) = all_ID(i);
        indexList(i,2) = i-1;  % map point re-index from zero
    end
    dlmwrite(indexList, saveReIndexFile, 'precision', '%d');

    
    
    % update the visMatrix and save
    
    fileExt = '*.txt';
    files = dir(fullfile(visFilesDir,fileExt)); 
    for i=0:length(files)
        
        fileCnt = num2str(i);
        old_fileName = [visFilesDir, fileCnt, '.txt'];
        file_old = fopen(old_fileName);
        point_ID_old = fscanf(filfile_olde_t, '%d');
        
        point_ID_new = [];
        remain_cnt = 1;
        for j =1:length(point_ID_old)
            row_indexList = find(indexList(:,1) == point_ID_old(j));
            if size(row_indexList,1) == 1
                point_ID_new(remain_cnt,:) = indexList(row_indexList,2); % the map point index
                remain_cnt = remain_cnt + 1;
            else  % zero matrix
                continue;
            end
        end
        
        % save the new visible map indexes
        new_fileName = [saveNewVisDir, fileCnt, '.txt'];
        dlmwrite(point_ID_new, saveNewVisDir, 'precision', '%d');

    end
    
    % update the q-matrix-vector
    file_q = fopen(weightFile);
    weights = fscanf(file_q, '%d');
    new_weights = [];
    for i = 1:length(all_ID)
        new_weights(i,:) = weights(all_ID(i));
    end
    dlmwrite(new_weights, saveNewQFile, 'precision', '%d');

end

