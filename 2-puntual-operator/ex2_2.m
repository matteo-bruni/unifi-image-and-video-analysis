% Equalization

img = rgb2gray(imread('test-hist-01.png'));
img2 = rgb2gray(imread('test-hist-02.png'));

figure;
histeq(img);
figure;
histeq(img2);


img3 = adapthisteq(img, 'clipLimit',0.1, 'NumTiles',[8 8]);
img4 = adapthisteq(img2, 'clipLimit',0.1, 'NumTiles',[8 8]);

figure;
histeq(img3);

figure;
histeq(img4);


img_what = (imread('what_is_this.png'));
figure;
histeq(img_what);
img_what_eq = adapthisteq(img_what, 'clipLimit',0.1, 'NumTiles',[8 8]);
figure;
histeq(img_what_eq);