function [result ] = test( )
%TEST Summary of this function goes here
%   Detailed explanation goes here

% test for ILP in 3D point cloud reduction

%     allZeros = zeros(6000, 500000);

disp('Generating')
model.A = sparse(round(rand(1000,100000)));
disp('Setting')
model.obj = ones(1,100000);    
model.rhs = 2*ones(1,1000);
model.sense = '>';
model.vtype = 'B';
model.modelsense = 'min';

gurobi_write(model, 'test.lp');

params.outputflag = 0;

result = gurobi(model, params);

dlmwrite('result.txt', result.x);

end

 