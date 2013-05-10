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

queryNames = { 'hiero_02/01.png';
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
'hiero_02/19.png';
};

numGridRows = 2;
numGridCols = 2;
nTheta = 6;

for kQuery = 1:size(queryNames, 1)
    % estrae il descrittore di una immagine di query
    HoGM_Qr = computeGradientHistogram(queryNames{kQuery}, numGridRows, numGridCols, nTheta)
            
    for kGallery = 1:size(galleryNames, 1)
        % legge una immagine dal dataset di gallery
        HoGM_Glr = computeGradientHistogram(galleryNames{kGallery}, numGridRows, numGridCols, nTheta)
        
        % calcola la distanza i due istogrammi
        d = norm(double(HoGM_Qr)-double(HoGM_Glr));
        hieroDist(kQuery, kGallery) = d;
%         figure;
%         plot(imgQrCols, -imgQrRows, 'b.');
%         hold on;
%         plot(imgGlrCols, -imgGlrRows, 'r.');
%         title(['HDist = ' num2str(d)]);
%         hold off;
    end
end
[bestMatches bestIndex] = min(hieroDist);

for k=1:size(bestIndex,2)
    figure;
    subplot(1, 2, 1);
    imshow(mat2gray(imread(galleryNames{k})));
    subplot(1, 2, 2);
    imshow(mat2gray(imread(queryNames{bestIndex(k)})));
%    disp([k bestIndex(k)]);
end
        
        