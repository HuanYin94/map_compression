totalNum = 2769950;
loopCompress(1.0, '/home/huyin/weightVector.txt', '/home/huyin/visMatrix/', 50, totalNum, 1000, '/home/huyin/gurobi_compress_2/');

ratio = salientNumCnt( '/home/huyin/gurobi_compress/', totalNum ) / totalNum