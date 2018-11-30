% params
totalNum = 1867290;
bValue = 1000;
lamda = 1;
qFile = '/home/yh/mapModel/2018/08.03/weightVector.txt';
visFilesDir_0 = '/home/yh/mapModel/2018/08.03/visMatrix/';

splitLength_0 = 50;
saveResultsDir_0 = '/home/yh/mapModel/2018/11.29/0/results/';
saveNewVisDir_0 = '/home/yh/mapModel/2018/11.29/0/visMatrix/';
saveReIndexFile_0 = '/home/yh/mapModel/2018/11.29/index/0.txt';
saveNewQFile_0 = '/home/yh/mapModel/2018/11.29/weight/0.txt';

splitLength_1 = 100;
visFilesDir_1 = saveNewVisDir_0;
saveResultsDir_1 = '/home/yh/mapModel/2018/11.29/1/results/';
saveNewVisDir_1 = '/home/yh/mapModel/2018/11.29/1/visMatrix/';
saveReIndexFile_1 = '/home/yh/mapModel/2018/11.29/index/1.txt';
saveNewQFile_1 = '/home/yh/mapModel/2018/11.29/weight/1.txt';

splitLength_2 = 500;
visFilesDir_2 = saveNewVisDir_1;
saveResultsDir_2 = '/home/yh/mapModel/2018/11.29/2/results/';
saveNewVisDir_2 = '/home/yh/mapModel/2018/11.29/2/visMatrix/';
saveReIndexFile_2 = '/home/yh/mapModel/2018/11.29/index/2.txt';
saveNewQFile_2 = '/home/yh/mapModel/2018/11.29/weight/2.txt';

splitLength_3 = 1000;
visFilesDir_3 = saveNewVisDir_2;
saveResultsDir_3 = '/home/yh/mapModel/2018/11.29/3/results/';
saveNewVisDir_3 = '/home/yh/mapModel/2018/11.29/3/visMatrix/';
saveReIndexFile_3 = '/home/yh/mapModel/2018/11.29/index/3.txt';
saveNewQFile_3 = '/home/yh/mapModel/2018/11.29/weight/3.txt';


%% iteration of optimization

[ epsilon_soft_0, time_sum_0  ] = Uniform_loopCompress( lamda, qFile, visFilesDir_0, splitLength_0, totalNum, bValue, saveResultsDir_0 );
min_cost_0 = get_min_cost(saveResultsDir_0, qFile, lamda, epsilon_soft_0);
totalNum = deleteZeroPoints( qFile, visFilesDir_0, saveResultsDir_0, saveNewVisDir_0, saveReIndexFile_0, saveNewQFile_0 );

[ epsilon_soft_1, time_sum_1  ] = Uniform_loopCompress( lamda, saveNewQFile_0, visFilesDir_1, splitLength_1, totalNum, bValue, saveResultsDir_1 );
min_cost_1 = get_min_cost(saveResultsDir_1, saveNewQFile_0, lamda, epsilon_soft_1);
totalNum = deleteZeroPoints( saveNewQFile_0, visFilesDir_1, saveResultsDir_1, saveNewVisDir_1, saveReIndexFile_1, saveNewQFile_1 );

[ epsilon_soft_2, time_sum_2  ] = Uniform_loopCompress( lamda, saveNewQFile_1, visFilesDir_2, splitLength_2, totalNum, bValue, saveResultsDir_2 );
min_cost_2 = get_min_cost(saveResultsDir_2, saveNewQFile_1, lamda, epsilon_soft_2);
totalNum = deleteZeroPoints( saveNewQFile_1, visFilesDir_2, saveResultsDir_2, saveNewVisDir_2, saveReIndexFile_2, saveNewQFile_2 );

[ epsilon_soft_3, time_sum_3  ] = Uniform_loopCompress( lamda, saveNewQFile_2, visFilesDir_3, splitLength_3, totalNum, bValue, saveResultsDir_3 );
min_cost_3 = get_min_cost(saveResultsDir_3, saveNewQFile_2, lamda, epsilon_soft_3);
totalNum = deleteZeroPoints( saveNewQFile_2, visFilesDir_3, saveResultsDir_3, saveNewVisDir_3, saveReIndexFile_3, saveNewQFile_3 );
