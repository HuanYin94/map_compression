totalNum = 1867290;
% 

% epsilon_soft_100 = Uniform_loopCompress( 0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/visMatrix/', 100, totalNum, 2000, '/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress0.2_100/' );
% 
% salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress0.2_100/', totalNum );
% 
% min_cost_Uniform_100  = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/DP_compress0.2_100/', '/home/yinhuan/mapModel/yq/weightVector.txt', 0.1, epsilon_soft_100);


% sparse test

 epsilon_soft_4_Uni = Uniform_loopCompress( 0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', 4, totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress0.2_4/' );

salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress0.2_4/', totalNum );

min_cost_Uniform_4  = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress0.2_4/', '/home/yinhuan/mapModel/yq/weightVector.txt', 0.1, epsilon_soft_4_Uni);
 



