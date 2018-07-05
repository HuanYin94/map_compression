function [ compressIndex ] = section( Dir, start, finish, totalNum, bValue )
%SECTIONTEST Summary of this function goes here
%   Detailed explanation goes here
    
    disp('Loop for Generating A');
    
    % a large sparse convisibility matrix
    visMatrix = zeros(finish-start+1, totalNum);
    
    rowCnt=1;
    for i=start:1:finish
        fileCnt = num2str(i);
        fileName = [Dir, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        pointID = fscanf(file_t, '%d');
        fclose(file_t);
        
        % no loop, sub2ind
       visMatrix(sub2ind(size(visMatrix), rowCnt*ones(1, length(pointID)), (pointID+1)')) = 1;
        
        rowCnt = rowCnt + 1; 
    end
   
    disp('Setting Params');
    model.A = sparse(visMatrix);
    model.obj = ones(1,totalNum);    
    model.rhs = bValue*ones(1, (finish-start)+1);
    
    model.sense = '>';
    model.vtype = 'B';
    model.modelsense = 'min';
    
%     gurobi_write(model, 'sectionTest.lp');

    params.outputflag = 0;

    result = gurobi(model, params);
    
    compressIndex = find(result.x == 1);
    
end

    