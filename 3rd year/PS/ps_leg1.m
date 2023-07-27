syms t  %variavel t
sum=0;  %variável somatório
y=t;    %x(t)

for n=1:20
    f=factorial(n);
    g=((t^(2))-1)^(n);
    p=(1/((2^(n))*f))*(diff(g,n));   %cálculo dos polinómios
    fprintf("%s\n", p);              %print dos polinómios
    c=(((2*n)+1)/2)*(int(y*p,t,-1,1));
    sum=sum+(p*c);                   %cálculo do somatório
end

fplot(t,y,[0,1]);                    %gráfico de x(t)
grid on;hold on;
fplot(t,(sum),[0,1]);                %gráfico da aproximação a x(t)

