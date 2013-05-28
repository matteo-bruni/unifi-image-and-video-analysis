I = imread('immaginiTest/dueCerchi.png');
I = im2double(I);
figure, imshow(I); [y,x] = getpts;
P=[x(:) y(:)];
[O,J]=Snake2D(I,P,Options);
