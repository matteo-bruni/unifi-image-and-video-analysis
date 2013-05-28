% Osservare il risultato della segmentazione con il metodo 'vector' su
% immagini rumorose
Irosa = imread('immaginiTest/rosa.jpg');
Irosa = imresize(Irosa,0.2);
imshow(Irosa);
% Applicare rumore
Irosa(:,:,1) = imnoise(Irosa(:,:,1),'speckle');
Irosa(:,:,2) = imnoise(Irosa(:,:,2),'salt & pepper',0.8);
chenvese(Irosa,'medium',800,0.2,1,1,0,1,'chan');
% Usare 'vector'
chenvese(Irosa,'medium',200,0.2,1,1,0,1,'vector');
