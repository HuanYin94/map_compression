function [  ] = convertPLY( openDir, saveDir )
%CONVERTPLY Summary of this function goes here
%   Detailed explanation goes here
    
    % For KITTI Dataset

    fileExt = '*.bin';
    files = dir(fullfile(openDir,fileExt));  
        
    for i=0:length(files)-1
        i
        
        fileID = sprintf('%06d', i);
        fileName = [openDir, fileID, '.bin' ];
        
        xyz = readVelodyne(fileName);
        xyz = xyz';
        xyz = xyz(:,1:3);
        
        pt = pointCloud(xyz);
        
        saveName = [saveDir, num2str(i), '.ply'];
        pcwrite(pt, saveName);
        
    end

end

