function [centers,assignments,error] = spectralKmeans(data,k,n,t,count)
er=inf;
data = data';
for c=1:count
    [cen,a,e]= kmKernel(data,k,n,t);
    if sum(e) < er;
        er= sum(e);
        centers= cen;
        assignments= a;
        error= e;
    end
end

function [centers,assignments,error] = kmKernel(data,k,n,t)
% data is a array with each column a data point in some metric
% space
% k is the number of centers
% n is the maximum number of iterations
% t is the change threshold

% random initialization

[N1,N2] = size(data);



permutation=randperm(N2);
centers=data(:,permutation(1:k));

change=t+1;
E=sum(data.^2,1)';
Jerr=Inf;
itercount=0;

while (change> t) && (itercount < n);

  F=sum(centers.^2,1)';
  T=E*ones(1,k) + ones(N2,1)*F'-2*data'*centers;
  [mind,minc]= min(T');
  change=Jerr-sum(mind);
  Jerr=sum(mind);

  
  for i = 1:k;
    [I,J]=find(minc==i);
    centers(:,i)=sum(data(:,J),2)/(length(J)+eps);
  end
  itercount=itercount+1;
  
end
F=sum(centers.^2,1)';
T=E*ones(1,k) + ones(N2,1)*F'-2*data'*centers;
[error,assignments]=min(T');
