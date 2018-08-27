%% YQ Dataset

%same device as the random forest, no server, we use

% Laptop

totalNum = 1867290;
loopCompress(0.1, '/home/yh/mapModel/2018/08.03/weightVector.txt', '/home/yh/mapModel/2018/08.03/visMatrix/', 50, totalNum, 900, '/home/yh/test/');

% loopCompress(0.1, '/home/yh/mapModel/2018/08.03/weightVector.txt', '/home/yh/mapModel/2018/08.03/visMatrix/', 50, totalNum, 900, '/home/yh/mapModel/2018/08.03/gurobi_compress_0.1/');

% ratio = salientNumCnt( '/home/yh/mapModel/2018/08.03/gurobi_compress_0.1/', totalNum ) / totalNum

%% KITTI Dataset
% 
% totalNum = 1410661;
% 
% loopCompress(0.1, '/home/yh/mapModel/2018/08.02/weightVector.txt', '/home/yh/mapModel/2018/08.02/visMatrix/', 50, totalNum, 6250, '/home/yh/mapModel/2018/08.02/gurobi_compress_0.2/');
% 
% ratio = salientNumCnt( '/home/yh/mapModel/2018/08.02/gurobi_compress_0.2/', totalNum ) / totalNum