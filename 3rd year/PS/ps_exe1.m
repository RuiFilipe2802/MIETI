syms t   
sum=0;
w0=(2*pi)/pi;
y=t/pi;     %x(t)
a0=(2/pi)*int(y,t,0,pi);

for n=1:20
    %coeficientes
    an=(2/pi)*int(y*cos(n*w0*t),t,0,pi);
    fprintf("%f\n", an);   %print an
    bn=(2/pi)*int(y*sin(n*w0*t),t,0,pi);
    fprintf("%f\n", bn);   %print bn
    cn=sqrt(an^2+bn^2);
    %somatório
    sum=sum+(an*cos(n*w0*t)+bn*sin(n*w0*t)); 
end

fplot(t,y,[0,pi]);
grid on;hold on;
fplot(t,(sum+(a0/2)),[0,pi]);
