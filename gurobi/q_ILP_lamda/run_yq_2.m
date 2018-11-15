totalNum = 1867290;
% 
% 
% 
% Uniform_loopCompress( 0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/visMatrix/', 100, totalNum, 2000, '/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress0.2_100/' )
%  salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress0.2_100/', totalNum );

Uniform_loopCompress( 0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/visMatrix/', 80, totalNum, 2000, '/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress0.2_80/' )
 salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress0.2_80/', totalNum );

min_cost_Uniform_80  = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/DP_compress0.2_80/', '/home/yinhuan/mapModel/yq/weightVector.txt');

 
 



