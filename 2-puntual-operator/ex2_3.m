% Geometric transforms


% apply 30 deg rotation to img
imgIn = imread('ferrari.jpg');
figure;imshow(imgIn);

% 30 deg to left = 90+30 =120
A = [cos(120) -sin(120) 0; sin(120) cos(120) 0; 0 0 1 ];
T = maketform('affine', A);
imgOut = imtransform(imgIn,T);
figure;imshow(imgOut);