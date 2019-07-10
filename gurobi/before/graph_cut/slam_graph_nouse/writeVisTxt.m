function [  ] = writeVisTxt( filesDir, saveAddress )
%WRITEVISTXT Summary of this function goes here
%   Detailed explanation goes here
        
    %  https://github.com/math314/cut-tree 
    %  roadID mapPointID (as a observe), or Gomory Hu tre cut
    
    fileExt = '*.txt';
    files = dir(fullfile(filesDir,fileExt)); 
    
    pathLength= length(files);
    
    saveFile = fopen(saveAddress, 'w');
    
    for i = 0:pathLength-1
        i
        fileCnt = num2str(i);
        fileName = [filesDir, fileCnt, '.txt'];     
        file_t = fopen(fileName);
        
        pointID = fscanf(file_t, '%d');
        fclose(file_t);    
        
        % from beginning to the end ?
        pointID = sort(pointID);
        
        % write edges between poses
        if i>0
           fprintf(saveFile,'%d ', i);
           fprintf(saveFile,'%d\n', i-1);    
        end
        
        for m = 1:length(pointID)
           % write pairs as edges  
           fprintf(saveFile,'%d ', i);
           fprintf(saveFile,'%d\n', (pointID(m) + pathLength)); % index from pathLength, not zero, changed for graph cut 

        end
            
    end
    fclose(saveFile);
    
end

