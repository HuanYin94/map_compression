function [ Graph ] = getMatlabGraph( pointByPose, poseLength )
%GETMATLABGRAPH Summary of this function goes here
%   Detailed explanation goes here

    symMat = zeros(poseLength, poseLength);
    
    for i=1:length(pointByPose)
    
        combinations = combntns(pointByPose(i,:), 2);
        
        for c = 1:length(combinations)
           [c1,c2] = combinations(c,:);
           symMat(c1, c2) = symMat(c1, c2) + 1;
        end
        
    end
    
    % symming
    symMat = symMat + symMat';
    
    Graph = graph(symMat);

end

