function main(imageName, sigmaColor, sigmaDistance, radius, threshold, methodName, YUV, nClusters)

%imageSegments= array of images representing the segments found
%Regions= image containing the visual representation of the image segments
%Recommended parameters values for spectral tecniques: sigmaColor =0.01 sigmaDistance = 1.7 radius = 5

if nargin < 6
    disp('!Arguments missing')
    return
end

image= strcat('./Images/',imageName,'.jpg');

if ~exist(image, 'file')
    disp('!Specified file does not exist')
else
    I = imread(image);
    I = double(I);
    if YUV== true
        I = RGB2YUV(I);
    end
    %I = I/255;
    
    handleDirectoryTree(methodName, imageName);
    
    imageSegments = {};
    switch methodName
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        case 'nCut'
            [imageSegments, Regions] = nCutWrapper(I, sigmaColor, sigmaDistance, radius, threshold);  %tested ok
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        case 'spectralKmeans'
            if nargin< 7
                disp('!nClusters not specified')
                return
            end
            [imageSegments,Regions] =  spectralKmeansImageSegment(I, sigmaColor, sigmaDistance, radius, nClusters); %tested ok
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        case 'kmeans'
            if nargin< 7
                disp('!nClusters not specified')
                return
            end
            Regions = stdKmeans(I, nClusters);    %tested ok
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        case 'watershed'
            Regions = watershedImageSegment(I);    %tested ok
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        case 'gmm'
            if nargin< 7
                disp('!nClusters not specified')
                return
            end
            Regions = gmmImageSegment(I,nClusters);  %tested ok
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        otherwise
            disp('!The methodName is invalid')
            return
    end
    
    %saving results to file
    imwrite(uint8(Regions),strcat('./Results/',methodName,'/',imageName,'/',imageName,'_0','.jpg'),'jpg');
    for i=1:length(imageSegments)
        if  YUV== true
            imageSegments{i}= YUV2RGB(imageSegments{i});
        end
        imwrite(uint8(imageSegments{i}),strcat('./Results/',methodName,'/',imageName,'/',imageName,'_',int2str(i),'.jpg'),'jpg');
    end
    
end
end

function handleDirectoryTree(methodName, imageName)
if (~exist(strcat('./Results/',methodName),'dir'))
    mkdir(strcat('./Results/',methodName));
end
if exist(strcat('./Results/',methodName,'/',imageName),'dir')
    rmdir(strcat('./Results/',methodName,'/',imageName),'s');
end
mkdir(strcat('./Results/',methodName,'/',imageName));

end



function YUV  = RGB2YUV(RGB)

R = RGB(:,:,1);
G = RGB(:,:,2);
B = RGB(:,:,3);
Y = 0.299 * R + 0.587 * G + 0.114 * B;
U = -0.14713 * R - 0.28886 * G + 0.436 * B;
V = 0.615 * R - 0.51499 * G - 0.10001 * B;
YUV = cat(3,Y,U,V);

end

function RGB = YUV2RGB(YUV)
Y = YUV(:,:,1);
U = YUV(:,:,2);
V = YUV(:,:,3);
R = Y + 1.139834576 * V;
G = -.5805942338 * V -.3946460533 * U +  Y;
B = 1 * Y + 2.032111938 * U ;
RGB = cat(3,R,G,B);
end
