totalNum = 1867290;

% epsilon_soft_100 = DP_loopCompress(0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/visMatrix/', '/home/yinhuan/mapModel/yq/rows_cut/DP_rows_cuts_100.txt', totalNum, 2050, '/home/yinhuan/mapModel/yq/rows_cut/DP_compress0.2_100/'); 
% 
% salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/DP_compress0.2_100/', totalNum );
%  
% min_cost_DP_100  = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/DP_compress0.2_100/', '/home/yinhuan/mapModel/yq/weightVector.txt', 0.1, epsilon_soft_100);


% sparse test
epsilon_soft_4_DP = DP_loopCompress(0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_rows_cuts_4.txt', totalNum, 2050, '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress0.2_4/'); 

salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress0.2_4/', totalNum );
 
min_cost_DP_4  = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress0.2_4/', '/home/yinhuan/mapModel/yq/weightVector.txt', 0.1, epsilon_soft_4_DP);


