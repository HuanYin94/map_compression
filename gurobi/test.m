function [result ] = test( )
%TEST Summary of this function goes here
%   Detailed explanation goes here

% test for ILP in 3D point cloud reduction

%     allZeros = zeros(6000, 500000);

disp('Generating')
% model.A = sparse(round(rand(6000,5000)));

% simulation
a = round(rand(200, 5000));
b = zeros(5800, 5000);
model.A = sparse([a; b]);

disp('Setting')
model.obj = ones(1,5000);    
% model.rhs = 2*ones(1,6000);

% simulation
aa = 2*ones(1, 200);
bb = zeros(1, 5800);
model.rhs = [aa,bb];

model.sense = '>';
model.vtype = 'B';
model.modelsense = 'min';

gurobi_write(model, 'test.lp');

params.outputflag = 0;

result = gurobi(model, params);

dlmwrite('result.txt', result.x);

end

 