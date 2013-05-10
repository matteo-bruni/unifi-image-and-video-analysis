function [  ] = gabor_filter(img_in)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
% 
% gamma = aspect ratio, (0.5)
% psi   = phase shift, [0 pi/2]
% lambda= wave length, (>=2)
% theta = angle in rad, [0 pi)


psi     = [0 pi/2];
gamma   = 0.5;
%numero di valori per theta
N = 4;
theta = zeros(N);
%numero di valori per lambda
M = 2; 
lambda = zeros(M);
%numero di valori per sigma
P = 2;
sigma = zeros(P);
%creazione dei vettori di valori per i tre parametri
for i =1:N
   theta(i+1) =  theta(i) + pi/N;
end

for i =1:M
   lambda(i) =  i * 2;
end

for i =1:P
   sigma(i) = 2^i;
end

filter_number = N*P*M;

img_out = zeros(size(img_in,1), size(img_in,2), filter_number);  % creazione N*P*M matrici di zeri
figure(1);
    k=1;
    for n = 1:N
        for m = 1:M
            for p = 1:P   
             %filtro di gabor
             gb = gabor_fn(theta(n), lambda(m), sigma(p), psi(1), gamma) + ...
                 1i * gabor_fn(theta(n), lambda(m), sigma(p), psi(2), gamma);  
            subplot(N,M*P,k), imshow(abs(gb/2+0.5));
            % applicazione del filtro
            img_out(:,:,k) = imfilter(img_in, gb,'conv');  
            k = k+1;            
            end
      
        end
    end
% superposition con L2-norm
img_out_disp = sum(abs(img_out).^2, 3).^0.5;
% normalize
img_out_disp = img_out_disp./max(img_out_disp(:));
figure; 
subplot(1,2,1),imshow(img_in);
subplot(1,2,2),imshow(img_out_disp);
title('gabor output');
end

