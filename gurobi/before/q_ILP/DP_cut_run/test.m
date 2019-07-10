function [  ] = test( compressIndex, visTxt, qFile )
%TEST Summary of this function goes here
%   Detailed explanation goes here
    
    q_file_t = fopen(qFile);
    observe_value = fscanf(q_file_t, '%d');
    fclose(q_file_t);
    observe_value(find(observe_value > 100)) = 100;  % remove some unusual counts
    q_value = (1 - mapminmax(observe_value', 0, 1))';  % actually same as in CVPR_W 2013

    seeAgain = intersect(compressIndex, visTxt);

    min_cost = 0;
    for i = 1:1:length(seeAgain)
        min_cost = min_cost + q_value(seeAgain(i));
    end
    
    disp(min_cost);
    
end

