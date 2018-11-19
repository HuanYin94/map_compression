min_costs = [min_cost_Uniform_5, min_cost_DP_5_10, min_cost_DP_5_20, min_cost_DP_5_50, min_cost_Uniform_1];
need_points = [point_num_Uni_5, point_num_DP_5_10, point_num_DP_5_20, point_num_DP_5_50, pointNum_1];
time = [time_Uni_5, time_DP_5_10, time_DP_5_20, time_DP_5_50, time_Uni_1];

figure;
plot(min_costs, 'r*', 'MarkerSize', 10);
xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('Min Cost', 'FontSize', 12, 'FontWeight', 'bold');
set(gca,'Xtick', 1:1:5, 'fontsize',10);
set(gca,'Xticklabel',{'uniform','DP 10%','DP 20%', 'DP 50%', 'No-Cut'});

figure;
plot(need_points, 'ko', 'MarkerSize', 10);
xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('Remained Points', 'FontSize', 12, 'FontWeight', 'bold');
set(gca,'Xtick', 1:1:5, 'fontsize',10);
set(gca,'Xticklabel',{'uniform','DP 10%','DP 20%', 'DP 50%', 'No-Cut'});


figure;
plot(time, 'bx', 'MarkerSize', 10);
xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
set(gca,'Xtick', 1:1:5, 'fontsize',10);
set(gca,'Xticklabel',{'uniform','DP 10%','DP 20%', 'DP 50%', 'No-Cut'});