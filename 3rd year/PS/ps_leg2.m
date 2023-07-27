syms t  %variavel t
sum=0;  %variável somatório
y1=-1;  %x(t) , -1<=x<=0
y2=1;   %x(t) ,  0<=x<=1

for n=1:20
    f=factorial(n);
    g=((t^(2))-1)^(n);
    p=(1/((2^(n))*f))*(diff(g,n));   %cálculo dos polinómios
    fprintf("%s\n", p);              %print dos polinómios
    c=(((2*n)+1)/2)*((int(y1*p,t,-1,0))+(int(y2*p,t,0,1)));
    sum=sum+(p*c);                   %cálculo do somatório
end

fplot(-1,[-1,0]);       %gráfico de x(t) negativo
grid on;hold on;        
fplot(1,[0,1]);         %gráfico de x(t) positivo
grid on;hold on;
fplot(t,(sum),[-1,1]);  %gráfico da aproximação a x(t)

