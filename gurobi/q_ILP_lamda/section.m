function [ compressIndex ] = section( lamda, q_value, Dir, start, finish, totalNum, bValue )
%SECTIONTEST Summary of this function goes here
%   Detailed explanation goes here
    
    disp('Loop for Generating A');
 
    % pose num of selected: 
    frameNum = finish-start+1;
    % points num:           totalNum
    
    % a large sparse convisibility matrix
    visMatrix = zeros(frameNum, totalNum);
    
    rowCnt=1;
    lenOfP = [];
    for i=start:1:finish
        fileCnt = num2str(i);
        fileName = [Dir, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        pointID = fscanf(file_t, '%d');
        fclose(file_t);
        
        lenOfP(rowCnt) = length(pointID);
        
        % no loop, sub2ind
       visMatrix(sub2ind(size(visMatrix), rowCnt*ones(1, lenOfP(rowCnt)), (pointID+1)')) = 1;
        
        rowCnt = rowCnt + 1; 
    end
   
    disp('Setting Params');
    % add frameNum of frame variables
    % add q_value and lamda_value, eye for diag-matrix
    model.A = sparse([visMatrix, eye(frameNum)]);
    model.obj = [q_value', lamda*ones(1,frameNum)];    
    
    bValueVector = bValue*ones(1, (finish-start)+1);  
    % change bvalus if the num of observed points under it, >=
    belowID = find(lenOfP < bValue);
    for i=1:length(belowID)
        bValueVector(belowID(i)) = lenOfP(belowID(i));
    end
    model.rhs = bValueVector;
    
    % multiple inequality
    symbol = ['>'];
    model.sense = repmat(symbol, 1, frameNum);
    
    % type different
    x_type = 'B';
    zeta_type = 'N';
    model.vtype = [repmat(x_type, 1, totalNum), repmat(zeta_type, 1, frameNum)];
    model.modelsense = 'min';
    
%     gurobi_write(model, 'sectionTest.lp');

    params.outputflag = 0;
    
    disp('Programming...');
    tic
    result = gurobi(model, params);
    time = toc
    compressIndex = find(result.x(1:totalNum) == 1);   % former ones
    
end

    