%First parameter sets the size of the numbers
%Other parameter sets the variance of the numbers (interval between min and
%max)

%--------PLOT ONE------:

mu = [2,3];
sigma = [40,1.5;1.5,100];
rng default  % For reproducibility
r1 = mvnrnd(mu,sigma,700);

r3 = normrnd(8,10,700);
m1 = r3(:,1:2)

plot(r1(:,1),r1(:,2),'O')
hold on;
plot(m1(:,1),m1(:,2),'X')
size(r3)


%--------PLOT TWO------:

% mu = [2,30];
% sigma = [4000,1.5;1.5,12000];
% rng default  % For reproducibility
% r1 = mvnrnd(mu,sigma,1000);
% 
% r3 = normrnd(150,120,700)
% m1 = r3(:,1:2)
% 
% plot(r1(:,1),r1(:,2),'O')
% hold on;
% plot(m1(:,1),m1(:,2),'X')
% 
% dlmwrite('cluster3.txt',r1, 'delimiter', ' ')
% dlmwrite('cluster4.txt',m1, 'delimiter', ' ')
