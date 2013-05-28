function [Segments] = spectralKmeansPartition(Img, W, nEigen, nClusters, nIter, kThreshold, kRandomizedRestarts)

[N1, N2, N3]= size(Img);
N = length(W);
d = sum(W, 2);
D = spdiags(d, 0, N, N);

%perturbing D for numerical stability
sigma= 1 * 10 ^ -11;
D= D + sigma*speye(size(D));

DW= D-W;
DW= sparse(DW);

[eigVectors,eigValues] = eigs(DW, D, nEigen, 'sm');

[centers,Segments,error]= spectralKmeans(eigVectors,nClusters,nIter,kThreshold,kRandomizedRestarts);
Segments= reshape(Segments,[N1 N2]);
end

