% Graylevel stretching

img = rgb2gray(imread('test-hist-01.png'));

% display the image
imshow(img); 
figure;
% get gray level histogram
figure;
imhist(img);

% no stretch in this example because the gray level is already between
% 0 and 255 ( because the white dots)
imgS = imadjust(img, double([min(img(:)) max(img(:))]/255.0));
figure;
imshow(imgS); 
figure; 
imhist(imgS);

%Apply graylevel stretching with 4% saturation to the image
imgSS = imadjust(img,stretchlim(img, [0.02 0.98]));

figure;
imshow(imgSS); 
figure; 
imhist(imgSS);

