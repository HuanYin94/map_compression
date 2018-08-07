%% YQ Dataset

% same device as the random forest, no server, we use

% Laptop

totalNum = 1866088;

loopCompress(0.1, '/home/yh/mapModel/2018/08.03/weightVector.txt', '/home/yh/mapModel/2018/08.03/visMatrix/', 50, totalNum, 1550, '/home/yh/mapModel/2018/08.03/gurobi_compress_0.2/');

ratio = salientNumCnt( '/home/yh/mapModel/2018/08.03/gurobi_compress_0.2/', totalNum ) / totalNum

%% KITTI Dataset