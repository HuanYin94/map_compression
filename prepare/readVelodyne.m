function xyz = readVelodyne(filename)

fid = fopen(filename,'rb');
if fid == -1
  error('%s does not exist', filename);
end
velo = uint8(fread(fid,[32 inf],'uint8'));  % 32 /18
xyz = velo(1:12,:);
xyz = xyz(:);
xyz = typecast(xyz, 'single');
xyz = reshape(xyz, 3, []);
xyz(4,:) = 1;

fclose(fid);