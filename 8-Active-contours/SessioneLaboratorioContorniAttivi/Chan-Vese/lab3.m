% Osservare il comportamento dell'algoritmo alla variazione di λ1
IforzaInterna = imread('immaginiTest/testParametroForzaInterna.png');
% Con i parametri 'classici'
chenvese(IforzaInterna,'medium',1000,0.2,1,1,0,1,'chan');
% Variare λ1
chenvese(IforzaInterna,'medium',4000,0.2,30,1,0,1,'chan');
