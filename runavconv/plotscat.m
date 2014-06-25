
r=0.482;
 t=0:.01:2*pi;
 m = csvread('a.csv', 1,0);
E4 = m(:,2);
x = m(:,4);
y = m(:,5);
z = m(:,6);
runav = m(:,7);
conv = m(:,8);


%E4m = [y z E4];
%runavm = [y z runav];
%convm = [y z conv];




f1 = figure;
scatter(y, z, 50, log(E4),'fill','s');

axis equal; 
axis on;
axis tight;
colorbar;

print(f1,'-dpdf','a.pdf');

f2 = figure;
scatter(y, z, 50, log(runav),'fill','s');
hold on;           
plot(r*sin(t),r*cos(t));
hold off;          

axis equal; 
axis on;
axis tight;
colorbar;

print(f2,'-dpdf','a.pdf');

f3 = figure;
scatter(y, z, 50, conv,'fill','s');
hold on;           
plot(r*sin(t),r*cos(t));
hold off;          

axis equal; 
axis on;
axis tight;
colorbar;

print(f3,'-dpdf','-append','a.pdf');
print(f3,'-dpng','-r600','a.png');