function [  ] = save_savedPoints( saveResultsDir_0,  saveDeletedFile)
%SAVE_DELETEDPOINTS Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(saveResultsDir_0,fileExt));  
    
    % get the union cost 
    saved_ID = [];
    for i = 0:length(files)-1
        
        fileCnt = num2str(i);
        fileName = [saveResultsDir_0, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        point_ID = fscanf(file_t, '%d');
        fclose(file_t);
        
        saved_ID = union(saved_ID, point_ID);
    end
    
    dlmwrite(saveDeletedFile, saved_ID, 'precision', '%d');

end

