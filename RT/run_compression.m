%% LOAD

% disp('LOADING');
% trainM = single(load('/home/huyin/learn/ILP_YQ_list_train_0.2_feature_label.txt', '-ascii'));
% testM = single(load('/home/huyin/learn/ILP_YQ_list_test_0.2_feature_label.txt', '-ascii'));
% Y_train = trainM(:,11);
% X_train = trainM(:,1:10);
% Y_test = testM(:,11);
% X_test = testM(:,1:10);

 %% RUN

disp('SETTING');
rtensparam = init_rf(10);
weight=[];
vis = 1;
% ???  selection of training samples
ls=int32(1:299956);

disp('TRAINING & TESTING');
tic
[Y_PRED]=rtenslearn_c(X_train,Y_train,ls,weight,rtensparam,X_test,1);
time = toc

%% evaluation

% disp('P-R')

% A = find(Y_test==1);
% B = find(Y_PRED>0.38);
% 
% TP = length(intersect(A, B));
% precision = TP / length(B);
% recall = TP / length(A);
% 
% Y_final = double(Y_PRED > 0.38);
% dlmwrite('/home/huyin/predict_result.txt', Y_final);















