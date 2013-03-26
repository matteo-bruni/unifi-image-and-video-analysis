function [imgGlrRows imgGlrCols] = normalizeCoordSet(imgMatrix)
%NORMALIZECOORDSET Summary of this function goes here
%   Detailed explanation goes here
    
    % get mean x and mean y of the matrix
    
    imgGlrRows = imgMatrix(:,1);
    imgGlrCols = imgMatrix(:,2);
    
    meanRows = mean(imgGlrRows);
    meanCols = mean(imgGlrCols);
    
    imgGlrRows = imgGlrRows - meanRows;
    imgGlrCols = imgGlrCols - meanCols;
    
    
    % NB normalizzazione vengono valori tra 0 e 1
    % non vanno bene per distanze tipo manhattan 
    % in quel caso è necessario moltiplicare per 100
    delta = max(imgGlrRows)-min(imgGlrRows);
    
    imgGlrRows = imgGlrRows*100/delta;
    imgGlrCols = imgGlrCols*100/delta;
   
   
    % modifichiamo scala
    % spostiamo coord di riga delta=max-minriga
    % si divide tutte le coordinate di riga dei due vettori per delta

end

