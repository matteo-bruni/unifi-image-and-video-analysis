img = imread('disks.png');
imgPoisson = imnoise(img, 'poisson');

n=5;
m=5;

% Non Linear filtering
imgOut_nl = nlfilter(double(imgPoisson), [n m], @myMedian);
figure; 
imshow(mat2gray(imgOut_nl));

d_3 = mse( double(imgOut_nl(:) ) - double(img(:)))


% Bilateral filtering
imgOut_bil = nlfilter(double(imgPoisson), [n m], @bilateral);
figure; 
imshow(mat2gray(imgOut_bil));

d_4 = mse( double(imgOut_bil(:) ) - double(img(:)))


% Median Filtering
imgOut_med = medfilt2(double(imgPoisson), [n m]);
figure; 
imshow(mat2gray(imgOut_med));
d_5 = mse( double(imgOut_med(:) ) - double(img(:)))