%First parameter sets the size of the numbers
%Other parameter sets the variance of the numbers (interval between min and
%max)

%--------PLOT ONE------:

% mu = [1,3];
% sigma = [10,1.5;1.5,3];
% rng default  % For reproducibility
% r1 = mvnrnd(mu,sigma,10000);
% 
% r3 = normrnd(15,30,10000);
% m1 = r3(:,1:2)
% 
% plot(r1(:,1),r1(:,2),'O')
% hold on;
% plot(m1(:,1),m1(:,2),'X')
%size(r1)


%--------PLOT TWO------:

mu = [2,3];
sigma = [40,1.5;1.5,120];
rng default  % For reproducibility
r1 = mvnrnd(mu,sigma,8000);

mu2 = [100,100];
sigma2 = [40,1.5;1.5,180];
rng default  % For reproducibility
r2 = mvnrnd(mu2,sigma2,8000);

merged1 = [r1(:,1), r1(:,2), r2(:,1), r2(:,2)]

mu3 = [2,100];
sigma3 = [40,1.5;1.5,120];
rng default  % For reproducibility
r3 = mvnrnd(mu3,sigma3,8000);

mu4 = [100,2];
sigma4 = [40,1.5;1.5,80];
rng default  % For reproducibility
r4 = mvnrnd(mu4,sigma4,8000);

merged2 = [r3(:,1), r3(:,2), r4(:,1), r4(:,2)]

plot(r1(:,1),r1(:,2),'O')
hold on;
plot(r2(:,1),r2(:,2),'X')
hold on;
plot(r3(:,1),r3(:,2),'o')
hold on;
plot(r4(:,1),r4(:,2),'x')

%dlmwrite('clusterspara1.txt',merged1, 'delimiter', ' ')
%dlmwrite('clusterspara2.txt',merged2, 'delimiter', ' ')
