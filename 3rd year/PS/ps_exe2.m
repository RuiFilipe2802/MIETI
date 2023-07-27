syms t
sum=0;
w0=(2*pi)/(2*pi);
y1=-1;  %x(t)
y2=1;
a0=(2/(2*pi))*((int(y1,t,-pi,0))+(int(y2,t,0,pi)));

for n=1:20
    %coeficientes
    an=(2/(2*pi))*(int(y1*cos(n*w0*t),t,-pi,0)+int(y2*cos(n*w0*t),t,0,pi));
    bn=(2/(2*pi))*(int(y1*sin(n*w0*t),t,-pi,0)+int(y2*sin(n*w0*t),t,0,pi));
    fprintf("%f\n", bn);
    cn=sqrt(an^2+bn^2);
    sum=sum+(an*cos(n*w0*t)+bn*sin(n*w0*t));  %somatorio
end

fplot(y1,[-pi,0]);
grid on;hold on;
fplot(y2,[0,pi]);
grid on;hold on;
fplot(t,(sum+a0),[-pi,pi]);
