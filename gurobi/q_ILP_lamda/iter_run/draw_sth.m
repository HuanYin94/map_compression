min_costs = [min_cost_0, min_cost_1, min_cost_2, min_cost_3, min_cost_Uniform_1, min_cost_DP_2_80_max];
need_points = [totalNum_0, totalNum_1, totalNum_2, totalNum_3, pointNum_1, point_num_DP_2_80_max];
time = [time_sum_0, time_sum_1, time_sum_2, time_sum_3, time_Uni_1, time_DP_2_80_max];

figure;
plot(min_costs, 'r*', 'MarkerSize', 10);
xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('Min Cost', 'FontSize', 12, 'FontWeight', 'bold');
grid on;
set(gca,'Xtick', 1:1:6, 'fontsize',10);
set(gca,'Xticklabel',{'Iter 1','Iter 2', 'Iter 3', 'Iter 4', 'Global Opti', 'Best 2 Cuts'});

figure;
plot(need_points, 'ro', 'MarkerSize', 10);
xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('Kept Points', 'FontSize', 12, 'FontWeight', 'bold');
grid on;
set(gca,'Xtick', 1:1:6, 'fontsize',10);
set(gca,'Xticklabel',{'Iter 1','Iter 2', 'Iter 3', 'Iter 4', 'Global Opti', 'Best 2 Cuts'});

figure;
plot(time, 'rs', 'MarkerSize', 10);
xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('Time (second)', 'FontSize', 12, 'FontWeight', 'bold');
grid on;
set(gca,'Xtick', 1:1:6, 'fontsize',10);
set(gca,'Xticklabel',{'Iter 1','Iter 2', 'Iter 3', 'Iter 4', 'Global Opti', 'Best 2 Cuts'});



