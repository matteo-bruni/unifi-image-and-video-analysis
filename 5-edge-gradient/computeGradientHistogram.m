function [ globalHisto ] = computeGradientHistogram( filename, numGridRows, numGridCols, nTheta  )
%COMPUTEGRADIENTHISTOGRAM Summary of this function goes here
%   Detailed explanation goes here

img = imread(filename);

[imgDx imgDy] = gaussgradient(img, 0.7);

imgMag = sqrt( imgDx.*imgDx + imgDy.*imgDy);

imgTheta = atan2(imgDy, imgDx);


imgQTheta = quantizeOrientation(imgTheta, nTheta);


% dividiamo in aree [n m]

gradHisto = zeros(nTheta, numGridRows*numGridCols);

for k=0:(numGridRows*numGridCols-1)
    
    [rMin, rMax, cMin, cMax] = gridBounds(size(imgMag),...
        numGridRows, numGridCols, k);

    gradHisto(:,k+1) = gradientHistogram(imgMag, imgQTheta, nTheta, ...
        rMin, cMin, rMax, cMax);

end

globalHisto = gradHisto(:);


end

