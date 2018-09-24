% 0.05 700
 
% 0.1 1200

% 0.2 2200

totalNum = 447026;

loopCompress(0.1, '/home/yinhuan/compression_exps/park/weightVector.txt', '/home/yinhuan/compression_exps/park/visMatrix/', 50, totalNum, 700, '/home/yinhuan/compression_exps/park/gurobi_compress_0.05/');

ratio = salientNumCnt( '/home/yinhuan/compression_exps/park/gurobi_compress_0.05/', totalNum ) / totalNum