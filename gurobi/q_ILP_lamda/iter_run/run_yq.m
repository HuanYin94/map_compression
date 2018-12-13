% params
totalNum = 1867290;
bValue = 1000;
lamda = 1;
qFile = '/home/yinhuan/mapModel/yq/weightVector.txt';
visFilesDir_0 = '/home/yinhuan/mapModel/yq/visMatrix/';
maxQ = 147;
minQ = 1;

splitLength_0 = 20;
saveResultsDir_0 = '/home/yinhuan/mapModel/yq/greedy/0/results/';
saveNewVisDir_0 = '/home/yinhuan/mapModel/yq/greedy/0/visMatrix/';
saveReIndexFile_0 = '/home/yinhuan/mapModel/yq/greedy/index/0.txt';
saveNewQFile_0 = '/home/yinhuan/mapModel/yq/greedy/weight/0.txt';

splitLength_1 = 100;
visFilesDir_1 = saveNewVisDir_0;
saveResultsDir_1 = '/home/yinhuan/mapModel/yq/greedy/1/results/';
saveNewVisDir_1 = '/home/yinhuan/mapModel/yq/greedy/1/visMatrix/';
saveReIndexFile_1 = '/home/yinhuan/mapModel/yq/greedy/index/1.txt';
saveNewQFile_1 = '/home/yinhuan/mapModel/yq/greedy/weight/1.txt';

splitLength_2 = 200;
visFilesDir_2 = saveNewVisDir_1;
saveResultsDir_2 = '/home/yinhuan/mapModel/yq/greedy/2/results/';
saveNewVisDir_2 = '/home/yinhuan/mapModel/yq/greedy/2/visMatrix/';
saveReIndexFile_2 = '/home/yinhuan/mapModel/yq/greedy/index/2.txt';
saveNewQFile_2 = '/home/yinhuan/mapModel/yq/greedy/weight/2.txt';

splitLength_3 =400;
visFilesDir_3 = saveNewVisDir_2;
saveResultsDir_3 = '/home/yinhuan/mapModel/yq/greedy/3/results/';
saveNewVisDir_3 = '/home/yinhuan/mapModel/yq/greedy/3/visMatrix/';
saveReIndexFile_3 = '/home/yinhuan/mapModel/yq/greedy/index/3.txt';
saveNewQFile_3 = '/home/yinhuan/mapModel/yq/greedy/weight/3.txt';

splitLength_4 =800; 
visFilesDir_4 = saveNewVisDir_3;
saveResultsDir_4 = '/home/yinhuan/mapModel/yq/greedy/4/results/';
saveNewVisDir_4 = '/home/yinhuan/mapModel/yq/greedy/4/visMatrix/';
saveReIndexFile_4 = '/home/yinhuan/mapModel/yq/greedy/index/4.txt';
saveNewQFile_4 = '/home/yinhuan/mapModel/yq/greedy/weight/4.txt';

splitLength_5 =1600;
visFilesDir_5 = saveNewVisDir_4;
saveResultsDir_5 = '/home/yinhuan/mapModel/yq/greedy/5/results/';
saveNewVisDir_5 = '/home/yinhuan/mapModel/yq/greedy/5/visMatrix/';
saveReIndexFile_5 = '/home/yinhuan/mapModel/yq/greedy/index/5.txt';
saveNewQFile_5 = '/home/yinhuan/mapModel/yq/greedy/weight/5.txt';

splitLength_6 =4924; % final of yq/greedy_pose
visFilesDir_6 = saveNewVisDir_5;
saveResultsDir_6 = '/home/yinhuan/mapModel/yq/greedy/6/results/';
saveNewVisDir_6 = '/home/yinhuan/mapModel/yq/greedy/6/visMatrix/';
saveReIndexFile_6 = '/home/yinhuan/mapModel/yq/greedy/index/6.txt';
saveNewQFile_6 = '/home/yinhuan/mapModel/yq/greedy/weight/6.txt';


%% iteration of optimization

% [ epsilon_soft_0, time_sum_0  ] = Uniform_loopCompress( lamda, qFile, visFilesDir_0, splitLength_0, totalNum, bValue, saveResultsDir_0 );
% min_cost_0 = get_min_cost(minQ, maxQ, saveResultsDir_0, qFile, lamda, epsilon_soft_0);
totalNum_0 = deleteZeroPoints( qFile, visFilesDir_0, saveResultsDir_0, saveNewVisDir_0, saveReIndexFile_0, saveNewQFile_0 );
% 
% [ epsilon_soft_1, time_sum_1  ] = Uniform_loopCompress( lamda, saveNewQFile_0, visFilesDir_1, splitLength_1, totalNum_0, bValue, saveResultsDir_1 );
% min_cost_1 = get_min_cost(minQ, maxQ, saveResultsDir_1, saveNewQFile_0, lamda, epsilon_soft_1);
% totalNum_1 = deleteZeroPoints( saveNewQFile_0, visFilesDir_1, saveResultsDir_1, saveNewVisDir_1, saveReIndexFile_1, saveNewQFile_1 );
% 
% [ epsilon_soft_2, time_sum_2  ] = Uniform_loopCompress( lamda, saveNewQFile_1, visFilesDir_2, splitLength_2, totalNum_1, bValue, saveResultsDir_2 );
% min_cost_2 = get_min_cost(minQ, maxQ, saveResultsDir_2, saveNewQFile_1, lamda, epsilon_soft_2);
% totalNum_2 = deleteZeroPoints( saveNewQFile_1, visFilesDir_2, saveResultsDir_2, saveNewVisDir_2, saveReIndexFile_2, saveNewQFile_2 );
% 
% [ epsilon_soft_3, time_sum_3  ] = Uniform_loopCompress( lamda, saveNewQFile_2, visFilesDir_3, splitLength_3, totalNum_2, bValue, saveResultsDir_3 );
% min_cost_3 = get_min_cost(minQ, maxQ, saveResultsDir_3, saveNewQFile_2, lamda, epsilon_soft_3);
% totalNum_3 = deleteZeroPoints( saveNewQFile_2, visFilesDir_3, saveResultsDir_3, saveNewVisDir_3, saveReIndexFile_3, saveNewQFile_3 );
% 
% [ epsilon_soft_4, time_sum_4  ] = Uniform_loopCompress( lamda, saveNewQFile_3, visFilesDir_4, splitLength_4, totalNum_2, bValue, saveResultsDir_4 );
% min_cost_4 = get_min_cost(minQ, maxQ, saveResultsDir_4, saveNewQFile_3, lamda, epsilon_soft_4);
% totalNum_4 = deleteZeroPoints( saveNewQFile_3, visFilesDir_4, saveResultsDir_4, saveNewVisDir_4, saveReIndexFile_4, saveNewQFile_4 );
% 
% [ epsilon_soft_5, time_sum_5  ] = Uniform_loopCompress( lamda, saveNewQFile_4, visFilesDir_5, splitLength_5, totalNum_2, bValue, saveResultsDir_5 );
% min_cost_5 = get_min_cost(minQ, maxQ, saveResultsDir_5, saveNewQFile_4, lamda, epsilon_soft_5);
% totalNum_5 = deleteZeroPoints( saveNewQFile_4, visFilesDir_5, saveResultsDir_5, saveNewVisDir_5, saveReIndexFile_5, saveNewQFile_5 );
% 
% [ epsilon_soft_6, time_sum_6  ] = Uniform_loopCompress( lamda, saveNewQFile_2, visFilesDir_5, splitLength_6, totalNum_2, bValue, saveResultsDir_6 );
% min_cost_6 = get_min_cost(minQ, maxQ, saveResultsDir_6, saveNewQFile_5, lamda, epsilon_soft_6);
% totalNum_6 = deleteZeroPoints( saveNewQFile_5, visFilesDir_6, saveResultsDir_6, saveNewVisDir_6, saveReIndexFile_6, saveNewQFile_6 );










