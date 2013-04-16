% impixelinfo get pixel from mouse
% apply 30 deg rotation to img
imgInput = imread('sat-1.png');
imgTarget = imread('sat-2.png');

% equivalents points
inputP = [130 135; 93 311; 342 392; 466 94];
targetP = [121 192; 146 371; 407 362; 421 40];

% solve a min squeare problem rotate the image to overlap input and target
T = cp2tform(inputP, targetP, 'affine');
% apply transformation
imgRegistered = imtransform(imgInput, T);

figure; imshow(imgRegistered)

% since we have black corners we crop the image we can confront
imgRegistered = imtransform(imgInput, T, 'XData',[1 size(imgTarget,2)], 'YData', [1 size(imgTarget,1)]);
figure; imshow(imgRegistered)

% write a function that given the target and registered
% images returns a map of the pixel by pixel differences

% we have rgb images, so we have 3 components to confront: x,y and plan
% r,g,b of an image

% nb difference between 2 unsigned images = 0

imgDifference = img_difference(imgTarget, imgRegistered);
figure;
imshow(imgDifference);
