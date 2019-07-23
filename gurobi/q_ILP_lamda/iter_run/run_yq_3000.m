% params
totalNum = 6159264;
bValue = 3000;
lamda = 0.1;
qFile = '/home/yinhuan/Data/mapModel/yq/weightVector.txt';
visFilesDir = '/home/yinhuan/Data/mapModel/yq/visMatrix/';
maxQ = 179;
minQ = 1;

splitLength_0 = 50;
saveResultsDir_0 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/0/';
saveReIndexFile_0 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/index/0.txt';
saveNewQFile_0 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/weight/0.txt';

splitLength_1 = 100;
saveResultsDir_1 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/1/';
saveReIndexFile_1 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/index/1.txt';
saveNewQFile_1 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/weight/1.txt';

splitLength_2 = 200;
saveResultsDir_2 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/2/';
saveReIndexFile_2 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/index/2.txt';
saveNewQFile_2 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/weight/2.txt';

splitLength_3 = 400;
saveResultsDir_3 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/3/';
saveReIndexFile_3 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/index/3.txt';
saveNewQFile_3 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/weight/3.txt';

splitLength_4 = 800; 
saveResultsDir_4 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/4/';
saveReIndexFile_4 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/index/4.txt';
saveNewQFile_4 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/weight/4.txt';

splitLength_5 = 1600;
saveResultsDir_5 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/5/';
saveReIndexFile_5 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/index/5.txt';
saveNewQFile_5 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/weight/5.txt';

splitLength_6 = 3200;
saveResultsDir_6 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/6/';
saveReIndexFile_6 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/index/6.txt';
saveNewQFile_6 = '/home/yinhuan/Data/mapModel/yq/iter_b_3000/weight/6.txt';

%% iteration of optimization

visCells_origin = fromVisDirToCells(visFilesDir);

[ epsilon_soft_0, time_sum_0  ] = Uniform_loopCompress_Cells( lamda, qFile, visCells_origin, splitLength_0, totalNum, bValue, saveResultsDir_0 );
min_cost_0 = get_min_cost(minQ, maxQ, saveResultsDir_0, qFile, lamda, epsilon_soft_0);
[totalNum_0, visCells_0] = deleteZeroPoints_Cells( qFile, visCells_origin, saveResultsDir_0, saveReIndexFile_0, saveNewQFile_0 );

[ epsilon_soft_1, time_sum_1  ] = Uniform_loopCompress_Cells( lamda, saveNewQFile_0, visCells_0, splitLength_1, totalNum_0, bValue, saveResultsDir_1 );
min_cost_1 = get_min_cost(minQ, maxQ, saveResultsDir_1, saveNewQFile_0, lamda, epsilon_soft_1);
[totalNum_1, visCells_1] = deleteZeroPoints_Cells( saveNewQFile_0, visCells_0, saveResultsDir_1, saveReIndexFile_1, saveNewQFile_1 );

[ epsilon_soft_2, time_sum_2  ] = Uniform_loopCompress_Cells( lamda, saveNewQFile_1, visCells_1, splitLength_2, totalNum_1, bValue, saveResultsDir_2 );
min_cost_2 = get_min_cost(minQ, maxQ, saveResultsDir_2, saveNewQFile_1, lamda, epsilon_soft_2);
[totalNum_2, visCells_2] = deleteZeroPoints_Cells( saveNewQFile_1, visCells_1, saveResultsDir_2, saveReIndexFile_2, saveNewQFile_2 );





[ epsilon_soft_3, time_sum_3  ] = Uniform_loopCompress_Cells( lamda, saveNewQFile_2, visCells_2, splitLength_3, totalNum_2, bValue, saveResultsDir_3 );
min_cost_3 = get_min_cost(minQ, maxQ, saveResultsDir_3, saveNewQFile_2, lamda, epsilon_soft_3);
[totalNum_3, visCells_3] = deleteZeroPoints_Cells( saveNewQFile_2, visCells_2, saveResultsDir_3, saveReIndexFile_3, saveNewQFile_3 );

[ epsilon_soft_4, time_sum_4  ] = Uniform_loopCompress_Cells( lamda, saveNewQFile_3, visCells_3, splitLength_4, totalNum_3, bValue, saveResultsDir_4 );
min_cost_4 = get_min_cost(minQ, maxQ, saveResultsDir_4, saveNewQFile_3, lamda, epsilon_soft_4);
[totalNum_4, visCells_4] = deleteZeroPoints_Cells( saveNewQFile_3, visCells_3, saveResultsDir_4, saveReIndexFile_4, saveNewQFile_4 );




[ epsilon_soft_5, time_sum_5  ] = Uniform_loopCompress_Cells( lamda, saveNewQFile_4, visCells_4, splitLength_5, totalNum_4, bValue, saveResultsDir_5 );
min_cost_5 = get_min_cost(minQ, maxQ, saveResultsDir_5, saveNewQFile_4, lamda, epsilon_soft_5);
[totalNum_5, visCells_5] = deleteZeroPoints_Cells( saveNewQFile_4, visCells_4, saveResultsDir_5, saveReIndexFile_5, saveNewQFile_5 );

[ epsilon_soft_6, time_sum_6  ] = Uniform_loopCompress_Cells( lamda, saveNewQFile_5, visCells_5, splitLength_6, totalNum_5, bValue, saveResultsDir_6 );
min_cost_6 = get_min_cost(minQ, maxQ, saveResultsDir_6, saveNewQFile_5, lamda, epsilon_soft_6);
[totalNum_6, visCells_6] = deleteZeroPoints_Cells( saveNewQFile_5, visCells_5, saveResultsDir_6, saveReIndexFile_6, saveNewQFile_6 );







%% save the results

compressIndex_5 = anal_reindex_last(saveResultsDir_6, saveReIndexFile_5);
compressIndex_4 = anal_reindex_middle(compressIndex_5, saveReIndexFile_4);
compressIndex_3 = anal_reindex_middle(compressIndex_4, saveReIndexFile_3);
compressIndex_2 = anal_reindex_middle(compressIndex_3, saveReIndexFile_2);
compressIndex_1 = anal_reindex_middle(compressIndex_2, saveReIndexFile_1);
compressIndex_0 = anal_reindex_middle(compressIndex_1, saveReIndexFile_0);

dlmwrite('/home/yinhuan/Data/mapModel/yq/iter_b_3000/compressIndex_final.txt', compressIndex_0, 'precision', '%d');








