function W = nCutComputeW(I, sigmaColor, sigmaDistance, radius)

[nRow, nCol, c] = size(I);
N = nRow * nCol;
W = sparse(N,N);

iTmp= reshape(I, N, 1, c);

X = cat(3, repmat((1:nRow)', 1, nCol), repmat((1:nCol), nRow, 1));
X = reshape(X, N, 1, 2);

for ic=1:nCol
    for ir=1:nRow
        jc = (ic - floor(radius)) : (ic + floor(radius));
        jr = ((ir - floor(radius)) :(ir + floor(radius)))';
        jc = jc(jc >= 1 & jc <= nCol);
        jr = jr(jr >= 1 & jr <= nRow);
        
        i = ir + (ic - 1) * nRow;
        j = repmat(jr, 1, length(jc)) + repmat((jc -1) * nRow, length(jr), 1);
        j = reshape(j, length(jc) * length(jr), 1);
        
        XJ = X(j, 1, :);
        XI = repmat(X(i, 1, :), length(j), 1);
        DX = XI - XJ;
        DX = sum(DX .* DX, 3);
        
        constraint = find(sqrt(DX) <= radius);
        j = j(constraint);
        DX = DX(constraint);
        
        FJ = iTmp(j, 1, :);
        FI = repmat(iTmp(i, 1, :), length(j), 1);
        DF = FI - FJ;
        DF = sum(DF .* DF, 3);
        
        SISquared= sigmaColor^2;
        SXSquared= sigmaDistance^2;
        W(i, j) = exp(-DF / SISquared) .* exp(-DX / SXSquared);
    end
end
end