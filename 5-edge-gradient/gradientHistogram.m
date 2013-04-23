function [ gradHisto ] = gradientHistogram( imgMag, imgQTheta, nTheta, rMin, ...
                                                    cMin, rMax, cMax )
%GRADIENTHISTOGRAM Summary of this function goes here
%   Detailed explanation goes here

    threshold = 0.1;

    gradHisto = zeros(1,nTheta);
    for i=rMin:rMax
        for j=cMin:cMax
            
            if(imgMag(i,j) > threshold)
                gradHisto(imgQTheta(i,j)+1) = gradHisto(imgQTheta(i,j)+1) + imgMag(i,j);
            end
            
        end
    end
    gradHisto = gradHisto ./ sum(gradHisto(:));

end

