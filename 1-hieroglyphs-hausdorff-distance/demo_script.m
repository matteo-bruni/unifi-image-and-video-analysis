galleryNames = { 'hiero_01/01.png';
'hiero_01/02.png';
'hiero_01/03.png';
'hiero_01/04.png';
'hiero_01/05.png';
'hiero_01/06.png';
'hiero_01/07.png';
'hiero_01/08.png';
'hiero_01/09.png';
'hiero_01/10.png';
'hiero_01/11.png';
'hiero_01/12.png';
'hiero_01/13.png';
'hiero_01/14.png';
'hiero_01/15.png';
'hiero_01/16.png';
'hiero_01/17.png';
'hiero_01/18.png';
'hiero_01/19.png';
'hiero_01/20.png';
'hiero_01/21.png';
'hiero_01/22.png'};

galleryNames2 = { 'hiero_02/01.png';
'hiero_02/02.png';
'hiero_02/03.png';
'hiero_02/04.png';
'hiero_02/05.png';
'hiero_02/06.png';
'hiero_02/07.png';
'hiero_02/08.png';
'hiero_02/09.png';
'hiero_02/10.png';
'hiero_02/11.png';
'hiero_02/12.png';
'hiero_02/13.png';
'hiero_02/14.png';
'hiero_02/15.png';
'hiero_02/16.png';
'hiero_02/17.png';
'hiero_02/18.png';
'hiero_02/19.png'};

% legge la prima immagine di gallery
img01 = imread(galleryNames{1});

% trova le coordinate dei pixel neri nell'immagine
[img01_I img01_J] = find(img01==0);

% legge l'immagine hiero_02/02.png
img02 = imread('hiero_02/02.png');

% trova le coordinate dei pixel neri nell'immagine
[img02_I img02_J] = find(img02==0);

% calcola la distanza di Hausdorff tra le coordinate dei due insiemi
hausdorff(img01_I, img01_J, img02_I, img02_J);

% creo una matrice delle distanze di hausdoff

hausMatrix = single(zeros(size(galleryNames,1), size(galleryNames2,1)));

for i=1:size(galleryNames,1)
    for j=1:size(galleryNames2,1)
        img01 = imread(galleryNames{i});
        img02 = imread(galleryNames2{j});
    
        [img01_I, img01_J] = find(img01==0);
        [img02_I, img02_J] = find(img02==0);


        hausMatrix(i,j) = hausdorff(img01_I, img01_J, img02_I, img02_J);
        
    end
    

end

hausMatrix

% calcola riga 1 il più simile
[C, I] = min (hausMatrix(1,:))

