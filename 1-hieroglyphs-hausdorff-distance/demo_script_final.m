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

for kQuery = 1:size(queryNames, 1)
    % legge una immagine dal dataset di query
    imgQr = imread(queryNames{kQuery});
    % trova le coordinate dei pixel neri nell'immagine
    [imgQrRows imgQrCols] = find(imgQr==0);
    % normalizza le coordinate 
    [imgQrRows imgQrCols] = normalizeCoordSet([imgQrRows imgQrCols]);
        
    for kGallery = 1:size(galleryNames, 1)
        
        % legge una immagine dal dataset di gallery
        imgGlr = imread(galleryNames{kGallery});
        % trova le coordinate dei pixel neri nell'immagine
        [imgGlrRows imgGlrCols] = find(imgGlr==0);
        % normalizza le coordinate
        [imgGlrRows imgGlrCols] = normalizeCoordSet([imgGlrRows imgGlrCols]);
        
        if kQuery == 19
            % calcolo vettore di traslazione
            [best_t best_val] = fminsearch(@(t) hausdorff_t(t, imgQrRows, imgQrCols, imgGlrRows, imgGlrCols), [0,0]);
            % calcola hausdorff con traslazione
            d = best_val;
        else
            % calcola la distanza di Hausdorff tra le coordinate dei due insiemi
            d = hausdorff(imgQrRows, imgQrCols, imgGlrRows, imgGlrCols);
        end

        
        %figure;
        plot(imgQrCols, -imgQrRows, 'b.');
        hold on;
        plot(imgGlrCols, -imgGlrRows, 'r.');
        title(['HDist = ' num2str(d)]);
        hieroDist(kQuery, kGallery) = d;
        hold off;
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
        
        
        
        
