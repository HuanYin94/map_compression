totalNum = 2769950;
loopCompress(0.2, '/home/huyin/weightVector.txt', '/home/huyin/visMatrix/', 50, totalNum, 500, '/home/huyin/gurobi_compress_4/');

ratio = salientNumCnt( '/home/huyin/gurobi_compress/', totalNum ) / totalNum