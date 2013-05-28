function[imageRegions]= stdKmeans(I, nClusters)

flatImg = double(reshape(I,size(I,1)*size(I,2),size(I,3)));
idx = kmeans(flatImg,nClusters);

Regions = reshape(idx,size(I,1),size(I,2));
%Regions = Regions.* (255/ nClusters);
Regions = cat(3,Regions,Regions, Regions);

imageRegions2 = zeros(size(I));
for i=1 : nClusters
    segIndeces = (Regions==i);
    segIndeces = logical(segIndeces);
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