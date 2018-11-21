% min_costs = [min_cost_Uniform_5, min_cost_DP_5_10, min_cost_DP_5_20, min_cost_DP_5_50, min_cost_Uniform_1];
% need_points = [point_num_Uni_5, point_num_DP_5_10, point_num_DP_5_20, point_num_DP_5_50, pointNum_1];
% time = [time_Uni_5, time_DP_5_10, time_DP_5_20, time_DP_5_50, time_Uni_1];
% 
% figure;
% plot(min_costs, 'r*', 'MarkerSize', 10);
% xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
% ylabel('Min Cost', 'FontSize', 12, 'FontWeight', 'bold');
% set(gca,'Xtick', 1:1:5, 'fontsize',10);
% set(gca,'Xticklabel',{'uniform','DP 10%','DP 20%', 'DP 50%', 'No-Cut'});
% 
% figure;
% plot(need_points, 'ko', 'MarkerSize', 10);
% xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
% ylabel('Remained Points', 'FontSize', 12, 'FontWeight', 'bold');
% set(gca,'Xtick', 1:1:5, 'fontsize',10);
% set(gca,'Xticklabel',{'uniform','DP 10%','DP 20%', 'DP 50%', 'No-Cut'});
% 
% 
% figure;
% plot(time, 'bx', 'MarkerSize', 10);
% xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
% ylabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
% set(gca,'Xtick', 1:1:5, 'fontsize',10);
% set(gca,'Xticklabel',{'uniform','DP 10%','DP 20%', 'DP 50%', 'No-Cut'});


min_costs = [min_cost_Uniform_2, min_cost_DP_2_10, min_cost_DP_2_20, min_cost_DP_2_50, min_cost_DP_2_80, min_cost_Uniform_1];
need_points = [point_num_Uni_2, point_num_DP_2_10, point_num_DP_2_20, point_num_DP_2_50, point_num_DP_2_80, pointNum_1];
time = [time_Uni_2, time_DP_2_10, time_DP_2_20, time_DP_2_50, time_DP_2_80,   time_Uni_1];

figure;
plot(min_costs, 'r*', 'MarkerSize', 10);
xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('Min Cost', 'FontSize', 12, 'FontWeight', 'bold');
grid on;
set(gca,'Xtick', 1:1:6, 'fontsize',10);
set(gca,'Xticklabel',{'uniform','DP 10%','DP 20%', 'DP 50%', 'DP 80%', 'No-Cut'});

hold on;

min_costs_3 = [min_cost_Uniform_3, min_cost_DP_3_10, min_cost_DP_3_20, min_cost_DP_3_50, min_cost_DP_3_80, min_cost_Uniform_1];
plot(min_costs_3, 'ro', 'MarkerSize', 10);

hold on;

min_costs_5 = [min_cost_Uniform_5, min_cost_DP_5_10, min_cost_DP_5_20, min_cost_DP_5_50, min_cost_DP_5_80, min_cost_Uniform_1];
plot(min_costs_5, 'rx', 'MarkerSize', 10);






figure;
plot(need_points, 'k*', 'MarkerSize', 10);
xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
ylabel('Remained Points', 'FontSize', 12, 'FontWeight', 'bold');
grid on;
set(gca,'Xtick', 1:1:6, 'fontsize',10);
set(gca,'Xticklabel',{'uniform','DP 10%','DP 20%', 'DP 50%', 'DP 80%', 'No-Cut'});

hold on;
need_points_3 = [point_num_Uni_3, point_num_DP_3_10, point_num_DP_3_20, point_num_DP_3_50, point_num_DP_3_80, pointNum_1];
plot(need_points_3, 'ko', 'MarkerSize', 10);

hold on;
need_points_5 = [point_num_Uni_5, point_num_DP_5_10, point_num_DP_5_20, point_num_DP_5_50, point_num_DP_5_80, pointNum_1];
plot(need_points_5, 'kx', 'MarkerSize', 10);


% 
% 
% figure;
% plot(time, 'bx', 'MarkerSize', 10);
% xlabel('Method', 'FontSize', 12, 'FontWeight', 'bold');
% ylabel('Time', 'FontSize', 12, 'FontWeight', 'bold');
% set(gca,'Xtick', 1:1:5, 'fontsize',10);
% set(gca,'Xticklabel',{'uniform','DP 10%','DP 20%', 'DP 50%', 'DP 80%', 'No-Cut'});