img = imread('test.png');

[imgDx imgDy] = gaussgradient(img, 0.7);

imgMag = sqrt( imgDx.*imgDx + imgDy.*imgDy);

imgTheta = atan2(imgDy, imgDx);

figure; imshow( double(imgMag) ./ max(imgMag(:)) );
 
nTheta = 5;

imgQTheta = quantizeOrientation(imgTheta, nTheta);

figure; imshow( mat2gray(imgQTheta) );


% qui lavora su tutta immagine

%gradHisto = gradientHistogram(imgMag, imgQTheta, rMin, cMin, rMax, cMax);

%bar(gradHisto);

% dividiamo in aree [n m]

gradHisto = zeros(nTheta, numGridRows*numGridCols);

numGridRows=3; numGridCols=2;
for k=0:(numGridRows*numGridCols-1)
    [rMin, rMax, cMin, cMax] = gridBounds(size(imgMag),...
    numGridRows, numGridCols, k);

    gradHisto(:,k+1) = gradientHistogram(imgMag, imgQTheta, nTheta, rMin, cMin, rMax, cMax);

end

globalHisto = gradHisto(:);

bar(globalHisto);