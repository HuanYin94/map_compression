for i=1:length(parkfullpose)
    
        
    transErrors_2_program(i,:) = getTransError(parkfullpose(i,:), parkprogram2pose(i,:));
    transErrors_2_learn(i,:) = getTransError(parkfullpose(i,:), parklearn2pose(i,:));
    rotmErrors_2_program(i,:) = getRotmError(parkfullpose(i,:), parkprogram2pose(i,:));
    rotmErrors_2_learn(i,:) = getRotmError(parkfullpose(i,:), parklearn2pose(i,:));
     
end



h = boxplot([transErrors_2_program, transErrors_2_learn, 999*ones(length(transErrors_2_program),1), 999*ones(length(transErrors_2_program),1), 999*ones(length(transErrors_2_program),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('2%');
ylim([0 0.5]);
ylabel('Translation error (m)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.05:0.5])




figure
h = boxplot([rotmErrors_2_program, rotmErrors_2_learn, 999*ones(length(transErrors_2_program),1), 999*ones(length(transErrors_2_program),1), 999*ones(length(transErrors_2_program),1)], 'Labels',{'Mp','Ml','Mr','Mv', 'Ms'}, 'OutlierSize', 8);
title('2%');
ylim([0 1.5]);
ylabel('Rotation error (°)', 'FontWeight', 'bold', 'FontSize', 12);
set(gca,'FontSize',12)
set(gca,'ytick', [0:0.15:1.5])








