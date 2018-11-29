function [  ] = sparse_visMatrix(node_list, visMatrix_Dir_sparse )
%SPARSE_VISMATRIX Summary of this function goes here
%   Detailed explanation goes here

    fileExt = '*.txt';
    files = dir(fullfile(visMatrix_Dir_sparse,fileExt)); 
    
    % jump the zero(first) file
    
    cnt = 1;
    for i = 1:length(files)-1
        
        fileName = [visMatrix_Dir_sparse, num2str(i), '.txt'];
        
        if any(node_list == i)
            fileName_new = [visMatrix_Dir_sparse, num2str(cnt), '.txt'];
            movefile(fileName, fileName_new);
            delete(fileName);
            cnt =  cnt + 1;
        else
            delete(fileName);
        end
        
    end
    
end

