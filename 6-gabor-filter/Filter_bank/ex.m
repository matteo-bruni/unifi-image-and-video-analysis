psi     = [0 pi/2];
gamma   = 0.5;
%numero di valori per theta
N = 4;
theta = zeros(N);
%numero di valori per lambda
M = 2; 
lambda = zeros(M);
%numero di valori per sigma
P = 2;
sigma = zeros(P);
%creazione dei vettori di valori per i tre parametri
for i =1:N
   theta(i+1) =  theta(i) + pi/N;
end

for i =1:M
   lambda(i) =  i * 2;
end

for i =1:P
   sigma(i) = 2^i;
end


filter_number = N*P*M;
feature_vector= zeros(2*filter_number, size(galleryNames,1));
for j = 1:size(galleryNames, 1)  
    k = 1;
    img_in = im2double(imread(galleryNames{j}));
    for n = 1:N
        for m = 1:M
            for p = 1:P
                [e_mean, e_var] = E_Features( img_in, theta(n),lambda(m),sigma(p), psi, gamma);   %E_Feature calcola l'energia e restituisce le sue features
                feature_vector(k,j) = e_mean;
                feature_vector(k+1,j) = e_var;
                k = k+2;
            end
        end
    end
end

for i = 1:2*filter_number
    m = mean(feature_vector(i,:));
    sigma = std(feature_vector(i,:));
    for j = 1:size(galleryNames, 1)  
        feature_vector(i,j) =  (feature_vector(i,j)- m)/ sigma;
    end
end


distMatrix = zeros(size(galleryNames, 1),size(galleryNames, 1));

for i = 1:size(galleryNames, 1)  
    for j = 1: size(galleryNames, 1)  
        distMatrix(i,j)= norm(feature_vector(:,i)-feature_vector(:,j));
    end
end

distMatrix