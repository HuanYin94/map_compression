function [ yaw_error ] = getRotmError( icp0, icp1 )
%GETROTMERROR Summary of this function goes here
%   Detailed explanation goes here

    angle0 = rotm2eul([icp0(1,1:3); icp0(1,5:7); icp0(1,9:11)]);
    angle1 = rotm2eul([icp1(1,1:3); icp1(1,5:7); icp1(1,9:11)]);

    yaw0 = angle0(1,1);
    yaw1 = angle1(1,1);
    
    yaw_error = abs(yaw0 - yaw1);
    
    yaw_error = rad2deg(yaw_error);
    
    for i=1:length(yaw_error)
       
        if yaw_error(i) > 180
            yaw_error(i) = 360 - yaw_error(i);
        end
        
    end
    
end

