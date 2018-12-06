function [ compressIndex, time, epsilon_soft ] = section_compress( lamda, q_value, Dir, start, finish, totalNum, bValue, savedPointsFile)
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
   
    % deleted point equal to zero, for the iterations
    d_file_t = fopen(savedPointsFile);
    saved_points = fscanf(d_file_t, '%d');
    fclose(d_file_t);
    
    if isempty(saved_points)
        
        % the first time
        disp('Setting Params');
        % add frameNum of frame variables = epsilon, for soft constraint
        % add q_value and lamda_value, eye for diag-matrix
        model.A = sparse([visMatrix, eye(frameNum)]);
        model.obj = [q_value', lamda*ones(1,frameNum)];     

        bValueVector = bValue*ones(1, (finish-start)+1);  
    %     % change bvalus if the num of observed points under it, >=
    %     belowID = find(lenOfP < bValue);
    %     for i=1:length(belowID)
    %         bValueVector(belowID(i)) = lenOfP(belowID(i));
    %     end
        model.rhs = bValueVector;

        % multiple inequality
        symbol_1 = ['>'];
        model.sense = repmat(symbol_1, 1, frameNum);

        % type different
        x_type = 'B';
        epsilon_type = 'N';
        model.vtype = [repmat(x_type, 1, totalNum), repmat(epsilon_type, 1, frameNum)];
        model.modelsense = 'min';

    %     gurobi_write(model, 'sectionTest.lp');

        params.outputflag = 0;

        disp('Programming...');
        tic
        result = gurobi(model, params);
        time = toc
        compressIndex = find(result.x(1:totalNum) == 1);   % former ones
        epsilon_soft = result.x(totalNum+1:end);
        
    else
        
        % we have deleted some points, set them zeros
        disp('Setting Params');
        
        % visibility
        A1 = [visMatrix, eye(frameNum)];
        % setting equal zeros, only one equation is enough
        A2 = zeros(1, (totalNum+frameNum));
        for z = 1:length(saved_points)
            A2(1, saved_points(z)) =  1;
        end
        A2 = 1 - A2; % those not saved is ONE = ZERO
        model.A = sparse([A1; A2]);

        % objective function should not be changed
        model.obj = [q_value', lamda*ones(1,frameNum)];     

        % more b values = 0
        bValueVector = bValue*ones(1, (finish-start)+1); 
        zerosVector = zeros(1,1);
        model.rhs = [bValueVector, zerosVector];
        
        % multiple inequality
        symbol_1 = ['>'];
        symbol_2 = ['='];
        model.sense = [repmat(symbol_1, 1, frameNum), repmat(symbol_2, 1, 1)];

        % type different
        x_type = 'B';
        epsilon_type = 'N';
        model.vtype = [repmat(x_type, 1, totalNum), repmat(epsilon_type, 1, frameNum)];
        model.modelsense = 'min';

    %     gurobi_write(model, 'sectionTest.lp');

        params.outputflag = 0;
        
        disp('Programming...');
        tic
        result = gurobi(model, params);
        time = toc
        compressIndex = find(result.x(1:totalNum) == 1);   % former ones
        epsilon_soft = result.x(totalNum+1:end);
        
    end
    
end

    