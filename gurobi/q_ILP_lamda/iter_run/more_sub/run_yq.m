% % params
% totalNum = 1867290;
% bValue = 1000;
lamda = 0.001;
% qFile = '/home/yinhuan/mapModel/yq/weightVector.txt';
% visFilesDir = '/home/yinhuan/mapModel/yq/visMatrix/';
% 
% savedFile_init = '/home/yinhuan/mapModel/yq/iter/blank.txt';
% 
% splitLength_0 = 50;
% saveResultsDir_0 = '/home/yinhuan/mapModel/yq/iter/results/0/';
% savedFile_0 = '/home/yinhuan/mapModel/yq/iter/saved_0.txt';
% 
% splitLength_1 = 100;
% saveResultsDir_1 = '/home/yinhuan/mapModel/yq/iter/results/1/';
% savedFile_1 = '/home/yinhuan/mapModel/yq/iter/saved_1.txt';
% 
% splitLength_2 = 400;
% saveResultsDir_2 = '/home/yinhuan/mapModel/yq/iter/results/2/';
% savedFile_2 = '/home/yinhuan/mapModel/yq/iter/saved_2.txt';
% 
% splitLength_3 = 1200;
% saveResultsDir_3 = '/home/yinhuan/mapModel/yq/iter/results/3/';
% savedFile_3 = '/home/yinhuan/mapModel/yq/iter/saved_3.txt';
% 
% splitLength_4 = 2400;
% saveResultsDir_4 = '/home/yinhuan/mapModel/yq/iter/results/4/';
% savedFile_4 = '/home/yinhuan/mapModel/yq/iter/saved_4.txt';
% 
% splitLength_5 = 4924;
% saveResultsDir_5 = '/home/yinhuan/mapModel/yq/iter/results/5/';
% savedFile_5 = '/home/yinhuan/mapModel/yq/iter/saved_5.txt';
% 
% %% iteration of optimization
% 
% [ epsilon_soft_0, time_sum_0  ] = Uniform_loopCompress( lamda, qFile, visFilesDir, splitLength_0, totalNum, bValue, saveResultsDir_0,  savedFile_init);
% save_savedPoints( saveResultsDir_0,  savedFile_0);
% [ min_cost_0, totalNum_0  ] = get_min_cost( saveResultsDir_0, qFile, lamda, epsilon_soft_0 );

[ epsilon_soft_1, time_sum_1  ] = Uniform_loopCompress( lamda, qFile, visFilesDir, splitLength_1, totalNum, bValue, saveResultsDir_1,  savedFile_0);
save_savedPoints( saveResultsDir_1,  savedFile_1);
[ min_cost_1, totalNum_1  ] = get_min_cost( saveResultsDir_1, qFile, lamda, epsilon_soft_1 );

[ epsilon_soft_2, time_sum_2  ] = Uniform_loopCompress( lamda, qFile, visFilesDir, splitLength_2, totalNum, bValue, saveResultsDir_2,  savedFile_1);
save_savedPoints( saveResultsDir_2,  savedFile_1);
[ min_cost_2, totalNum_2  ] = get_min_cost( saveResultsDir_2, qFile, lamda, epsilon_soft_2 );

[ epsilon_soft_3, time_sum_3  ] = Uniform_loopCompress( lamda, qFile, visFilesDir, splitLength_3, totalNum, bValue, saveResultsDir_3,  savedFile_2);
save_savedPoints( saveResultsDir_3,  savedFile_1);
[ min_cost_3, totalNum_3  ] = get_min_cost( saveResultsDir_3, qFile, lamda, epsilon_soft_3 );

[ epsilon_soft_4, time_sum_4  ] = Uniform_loopCompress( lamda, qFile, visFilesDir, splitLength_4, totalNum, bValue, saveResultsDir_4,  savedFile_3);
save_savedPoints( saveResultsDir_4,  savedFile_1);
[ min_cost_4, totalNum_4  ] = get_min_cost( saveResultsDir_4, qFile, lamda, epsilon_soft_4 );

[ epsilon_soft_5, time_sum_5  ] = Uniform_loopCompress( lamda, qFile, visFilesDir, splitLength_5, totalNum, bValue, saveResultsDir_5,  savedFile_4);
save_savedPoints( saveResultsDir_5,  savedFile_1);
[ min_cost_5, totalNum_5  ] = get_min_cost( saveResultsDir_5, qFile, lamda, epsilon_soft_5 );












