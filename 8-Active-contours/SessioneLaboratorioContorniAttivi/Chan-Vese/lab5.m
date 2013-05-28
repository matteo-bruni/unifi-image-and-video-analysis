% Osservare il risultato della segmentazione con il metodo 'vector'
Ilego4colori = imread('immaginiTest/lego4colori.jpg');
% Algoritmo Chan-Vese
chenvese(Ilego4colori,'medium',800,0.2,1,1,0,1,'chan');
% Algoritmo Chan-Vese Vector
chenvese(Ilego4colori,'medium',1000,0.2,1,1,0,1,'vector');
