function [  ] = optimal_filter( img_in, theta, lambda, sigma, psi, gamma)
% 
% gamma = aspect ratio, (0.5)
% psi   = phase shift, [0 pi/2]
% lambda= wave length, (>=2)
% theta = angle in rad, [0 2pi]

img_out = zeros(size(img_in,1), size(img_in,2)); 
gb = gabor_fn(theta, lambda, sigma,psi(1),gamma) + 1i *  gabor_fn(theta, lambda, sigma,psi(2),gamma);
figure;
% gb/2 + 0.5 per ottenere l'immagine in scale di grigio
imshow(abs(gb/2+0.5));
img_out(:,:) = imfilter(img_in, gb, 'conv');
img_out = img_out./max(img_out(:));
figure; 
subplot(1,2,1),imshow(img_in);
subplot(1,2,2),imshow(double(imag(img_out)));
end