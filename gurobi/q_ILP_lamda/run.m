%% YQ Dataset

% Laptop

% totalNum = 2769950;
%  loopCompress(0.1, '/home/yh/mapModel/2018/08.03/weightVector.txt', '/home/yh/mapModel/2018/08.03/visMatrix/', 100, totalNum, 300, '/home/yh/mapModel/2018/08.03/gurobi_compress_0.05/');
% % loopCompress(0.1, '/home/yh/mapModel/2018/08.03/weightVector.txt', '/home/yh/mapModel/2018/08.03/visMatrix/', 50, 500000, 350, '/home/yh/mapModel/2018/08.03/gurobi_compress_0.1/');
% % loopCompress(0.1, '/home/yh/mapModel/2018/08.03/weightVector.txt', '/home/yh/mapModel/2018/08.03/visMatrix/', 50, 500000, 350, '/home/yh/mapModel/2018/08.03/gurobi_compress_0.2/');
% 
% ratio = salientNumCnt( '/home/yh/mapModel/2018/08.03/gurobi_compress_0.05/', totalNum ) / totalNum

% server

totalNum = 2769950;
loopCompress(0.1, '/home/huyin/weightVector.txt', '/home/huyin/visMatrix/', 100, totalNum, 600, '/home/huyin/gurobi_compress/');

ratio = salientNumCnt( '/home/huyin/gurobi_compress/', totalNum ) / totalNum

%% KITTI Dataset