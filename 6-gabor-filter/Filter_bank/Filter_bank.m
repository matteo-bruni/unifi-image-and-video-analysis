clear all;
lambda  = 4;
theta   = pi/2;
psi     = [0 pi/2];
gamma   = 0.5;
galleryNames = {

    'DataSet/1.1.01.png';
    'DataSet/sub01_1.1.01.png';
    %'DataSet/sub02_1.1.01.png';
    'DataSet/1.1.02.png';
    'DataSet/sub01_1.1.02.png';
    % 'DataSet/sub02_1.1.02.png';
    'DataSet/grey_brick01.jpg';
    'DataSet/grey_sub_brick01.jpg';
    %'DataSet/grey_sub02_brick01.jpg';
    'DataSet/1.1.03.png';
    'DataSet/sub01_1.1.03.png';
    %'DataSet/sub02_1.1.03.png';
 
};
for i = 1:size(galleryNames, 1)
    img_in = im2double(imread(galleryNames{i}));
  % img_in = rgb2gray(img_in);  
   gabor_filter( img_in);
end
