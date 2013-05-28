% Osservare il risultato della segmentazione con il metodo 'multiphase'
Ilego4colori = imread('immaginiTest/lego4colori.jpg');
% Algoritmo Chan-Vese Vector
chenvese(Ilego4colori,'medium',1000,0.2,1,1,0,1,'vector');
% Algoritmo Chan-Vese Multiphase
chenvese(Ilego4colori,'medium',500,0.2,1,1,0,1,'multiphase');
Ilego5colori = imread('immaginiTest/lego5colori.jpg');
chenvese(Ilego5colori,'medium',1400,0.2,1,1,0,-1,'multiphase');
