function [Segments] = nCut(iIndex, W, Img, Segments, nMax, cutThreshold)
N = length(W);
d = sum(W, 2);
D = spdiags(d, 0, N, N);

%perturbing D for numerical stability
sigma= 1 * 10 ^ -15;
D= D + sigma*speye(size(D));

DW= sparse(D-W);

nEigen= 5;
warning off;
OPTS.maxit= 10000;
OPTS.tol= 0.000001;
[eigVectors,eigValues] = eigs(DW, D, nEigen, 'sm');
eigCurrent= eigVectors(:,2);
eigBest = eigCurrent;
tBest = median(eigCurrent);
tBest= fminsearch('nCutCutValue', tBest, [], eigCurrent, W, D);
bestCutValue= nCutCutValue(tBest, eigBest, W, D);
t = tBest;
A = find(eigCurrent > t);
B = find(eigCurrent <= t);

for i = 3:nEigen
    eigCurrent= eigVectors(:,i);
    t = median(eigCurrent);
    t = fminsearch('nCutCutValue', t, [], eigCurrent, W, D);
    currentCutValue = nCutCutValue(t, eigCurrent, W, D);
    if (currentCutValue < bestCutValue)% && length(A) > minArea && length(B)> minArea
        bestCutValue = currentCutValue;
        A = find(eigCurrent > t);
        B = find(eigCurrent <= t);
    end
end

%length(A) < sArea || length(B) < sArea)
if (bestCutValue < cutThreshold && nMax ~= 0&&length(A) > 100 && length(B) > 100)% && length(A) > minArea && length(B)> minArea)
    Segments = nCut(iIndex(B), W(B, B), Img, Segments, nMax -1, cutThreshold);
    Segments = nCut(iIndex(A), W(A, A), Img, Segments, nMax -1, cutThreshold);
else
    Segments{length(Segments)+1}= iIndex;
end
end

