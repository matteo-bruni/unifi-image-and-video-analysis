function [ out ] = isMax( inputData )
%ISMAX Summary of this function goes here
%   Detailed explanation goes here
    central = uint8( (size(inputData) + 1) / 2 );
    if (inputData(central(1), central(2)) < 0.8)
        out = 0;
    elseif (inputData(central(1), central(2)) == max(inputData(:)) )
        out = 1;
    else
        out = 0;
    end
    
end

