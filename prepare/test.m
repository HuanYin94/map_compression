

travelDis(1,:) = [ 0, 1];

allDis = 0;

for i = 2 : length(icp030001)
    
    pose1 = [icp030001(i,4), icp030001(i,8)];
    pose2 = [icp030001(i-1,4), icp030001(i-1,8)];

    allDis = allDis + norm(pose1 - pose2);

    travelDis(i,:) = [ allDis, i];
     
end

plot(travelDis(:,1), travelDis(:,2), 'k.');

choose = decimate(travelDis(:,1)*100, 5);