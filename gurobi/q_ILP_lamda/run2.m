totalNum = 1867290;

loopCompress(0.1, '/home/yh/mapModel/2018/08.03/weightVector.txt', '/home/yh/mapModel/2018/08.03/visMatrix/', 50, totalNum, 1500, '/home/yh/mapModel/2018/08.03/gurobi_compress_0.2/');

ratio = salientNumCnt( '/home/yh/mapModel/2018/08.03/gurobi_compress_0.2/', totalNum ) / totalNum