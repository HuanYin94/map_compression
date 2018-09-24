% 0.2

totalNum = 447026;

loopCompress(0.1, '/home/yinhuan/compression_exps/park/weightVector.txt', '/home/yinhuan/compression_exps/park/visMatrix/', 50, totalNum, 800, '/home/yinhuan/compression_exps/park/gurobi_compress_0.2/');

ratio = salientNumCnt( '/home/yinhuan/compression_exps/park/gurobi_compress_0.2/', totalNum ) / totalNum