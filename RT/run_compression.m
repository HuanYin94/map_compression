%% LOAD

% disp('LOADING');
% trainM = single(load('/home/huyin/learn/ILP_YQ_list_train_0.1_feature_label.txt', '-ascii'));
% testM = single(load('/home/huyin/learn/ILP_YQ_list_test_0.1_feature_label.txt', '-ascii'));
% Y_train = trainM(:,11);
% X_train = trainM(:,1:10);
% Y_test = testM(:,11);
% X_test = testM(:,1:10);

%% RUN

% disp('SETTING');
% rtensparam=init_extra_trees();
% % To change the number of trees in the ensemble
% rtensparam.nbterms=100;
% % To change the parameter K of the extra-trees method
% rtensparam.rtparam.adjustdefaultk=1;
% rtensparam.rtparam.extratreesk=10;
% weight=[];
% vis = 1;
% % ???  selection of training samples
% ls=int32(1:200000);
% 
% disp('TRAINING & TESTING');
% tic
% Y_PRED = rtenslearn_c(X_train, Y_train, ls, weight, rtensparam, X_test, 1);
% time = toc

%% evaluation

disp('SO?')

A = find(Y_test==1);
B = find(Y_PRED>0.31);

TP = length(intersect(A, B));
precision = TP / length(B);
recall = TP / length(A);















