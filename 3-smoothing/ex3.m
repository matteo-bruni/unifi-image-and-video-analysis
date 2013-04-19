% Linear Filtering

img = imread('disks.png');
imgPoisson = imnoise(img, 'poisson');
imgSpeckle = imnoise(img, 'speckle');
imshow(imgPoisson);
figure;
imshow(imgSpeckle);

mask = ones ([3 3]) ./ 9;

imgOut = imfilter(imgPoisson, mask, 'conv');
figure;
imshow(imgOut);

imgOut = conv2(double(imgPoisson), double(mask), 'same');
figure;
% nb double between 0-255 foreach valure above 1 goes blank
imshow(imgOut ./ max(imgOut(:)));


% calculate mean squared error

d_1 = mse( double(imgOut(:) ) - double(img(:)));
% low value low error



% use gaussian operator
mask = makeGauss(1.2);

imgOut_g = imfilter(imgPoisson, mask, 'conv');
figure; 
imshow(imgOut_g);

d_2 = mse( double(imgOut_g(:) ) - double(img(:)));

d_1
d_2