function [  ] = salientNumCnt( filesDir, totalNum )
%SALIENTNUMCNT Summary of this function goes here
%   Detailed explanation goes here
    
    allPoints = zeros(1,totalNum);

    fileExt = '*.txt';
    
    files = dir(fullfile(filesDir,fileExt));  
        
    for i=0:length(files)-1
        i
        fileCnt = num2str(i);
        fileName = [filesDir, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        pointID = fscanf(file_t, '%d');
        fclose(file_t);
        
        if length(pointID) > 0
            allPoints(sub2ind(size(allPoints), ones(1, length(pointID)), (pointID)')) = 1;
        end

    end
    
    disp('salient predicted:');
    disp(sum(allPoints));
        
end

