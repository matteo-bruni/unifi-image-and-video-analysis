function [ distance ] = hausdorff_t(t, img1_i, img1_j, img2_i, img2_j)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
distance = 0;

distMatrix = single(zeros(size(img1_i,1),size(img2_i,1)));

img1_i = img1_i + t(1);
img1_j = img1_j + t(2);


for i=1:size(img1_i,1)
    for j=1:size(img2_i,1)
        distMatrix(i,j) = norm( single([img1_i(i) img1_j(i)]) - single([img2_i(j) img2_j(j)]) );
    end
end

% prendo il più grande valore nel vettore di distanze più piccole tra punti
% e sono le distanze A,B

h1 = max(min(distMatrix));

% per le distanze B,A basta la trasposta

h2 = max(min(distMatrix'));

% ritorno il valore massimo tra i due

if h1>h2
    distance = h1;
else
    distance = h2;
end
  

end

