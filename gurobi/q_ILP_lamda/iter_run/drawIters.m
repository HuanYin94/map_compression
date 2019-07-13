

%%  min cost of each iteration


iter = [1;2;3;4;5;6;7];
min_cost_2000 = [min_cost_0_2000; min_cost_1_2000; min_cost_2_2000; min_cost_3_2000; min_cost_4_2000; min_cost_5_2000; min_cost_6_2000 ];
min_cost_3000 = [min_cost_0_3000; min_cost_1_3000; min_cost_2_3000; min_cost_3_3000; min_cost_4_3000; min_cost_5_3000; min_cost_6_3000];
min_cost_1000 = [min_cost_0_1000; min_cost_1_1000; min_cost_2_1000; min_cost_3_1000; min_cost_4_1000; min_cost_5_1000; min_cost_6_1000 ];
min_cost_4000 = [min_cost_0_4000; min_cost_1_4000; min_cost_2_4000; min_cost_3_4000; min_cost_4_4000; min_cost_5_4000; min_cost_6_4000 ];

ff = figure;
plot(iter, min_cost_4000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'm'); hold on;
plot(iter, min_cost_3000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'b'); hold on;
plot(iter, min_cost_2000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'r'); hold on;
plot(iter, min_cost_1000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'k'); hold on;

grid on;
l=legend('b=4000', 'b=3000',  'b=2000', 'b=1000');
title(l, 'Cost after Progm');
xlabel('Iteration', 'FontWeight', 'bold', 'FontSize', 14);
ylabel('Cost', 'FontWeight', 'bold', 'FontSize', 14);
set(ff, 'Renderer', 'painters');
set(gca,'Xtick', 1:1:7);
set(gca,'FontSize',11);
set(gca,'Xticklabel',{'Iter 1','Iter 2', 'Iter 3', 'Iter 4', 'Iter 5', 'Iter 6', 'Iter 7'})









%% totalNum and Kept ratio



iter = [1;2;3;4;5;6;7];
totalNum_2000 = [totalNum_0_2000; totalNum_1_2000; totalNum_2_2000; totalNum_3_2000; totalNum_4_2000; totalNum_5_2000; totalNum_6_2000 ];
totalNum_3000 = [totalNum_0_3000; totalNum_1_3000; totalNum_2_3000; totalNum_3_3000; totalNum_4_3000; totalNum_5_3000; totalNum_6_3000 ];
totalNum_1000 = [totalNum_0_1000; totalNum_1_1000; totalNum_2_1000; totalNum_3_1000; totalNum_4_1000; totalNum_5_1000; totalNum_6_1000 ];
totalNum_4000 = [totalNum_0_4000; totalNum_1_4000; totalNum_2_4000; totalNum_3_4000; totalNum_4_4000; totalNum_5_4000; totalNum_6_4000 ];


% ff = figure;
% plot(iter, totalNum_2000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'r'); hold on;
% plot(iter, totalNum_1000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'm'); hold on;
% plot(iter, totalNum_4000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'k'); hold on;
% plot(iter, totalNum_4000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'm'); hold on;

% grid on;
% l=legend('b=2000', 'b=3000', 'b=1000', 'b=500');
% title(l, 'Number of Points after Progm');
% xlabel('Iteration', 'FontWeight', 'bold', 'FontSize', 14);
% ylabel('Number of Points', 'FontWeight', 'bold', 'FontSize', 14);
% set(ff, 'Renderer', 'painters');
% set(gca,'Xtick', 1:1:8, 'fontsize',14);
% set(gca,'Xticklabel',{'Iter 1','Iter 2', 'Iter 3', 'Iter 4', 'Iter 5', 'Iter 6', 'Iter 7', 'Iter 8'});
% set(gca,'FontSize',12);

iter = [1;2;3;4;5;6;7];
ratio_2000 = totalNum_2000 / totalNum;
ratio_3000 = totalNum_3000 / totalNum;
ratio_1000 = totalNum_1000 / totalNum;
ratio_4000 = totalNum_4000 / totalNum;

ff = figure;
plot(iter, ratio_4000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'm'); hold on;
plot(iter, ratio_3000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'b'); hold on;
plot(iter, ratio_2000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'r'); hold on;
plot(iter, ratio_1000, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'k'); hold on;

grid on;
l=legend('b=4000', 'b=3000', 'b=2000', 'b=1000');
title(l, 'Ratio after Progm');
xlabel('Iteration', 'FontWeight', 'bold', 'FontSize', 14);
ylabel('Compression Ratio', 'FontWeight', 'bold', 'FontSize', 14);
set(ff, 'Renderer', 'painters');
set(gca,'Xtick', 1:1:7);
set(gca,'Xticklabel',{'Iter 1','Iter 2', 'Iter 3', 'Iter 4', 'Iter 5', 'Iter 6', 'Iter 7'});
set(gca,'FontSize',11);

%%  Total time cost
% 
% time_sum_2000 = sum([time_sum_0, time_sum_1, time_sum_2, time_sum_3, time_sum_4, time_sum_5, time_sum_6, time_sum_7]);
% time_sum_3000 = sum([time_sum_0, time_sum_1, time_sum_2, time_sum_3, time_sum_4, time_sum_5, time_sum_6, time_sum_7]);
% time_sum_1200 = sum([time_sum_0, time_sum_1, time_sum_2, time_sum_3, time_sum_4, time_sum_5, time_sum_6, time_sum_7]);
% time_sum_1000 = sum([time_sum_0, time_sum_1, time_sum_2, time_sum_3, time_sum_4, time_sum_5, time_sum_6, time_sum_7]);
% time_sum_800 = sum([time_sum_0, time_sum_1, time_sum_2, time_sum_3, time_sum_4, time_sum_5, time_sum_6, time_sum_7]);
% 
% 
% 
% iter = [1;2;3;4;5];
% times = [time_sum_2000; time_sum_3000; time_sum_1200; time_sum_1000; time_sum_800];
% ff = figure;
% plot(iter, times, '--bs', 'MarkerSize', 10,  'MarkerFaceColor', 'r'); hold on;
% grid on;
% l=legend('time costs');
% title(l, 'Total time under Diff b');
% xlabel('Iteration', 'FontWeight', 'bold', 'FontSize', 12);
% ylabel('Time Costs (second)', 'FontWeight', 'bold', 'FontSize', 12);
% set(ff, 'Renderer', 'painters');
% set(gca,'Xtick', 1:1:8, 'fontsize',10);
% set(gca,'Xticklabel',{'b=2000','b=3000', 'b=1200', 'b=1000', 'b=800'});