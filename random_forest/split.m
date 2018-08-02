%% stop using 
% Y_train = trainCRFFeatures(:,9);
% Y_test_Gnd = testCRFFeatures(:,9);

% 
% X_train_12345 = trainCRFFeatures(:,1:5);
% X_test_12345 = testCRFFeatures(:,1:5);


% X_train_123567 = [trainCRFFeatures(:,1:3), trainCRFFeatures(:,5:7)];
% X_test_123567 = [testCRFFeatures(:,1:3), testCRFFeatures(:,5:7)];

% test on tianjin dataset
% X_test_123567 = [wholeMapfeatures(:,1:3), wholeMapfeatures(:,5:7)];
% [ Y_hard, Y_soft_123567 ] = forestTest(model_123567, X_test_123567);

% sum(Y_soft_123567(:,2)>0.052)

%Y_final = double(Y_soft_123567(:,2)>0.052);


