% 0.05 700
 
% 0.1 1200

% 0.2 2200

%0.02

totalNum = 447026;

loopCompress(0.1, '/home/yinhuan/compression_exps/park/weightVector.txt', '/home/yinhuan/compression_exps/park/visMatrix/', 50, totalNum, 350, '/home/yinhuan/compression_exps/park/gurobi_compress_0.02/');

ratio = salientNumCnt( '/home/yinhuan/compression_exps/park/gurobi_compress_0.02/', totalNum ) / totalNum