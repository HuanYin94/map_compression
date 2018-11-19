totalNum = 1867290;
% 

% epsilon_soft_100 = Uniform_loopCompress( 0.1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/visMatrix/', 100, totalNum, 2000, '/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress0.2_100/' );
% 
% salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/Uniform_compress0.2_100/', totalNum );
% 
% min_cost_Uniform_100  = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/DP_compress0.2_100/', '/home/yinhuan/mapModel/yq/weightVector.txt', 0.1, epsilon_soft_100);


%%

% sparse test

%  epsilon_soft_2_Uni = Uniform_loopCompress( 1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', 2, totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress0.2_2/' );
% 
% salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress0.2_2/', totalNum );
% 
% min_cost_Uniform_2  = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress0.2_2/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_2_Uni);
 


 epsilon_soft_5_Uni = Uniform_loopCompress( 1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', 5, totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress0.2_5/' );

point_num_Uni_5 = salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress0.2_5/', totalNum );

min_cost_Uniform_5 = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/Uniform_compress0.2_5/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_5_Uni);


