totalNum = 1867290;


% [epsilon_soft_2_DP_test, time_DP_2_test] = DP_loopCompress(1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_rows_cuts_2_test.txt', totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_test/'); 
% 
% point_num_DP_2_test = salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_test/', totalNum );
%  
% min_cost_DP_2_test = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_test/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_2_DP_test);


%% sparse test
% two nums: section_num search_domain 
% [epsilon_soft_5_DP_80_max, time_DP_5_80_max] = DP_loopCompress(1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_rows_cuts_5_80_max.txt', totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_5_80_max/'); 
% 
% point_num_DP_5_80_max = salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_5_80_max/', totalNum );
%  
% min_cost_DP_5_80_max = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_5_80_max/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_5_DP_80_max);

% 
% 
% [epsilon_soft_2_DP_50, time_DP_2_50] = DP_loopCompress(1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_rows_cuts_2_50.txt', totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_50/'); 
% 
% point_num_DP_2_50 = salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_50/', totalNum );
%  
% min_cost_DP_2_50 = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_50/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_2_DP_50);
% 
% 
% 
% 
% [epsilon_soft_2_DP_20, time_DP_2_20] = DP_loopCompress(1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_rows_cuts_2_20.txt', totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_20/'); 
% 
% point_num_DP_2_20 = salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_20/', totalNum );
%  
% min_cost_DP_2_20 = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_20/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_2_DP_20);
% 
% 
% 
% [epsilon_soft_2_DP_10, time_DP_2_10] = DP_loopCompress(1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_rows_cuts_2_10.txt', totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_10/'); 
% 
% point_num_DP_2_10 = salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_10/', totalNum );
%  
% min_cost_DP_2_10 = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_2_10/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_2_DP_10);

%% 11.30



[epsilon_soft_5_DP_80_max, time_DP_5_80_max] = DP_loopCompress(1, '/home/yinhuan/mapModel/yq/weightVector.txt', '/home/yinhuan/mapModel/yq/rows_cut/sparse/visMatrix/', '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_rows_cuts_5_80_max.txt', totalNum, 1000, '/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_5_80_max/'); 

point_num_DP_5_80_max = salientNumCnt('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_5_80_max/', totalNum );
 
min_cost_DP_5_80_max = get_min_cost('/home/yinhuan/mapModel/yq/rows_cut/sparse/DP_compress_5_80_max/', '/home/yinhuan/mapModel/yq/weightVector.txt', 1, epsilon_soft_5_DP_80_max);