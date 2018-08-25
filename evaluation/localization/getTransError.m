function [ error ] = getTransError( icp0, icp1 )
%GETTRANSERROR Summary of this function goes here
%   Detailed explanation goes here

    pos0 = [icp0(1,4), icp0(1,8), icp0(1,12)];
    pos1 = [icp1(1,4), icp1(1,8), icp1(1,12)];

    error = norm(pos0 - pos1);

end

