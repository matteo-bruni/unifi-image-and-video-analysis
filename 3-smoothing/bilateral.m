function [val] = bilateral(inputData)

s = size(inputData,1);
i =  uint8((s+1)/2);
j =  uint8((s+1)/2);
sigma_d = i/3;
sigma_r = 30/3;

domainKernel = zeros(size(inputData,1),size(inputData,1));
rangekernel = zeros(size(inputData,1),size(inputData,1));

for k=1:s
    for l=1:s
        domainKernel(k,l) = -(  ((i-k)^2+(j-l)^2)) / (2*(sigma_d)^2 ) ;
        rangekernel(k,l) = -(norm(inputData(i,j) - inputData(k,l))^2 / (2*sigma_r^2));
    end
end

w = exp(domainKernel).*exp(rangekernel);

w = w./sum(w(:));

temp = inputData.*w;

val = uint8(sum(temp(:)));
