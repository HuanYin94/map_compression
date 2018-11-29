function [ node_list ] = getPoseGraph_sparse( visFilesDir, windowLength,  sparse_separate, saveAddress  )
%GETPOSEGRAPH_SPARSE Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(visFilesDir,fileExt)); 
    
    pathLength= length(files);
    
    % the nodes we need to save 
    node_list = [];
    cnt = 1;
    for i=0:sparse_separate:pathLength-1
        node_list(cnt, :) = i;
        cnt =cnt +1;
    end     
    
    % build the graph from the vis matrix and the sparse nodes
    % different from the previous one, get the window one by one :)
    for i=1:length(node_list)
        
        % save one by one
        saveFile = fopen(saveAddress, 'a');
        
        % open the first file
        fileName_i = [visFilesDir, num2str(node_list(i)), '.txt'];     
        file_t_i = fopen(fileName_i);
        pointID_i = fscanf(file_t_i, '%d');
                                                                                                                                                                                                                                            
        for j=i+1:i+windowLength
            
            if j > length(node_list)
                % do nothing
            else
                fileName_j = [visFilesDir, num2str(node_list(j)), '.txt'];     
                file_t_j= fopen(fileName_j);
                pointID_j = fscanf(file_t_j, '%d');

                samer = intersect(pointID_i, pointID_j);

                fprintf(saveFile,'%d ', i-1);   % save from zero                     
                fprintf(saveFile,'%d ', j-1);   % minus one
                fprintf(saveFile,'%d\n', length(samer));
            end
            
        end
        
        fclose all;
   
    end
    
    
    
end

