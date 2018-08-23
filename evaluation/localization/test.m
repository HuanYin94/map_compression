cnt=1;
for i=4501:1:4800

    position_gt(cnt,:) = [icp0301(keep0301(i),4), icp0301(keep0301(i),8), icp0301(keep0301(i),12)]; 
    angle = rotm2eul([icp0301(keep0301(i),1:3); icp0301(keep0301(i),5:7); icp0301(keep0301(i),9:11)]);
    yaw_gt(cnt,:) = angle(1,1);
    cnt = cnt + 1;
end

position_learn = [yqLearned0(:,4), yqLearned0(:,8), yqLearned0(:,12)];
position_random = [yqRandom0(:,4), yqRandom0(:,8), yqRandom0(:,12)];

for i=1:1:300
    angles_learn = rotm2eul([yqLearned0(i,1:3); yqLearned0(i,5:7); yqLearned0(i,9:11)]);
    yaw_learn(i,:) = angles_learn(1,1);
    angles_random = rotm2eul([yqRandom0(i,1:3); yqRandom0(i,5:7); yqRandom0(i,9:11)]);
    yaw_random(i,:) = angles_random(1,1);
end

for i =1:300
    pos_error_learn(i) = norm(position_learn(i) - position_gt(i));
    pos_error_random(i) = norm(position_random(i) - position_gt(i));

    yaw_error_learn(i) = norm(yaw_learn(i) - yaw_gt(i));
    yaw_error_random(i) = norm(yaw_random(i) - yaw_gt(i));
end

%
position_all = [yqall0(:,4), yqall0(:,8), yqall0(:,12)];
for i =1:300
    angles_all = rotm2eul([yqall0(i,1:3); yqall0(i,5:7); yqall0(i,9:11)]);
    yaw_all(i,:) = angles_all(1,1);
    
    pos_error_all(i) = norm(position_all(i) - position_gt(i));

    yaw_error_all(i) = norm(yaw_all(i) - yaw_gt(i));
end


plot(pos_error_learn, 'k');
hold on;
plot(pos_error_random, 'r');
hold on;
plot(pos_error_all, 'b');

figure;
plot(yaw_error_learn, 'k');
hold on;
plot(yaw_error_random, 'r');
hold on;
plot(yaw_error_all, 'b');







