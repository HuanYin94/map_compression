function [ save_totalNum,  visCells_new ] = deleteZeroPoints_Cells( weightFile, visCells, compressResultsDir, saveReIndexFile, saveNewQFile )
%DELETEZEROPOINTS Summary of this function goes here
%   Detailed explanation goes here
    
    % get the union result
    disp('Prepare the next optimization');
    
    fileExt = '*.txt';
    compress_files = dir(fullfile(compressResultsDir,fileExt)); 
    all_ID = [];
    for i = 0:length(compress_files)-1
        
        fileCnt = num2str(i);
        fileName = [compressResultsDir, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        point_ID = fscanf(file_t, '%d');
        fclose(file_t);
        
        all_ID = union(all_ID, point_ID);
    
    end
    save_totalNum = length(all_ID);
    
    
    %%
    % get the one2one indexes relations and save
    % the map point index, in C++ from 0, in Matlab from 1
    % indexList id from ZERO
    indexList = [];
    indexList(:,1) = all_ID-1;
    for i= 1:length(all_ID)
        indexList(i,2) = i-1;  % map point re-index from zero
    end
    dlmwrite(saveReIndexFile, indexList, 'precision', '%d');
    
    % update the visMatrix and save
    
    for i=0:length(visCells)-1
%         tic
%         disp(i);
        vis_point_ID_old = visCells{i+1};       

        % thought 3
        isInIndex = ismember(indexList(:,1), vis_point_ID_old);
        isSaved = find(isInIndex==1);
        newnew = indexList(:,2);
        vis_point_ID_new = newnew(isSaved);
        
        visCells_new{i+1} = vis_point_ID_new;
        
%         toc
    end
    
    % update the q-matrix-vector
    % weight is from ONE
    file_q = fopen(weightFile);
    weights = fscanf(file_q, '%d');
    new_weights = [];
    for i = 1:length(indexList)
        new_weights(i,:) = weights(indexList(i,1)+1);
    end
    dlmwrite(saveNewQFile, new_weights, 'precision', '%d');
    
    disp('Finished');
    
end


%         % visMatrix is from 
%         vis_point_ID_new = [];
%         remain_cnt = 1;      
        % thought 1
%         for j =1:length(vis_point_ID_old)
%             row_indexList = find(indexList(:,1) == (vis_point_ID_old(j)));
%             if size(row_indexList,1) == 1
%                 vis_point_ID_new(remain_cnt,:) = indexList(row_indexList,2);  % already from zero before this part 
%                 remain_cnt = remain_cnt + 1;
%             else  % zero matrix
%             end
%         end
%         % thought 2
%         vis_point_ID_old_inter = intersect(vis_point_ID_old, indexList(:,1));
%         for j = 1:length(vis_point_ID_old_inter)
% %             rowLine = find(indexList(:,1) == vis_point_ID_old_inter(j));
%             vis_point_ID_old_inter(j,:) = indexList(find(indexList(:,1) == vis_point_ID_old_inter(j)), 2);
%         end
%       

