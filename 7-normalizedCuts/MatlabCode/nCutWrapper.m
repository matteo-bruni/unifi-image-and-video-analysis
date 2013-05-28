function [imageSegments, imageRegions] = nCutWrapper(I, sigmaColor, sigmaDistance, radius, minArea)

%   I  nRow x nCol x c matrix representing image.
%   SigmaColor  Coefficient for computing color similarity
%   SigmaDistance  Coefficient for computing spatial location similarity
%   radius   Maximum distance considered before weight matrix value is zero

[nRow, nCol, c] = size(I);
N = nRow * nCol;
IReshaped = reshape(I, N, c);
maxRecursions= 4;

%Compute weight matrix W
W = nCutComputeW(I, sigmaColor, sigmaDistance, radius);
% Recursively repartition
Segments= {};
finalSegments= nCut((1:N)', W, IReshaped, Segments, maxRecursions, minArea);

imageSegments= cell(length(finalSegments));
imageRegions= zeros(N, c);
iReshaped = reshape(I,N,c);
for i= 1:length(finalSegments)
    chann1= ones(size(finalSegments{i})).*getColor(iReshaped(:,1),finalSegments{i});
    chann2= ones(size(finalSegments{i})).*getColor(iReshaped(:,2),finalSegments{i});
    chann3= ones(size(finalSegments{i})).*getColor(iReshaped(:,3),finalSegments{i});
    patch= cat(2,chann1,chann2,chann3);
    imageRegions(finalSegments{i}, :)= patch;
    
    subV = zeros(N, c);
    subV(finalSegments{i}, :) = IReshaped(finalSegments{i}, :);
    imageSegments{i} = ((reshape(subV, nRow, nCol, c)));
end
imageRegions= reshape(imageRegions, nRow, nCol, c);

end

function [color] = getColor(I, index)
 color = mean(I(index,:));
end