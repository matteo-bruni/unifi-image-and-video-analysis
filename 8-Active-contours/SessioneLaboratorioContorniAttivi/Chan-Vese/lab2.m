Ipolmoni = imread('immaginiTest/polmoni.jpg');
chenvese(Ipolmoni,'large',700,0.2,1,1,0,1,'chan');
chenvese(Ipolmoni,'whole',700,0.2,1,1,0,1,'chan');
Ineurone = imread('immaginiTest/neurone.jpg');
chenvese(Ineurone,'whole',700,0.2,1,1,0,1,'chan');
mask = zeros(size(Ineurone));
mask(250:350, 300:400,:) = 1;
chenvese(Ineurone,mask,1000,0.2,1,1,0,1,'chan');
