
psi = [0 pi/2];
gamma = 0.5;

%best_par è il vettore [theta lambda sigma] dei parametri ottimali
img_in = im2double(imread('DataSet/grey_zebra_texture.jpg'));
%img_in = im2double(imread('DataSet/grey_leo_texture.jpg'));

% par è un vettore di dimensione 3: [theta lambda sigma]
% sintassi:  par_da_ott funzione, X0, LB, UB
[best_par best_val]= fminsearchbnd(@(par) -Energy(img_in, par, psi, gamma), [0 2 2], [0 0 2], [2*pi 8 16]);
theta_opt = best_par(1);
lambda_opt = best_par(2);
sigma_opt = best_par(3);


optimal_filter( img_in, theta_opt, lambda_opt, sigma_opt, psi, gamma);
 
%test_img_in = im2double(imread('DataSet/grey_leo.jpg'));

% nb qui fa cacare 
%test_img_in = im2double(imread('DataSet/grey_leo_erba.jpg'));

test_img_in = im2double(imread('DataSet/grey_zebra.jpg'));
optimal_filter( test_img_in, theta_opt, lambda_opt, sigma_opt, psi, gamma);



