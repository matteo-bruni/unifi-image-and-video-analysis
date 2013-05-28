function [imageRegions] = gmmImageSegment(image, nClusters, W)
image = image/255;
if nargin < 3
    W = [3 3 3 1 1];
end
if nargin < 2
    nClusters = 3;
end

% import an image
img_RGB = image;
Ncol = size(img_RGB,2);
Nrow = size(img_RGB,1);

cform = makecform('srgb2lab');
img_LAB = applycform(img_RGB,cform);

I = img_LAB;
I = double(I)/255;
Y = reshape(I(:),Ncol*Nrow,[]);

[xI,yI] = meshgrid(1:Ncol,1:Nrow);

xI = xI(:); yI = yI(:);
xI = xI/max(xI,[],1);
yI = yI/max(yI,[],1);

Y = [Y xI yI];
Y = Y(:,W~=0);
W = W(W~=0);

Y = Y - repmat( mean(Y,1), length(Y), 1);
Y = Y./repmat(std(Y,1), length(Y), 1);

W = W/sum(W,2)*10;
Y = Y.*repmat(W,size(Y,1),1);

options = statset('MaxIter',1000);
gmm_obj = gmdistribution.fit(Y,nClusters,'Regularize',1e-4,'Replicates',1,'Options',options);

gmm_posterior = posterior(gmm_obj,Y);
[max_postr, class_result] = max(gmm_posterior,[],2);

I_label = reshape(class_result,Nrow,Ncol,[]);
I_post = reshape(max_postr,Nrow,Ncol,[]);

Regions = I_label;


I = image*255;
Regions = cat(3,Regions,Regions, Regions);

imageRegions2 = zeros(size(I));
for i=1 : nClusters
    segIndeces = (Regions==i);
    segIndeces = logical(segIndeces);
    imageRegions2(segIndeces) = getColor(I(segIndeces));
end
imageRegions = imageRegions2;

% I_tmp = double(label2rgb(I_label))/255;
% imageRegions = I_tmp;
% 
% imageRegions(:,:,1) = I_tmp(:,:,1).* I_post;
% imageRegions(:,:,2) = I_tmp(:,:,2).* I_post;
% imageRegions(:,:,3) = I_tmp(:,:,3).* I_post;
% 
% imageRegions = imageRegions.* (255);
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
% I_tmp = double(label2rgb(I_label))/255;
% imageRegions = I_tmp;
% 
% imageRegions(:,:,1) = I_tmp(:,:,1).* I_post;
% imageRegions(:,:,2) = I_tmp(:,:,2).* I_post;
% imageRegions(:,:,3) = I_tmp(:,:,3).* I_post;
% 
% imageRegions = imageRegions.* (255);


