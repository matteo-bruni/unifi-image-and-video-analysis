function gb = gabor_fn(theta, lambda, sigma, psi, gamma)
% gamma = aspect ratio, (0.5)
% psi   = phase shift, [0 pi/2]
% lambda= wave length, (>=2)
% theta = angle in rad, [0 pi)
 
sigma_x = sigma;
sigma_y = sigma/gamma;

%definizione della finestra di kernel

sz = fix(8*max(sigma_y,sigma_x));
%la finestra deve avere dimensione dispari
if mod(sz,2)==0, 
    sz=sz+1;
end
 
[x y] = meshgrid(-fix(sz/2):fix(sz/2),fix(sz/2):-1:fix(-sz/2));

x_theta = (x*cos(theta)+y*sin(theta));
y_theta = (-x*sin(theta)+y*cos(theta));

gb=exp(-0.5*(x_theta.^2/sigma_x^2+y_theta.^2/sigma_y^2)).*cos(2*pi/lambda*x_theta+psi);


end

