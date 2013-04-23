function [ rMin, rMax, cMin, cMax ] = gridBounds( size, numGridRows, numGridCols, k )
%GRIDBOUNDS Summary of this function goes here
%   Detailed explanation goes here
    
    deltaRows = uint16( floor(single(size(1,1)) / single(numGridRows)));
    deltaCols = uint16( floor(single(size(1,2)) / single(numGridCols)));
    
    
    gridCollIndex = mod(k, numGridCols);
    gridRowIndex = floor( single(k) / single(numGridCols));
    
    % WARNING gridCollIndex and gridRowIndex
    % takes values 0..gridCollIndex-1 0..gridRowIndex-1
    
    rMin = 1 + uint16(deltaRows) * uint16(gridRowIndex);
    rMax = rMin + deltaRows - 1;
    
    cMin = 1 + uint16(deltaCols) * uint16(gridCollIndex);
    cMax = cMin + deltaCols - 1;
    
    
end

