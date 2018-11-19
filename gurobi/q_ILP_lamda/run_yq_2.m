totalNum = 1867290;
% 

% epsilon_soft_100 = Uniform_loopCompress( 0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/visMatrix/', 100, totalNum, 2000, '/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress_100/' );
% 
% salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress_100/', totalNum );
% 
% min_cost_Uniform_100  = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/DP_compress_100/', '/home/yinhuan/mapModel/yq/weightVector.txt', 0.1, epsilon_soft_100);


%%

% sparse test

 [epsilon_soft_1_Uni, time_Uni_1] = Uniform_loopCompress( 1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', 1, totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress_1/' );

pointNum_1 = salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress_1/', totalNum );

min_cost_Uniform_1 = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress_1/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_1_Uni);
 


[epsilon_soft_5_Uni, time_Uni_5] = Uniform_loopCompress( 1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', 5, totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress_5/' );

point_num_Uni_5 = salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress_5/', totalNum );

min_cost_Uniform_5 = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress_5/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_5_Uni);


