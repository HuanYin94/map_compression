% [ Y_soft_all, model_all ] = random_test(Y_train, X_train_all, X_test_all);
% [ Y_soft_123567, model_123567 ] = random_test(Y_train, X_train_123567, X_test_123567);
% [ Y_soft_12367, model_12367 ] = random_test(Y_train, X_train_12367, X_test_12367);
% [ Y_soft_123678, model_123678 ] = random_test(Y_train, X_train_123678, X_test_123678);
% [ Y_soft_12345, model_12345 ] = random_test(Y_train, X_train_12345, X_test_12345);
% [ Y_soft_678, model_678 ] = random_test(Y_train, X_train_678, X_test_678);

%Precsion Recall curves

% [precision_all, recall_all, threshold_all] = plot_PR(Y_soft_all(:,2), Y_test_Gnd);
% [precision_123567, recall_123567, threshold_123567] = plot_PR(Y_soft_123567(:,2), Y_test_Gnd);
% [precision_12367, recall_12367, threshold_12367] = plot_PR(Y_soft_12367(:,2), Y_test_Gnd);
% [precision_123678, recall_123678, threshold_123678] = plot_PR(Y_soft_123678(:,2), Y_test_Gnd);
% [precision_12345, recall_12345, threshold_12345] = plot_PR(Y_soft_12345(:,2), Y_test_Gnd);
% [precision_678, recall_678, threshold_678] = plot_PR(Y_soft_678(:,2), Y_test_Gnd);


% 
% figure;
% plot(recall_all, precision_all, 'LineWidth', 2);
% hold on;
% plot(recall_123567, precision_123567, 'LineWidth', 2);
% hold on;
% plot(recall_12367, precision_12367, 'LineWidth', 2);
% hold on;
% plot(recall_123678, precision_123678, 'LineWidth', 2);
% hold on;
% plot(recall_12345, precision_12345, 'LineWidth', 2);
% hold on;
% plot(recall_678, precision_678, 'LineWidth', 2);
% legend('all', '123567', '12367', '123678', '12345', '678');
% xlabel('Recall');
% ylabel('Precision');
% grid on;
% xlim([0 1]);
% ylim([0 1]);

% 
% %ROC curves
% [X_all,Y_all,T_all,AUC_all] = perfcurve(Y_test_Gnd, Y_soft_all(:,2), '1');
% [X_123567,Y_123567,T_123567,AUC_123567] = perfcurve(Y_test_Gnd, Y_soft_123567(:,2), '1');
% [X_12367,Y_12367,T_12367,AUC_12367] = perfcurve(Y_test_Gnd, Y_soft_12367(:,2), '1');
% [X_123678,Y_123678,T_123678,AUC_123678] = perfcurve(Y_test_Gnd, Y_soft_123678(:,2), '1');
% [X_12345,Y_12345,T_12345,AUC_12345] = perfcurve(Y_test_Gnd, Y_soft_12345(:,2), '1');
% [X_678,Y_678,T_678,AUC_678] = perfcurve(Y_test_Gnd, Y_soft_678(:,2), '1');
% 
% 
% figure;
% plot(X_all, Y_all, 'LineWidth', 2);
% hold on;
% plot(X_123567, Y_123567, 'LineWidth', 2);
% hold on;
% plot(X_12367, Y_12367, 'LineWidth', 2);
% hold on;
% plot(X_123678, Y_123678, 'LineWidth', 2);
% hold on;
% plot(X_12345, Y_12345, 'LineWidth', 2);
% hold on;
% plot(X_678, Y_678, 'LineWidth', 2);
% hold on;
% legend('all', '123567', '12367', '123678', '12345', '678');
% xlabel('FPR');
% ylabel('TPR');
% grid on;
% xlim([0 1]);
% ylim([0 1]);






