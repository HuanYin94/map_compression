totalNum = 1867290;



% Uniform_loopCompress( 0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/visMatrix/', 100, totalNum, 2000, '/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress0.2_100/' )
DP_loopCompress(0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/visMatrix/', '/home/yinhuan/mapModel/yq/rows_cut/DP_rows_cuts_100.txt', totalNum, 2050, '/home/yinhuan/mapModel/yq/rows_cut/DP_compress0.2_100/');
 salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/DP_compress0.2_100/', totalNum );
 
 



