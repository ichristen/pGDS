% v = VECTOR(1,2);
% u = VECTOR(3,-1);
% 
% w = u + v;
% 
% disp(u);
% disp(v);
% disp(w);

braggperiod = .6372/2/((2.417 + 1)/2);

braggperiod
braggperiod*3

numlayers = 5;

% r = [.5 .65 .8 .95 1.1 1.25];
r = (0:(2*numlayers - 1)) * braggperiod/2 + braggperiod*2.25;

dev = hyperCavityQuarter_device(.325, pi/2, r, .1, 1, .1);

% bigDev = DEVICE('hyperCavity');
% 
% % dev * pGDS.mirrorX
% 
% bigDev.add(dev);
% bigDev.add(dev * pGDS.mirrorX);
% bigDev.add(dev * pGDS.mirrorY);
% bigDev.add(dev * pGDS.mirrorX * pGDS.mirrorY);
    
dev.exportLibraryGDS("/Users/i/Desktop/hyper.gds");