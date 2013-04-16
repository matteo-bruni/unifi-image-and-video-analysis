function [ img3 ] = img_difference( img1, img2 )
%DIFFERENCE_MATRIX Summary of this function goes here
%   Detailed explanation goes here
    img3 = zeros(size(img1));
    
    
    img3 = img1(:,:,:) - img2(:,:,:);
    %img3(:,:,2) = img2(:,:,2) - img1(:,:,2);
    %img3(:,:,3) = img2(:,:,3) - img1(:,:,3);
    
end

