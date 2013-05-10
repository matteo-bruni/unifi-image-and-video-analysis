function [ mu ] = Energy(img_in,par,psi,gamma)
% par è un vettore di dimensione 3: [theta lambda sigma]
  %psi è un vettore: [0 pi/2]  

    % calcolo della parte reale e immaginaria del filtro di Gabor,
    % attraverso un diverso valore della fase
    gb_R = gabor_fn(par(1), par(2), par(3), psi(1), gamma);
    gb_I = gabor_fn(par(1), par(2), par(3), psi(2), gamma);     
    
    %convoluzione dell'immagine rispettivamente con la parte reale e la parte
    %immaginaria del filtro  di Gabor
    H_R(:,:) = imfilter(img_in, gb_R, 'conv'); 
    H_I(:,:) = imfilter(img_in, gb_I, 'conv'); 
    
    %calcolo dell'energia
    E = uint8((H_R.^2+H_I.^2).^0.5);
   
    variance= var(double(E(:)));
    E = E/variance;
    mu = mean2(E);
end

