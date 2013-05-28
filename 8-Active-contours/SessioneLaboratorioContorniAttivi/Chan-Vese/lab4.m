% Osservare il comportamento dell'algoritmo alla variazione di λ2
IforzaEsterna = imread('immaginiTest/testParametroForzaEsterna.png');
% Con i parametri 'classici'
chenvese(IforzaEsterna,'medium',400,0.2,1,1,0,1,'chan');
% Variare λ2
mask = zeros(size(IforzaEsterna));
mask(331:719,371:711) = 1;
chenvese(IforzaEsterna,'medium',2500,0.2,1,100,0,1,'chan');
