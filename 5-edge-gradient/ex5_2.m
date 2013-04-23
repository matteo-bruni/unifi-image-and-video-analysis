templateHisto = computeGradientHistogram('hiero_01/01.png', 2, 2, 6);

queryHisto = computeGradientHistogram('hiero_02/01.png', 2, 2, 6);
norm( templateHisto-queryHisto )% = 0.4013
queryHisto = computeGradientHistogram('hiero_02/02.png', 2, 2, 6);
norm( templateHisto-queryHisto ) %= 0.5552
