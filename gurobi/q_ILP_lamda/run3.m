% on the UTS server

totalNum = 1867290;

loopCompress(0.1, '/home/huyin/weightVector.txt', '/home/huyin/visMatrix/', 50, totalNum, 666, '/home/huyin/gurobi_compress_0.05/');

ratio = salientNumCnt( '/home/huyin/gurobi_compress_0.05/', totalNum ) / totalNum