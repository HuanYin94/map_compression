function [ result ] = sectionTest( Dir, start, finish )
%SECTIONTEST Summary of this function goes here
%   Detailed explanation goes here

    % preparation
    totalNum = 500000;
    bValue = 50;
    
    disp('Loop for Generating A');
    
    visMatrix = [];
    
    for i=start:1:finish
        fileCnt = num2str(i);
        fileName = [Dir, fileCnt, '.txt'];
        
        file_t = fopen(fileName);
        pointID = fscanf(file_t, '%d');
       
        aRow = zeros(1, totalNum);
        for j=1:length(pointID)
            aRow(1, pointID(j)+1) = 1;   % from zero to one, c++ to Matlab
        end
        visMatrix = [visMatrix; aRow];
    end
   
    disp('Setting Params');
    model.A = sparse(visMatrix);
    model.obj = ones(1,totalNum);    
    model.rhs = bValue*ones(1, (finish-start)+1);
    
    model.sense = '>';
    model.vtype = 'B';
    model.modelsense = 'min';
    
    gurobi_write(model, 'sectionTest.lp');

    params.outputflag = 1;

    result = gurobi(model, params);
    
end

    