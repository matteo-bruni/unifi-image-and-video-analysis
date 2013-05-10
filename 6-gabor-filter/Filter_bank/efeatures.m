function [ mean, variance ] = efeatures(img_in, theta, lambda, sigma, psi, gamma)
%EFEATURES Summary of this function goes here
%   Detailed explanation goes here

    sigma_x = sigma;
    sigma_y = sigma/gamma;

    sz = fix(8*max(sigma_y,sigma_x));
    if mod(sz,2)==0, 
        sz=sz+1;
    end

    [x y] = meshgrid(-fix(sz/2):fix(sz/2),fix(sz/2):-1:fix(-sz/2));

    x_theta = (x*cos(theta)+y*sin(theta));
    y_theta = (-x*sin(theta)+y*cos(theta));

    gb = exp(-0.5*(x_theta.^2/sigma_x^2+y_theta.^2/sigma_y^2)).* ... 
 
        cos(2*pi/lambda*x_theta+psi);

    Im_gb = imfilter(img_in, double(imag(gb)), 'conv');
    Re_gb= imfilter(img_in, double(real(gb)), 'conv');

    energia = uint8(sqrt(Im_gb.^2 +  Re_gb.^2));

    %Estrazione delle features
    mean = mean2(energia);
    variance = var(double(energy(:)));

end

