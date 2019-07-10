% 0.01

totalNum = 1867290;

loopCompress(0.1, '/home/huyin/weightVector.txt', '/home/huyin/visMatrix/', 50, totalNum, 200, '/home/huyin/gurobi_compress_0.01/');

ratio = salientNumCnt( '/home/huyin/gurobi_compress_0.01/', totalNum ) / totalNum