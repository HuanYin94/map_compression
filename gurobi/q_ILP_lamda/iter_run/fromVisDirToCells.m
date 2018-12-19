function [ visCells ] = fromVisDirToCells( visFilesDir )
%FROMVISDIRTOCELLS Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    vis_files = dir(fullfile(visFilesDir,fileExt)); 
    
    for i = 0:length(vis_files)-1
        fileCnt = num2str(i);
        old_fileName = [visFilesDir, fileCnt, '.txt'];
        file_old = fopen(old_fileName);
        vis_point_ID_old = fscanf(file_old, '%d');        
        
        visCells{i+1} = vis_point_ID_old;

    end

end

