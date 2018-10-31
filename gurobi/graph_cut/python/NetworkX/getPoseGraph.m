function [ ] = getPoseGraph( filesDir, windowLength, saveAddress )
%GETPOSEGRAPH Summary of this function goes here
%   Detailed explanation goes here
    
    fileExt = '*.txt';
    files = dir(fullfile(filesDir,fileExt)); 
    
    pathLength= length(files);
    
    saveFile = fopen(saveAddress, 'w');
        
    for i=0:pathLength-1
        i
        
        fileName_i = [filesDir, num2str(i), '.txt'];     
        file_t_i = fopen(fileName_i);
        pointID_i = fscanf(file_t_i, '%d');
        
        if i==0
            % save
            cnt = 1;
            for j=i+1:i+windowLength
            
                fileName_j = [filesDir, num2str(j), '.txt'];     
                file_t_j = fopen(fileName_j);
                pointID_j = fscanf(file_t_j, '%d');

                samer = intersect(pointID_i, pointID_j);

                fprintf(saveFile,'%d ', i);                     
                fprintf(saveFile,'%d ', j);
                fprintf(saveFile,'%d\n', length(samer));

                % build thw window, no matrix
                windowID{cnt} = pointID_j;
                cnt = cnt + 1;
            end
            cnt = cnt - 1;           
            continue;
        end
       
        % delete the first itself
        windowID(1)=[];
        
        if (i+windowLength) >= pathLength
           
        else
        % read a new txt
            fileName_j = [filesDir, num2str(i+windowLength), '.txt'];     
            file_t_j = fopen(fileName_j);
            pointID_j = fscanf(file_t_j, '%d');
            windowID{cnt} = pointID_j;
        end
        
        for j = i+1:i+windowLength
            
            if j >= pathLength
                continue;
            end
            
            rowLine = j-i;
            
            samer = intersect(pointID_i, windowID{rowLine});
            
            % no common see
            if isempty(samer)
               continue; 
            end
            
            fprintf(saveFile,'%d ', i);
            fprintf(saveFile,'%d ', j);
            fprintf(saveFile,'%d\n', length(samer));
            
        end
                
    end
    

end

