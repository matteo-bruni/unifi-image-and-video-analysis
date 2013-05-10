function [mu, variance] = E_Features(img_in, theta, lambda, sigma, psi, gamma)

 Im_gb = zeros(size(img_in,1), size(img_in,2));
 Re_gb = zeros(size(img_in,1), size(img_in,2));
 energy = zeros(size(img_in,1), size(img_in,2));

 %costruzione del filtro di Gabor
 gb = gabor_fn(theta, lambda, sigma, psi(1), gamma) + 1i * gabor_fn(theta, lambda, sigma, psi(2), gamma);   
 %figure;imshow(abs(gb/2+0.5));
 
 %convoluzione dell'immagine rispettivamente con la parte reale e la parte
 %immaginaria del filtro  di Gabor
 Im_gb = imfilter(img_in, double(imag(gb)), 'conv');
 Re_gb= imfilter(img_in, double(real(gb)), 'conv');
 
 %Calcolo dell'energia
 energy = uint8(sqrt(Im_gb.^2 +  Re_gb.^2));

 %Estrazione delle features
 mu = mean2(energy);
 variance = var(double(energy(:)));
 
end


