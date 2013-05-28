function[imageSegments,imageRegions]= spectralKmeansImageSegment(I, sigmaColor, sigmaDistance, radius, nClusters)

%   I  nRow x nCol x c matrix representing the image.
%   sigmaColor  Coefficient for computing color similarity
%   sigmaDistance  Coefficient for computing spatial location similarity
%   radius   Maximum distance considered before weight matrix value is zero

W = nCutComputeW(I, sigmaColor, sigmaDistance, radius);

[imageRegions]= spectralKmeansPartition(I, W, 100, nClusters, 50, 0.0, 5);
imageSegments= cell(length(nClusters));
imageRegions2 = zeros(size(I));
for i=1 : nClusters
    imageSegments{i} = zeros(size(I));
    segIndeces = (imageRegions==i);
    segIndeces = logical(cat(3,segIndeces,segIndeces,segIndeces));
    imageSegments{i}(segIndeces) = I(segIndeces);
    imageRegions2(segIndeces) = getColor(I(segIndeces));
end
imageRegions = imageRegions2;
end

function[color] = getColor(ipatch)
color = ones(size(ipatch));
n = size(ipatch,1);
ipatch = reshape(ipatch, n/3,3);
avg = mean(ipatch);
color(1:n/3) = avg(1,1);
color((n/3)+1:(2*n)/3) = avg(1,2);
color((2*n/3)+1: n) = avg(1,3);
end