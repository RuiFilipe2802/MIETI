#include <stdio.h>
#include <stdlib.h>

int max(int a,int b,int c,int d,int e){

        if(a<b && a<c && a<d && a<e){
            return (a);
        }
        if(b<a && b<c && b<d && b<e){
            return (b);
        }
        if(c<a && c<b && c<d && c<e){
            return (c);
        }
        if(d<a && d<b && d<c && d<e){
            return (d);
        }
        if(e<a && e<b && e<c && e<d){
            return (e);
        }
}

int min(int a,int b,int c,int d,int e){

        if(a>b && a>c && a>d && a>e){
            return (a);
        }
        if(b>a && b>c && b>d && b>e){
            return (b);
        }
        if(c>a && c>b && c>d && c>e){
            return (c);
        }
        if(d>a && d>b && d>c && d>e){
            return (d);
        }
        if(e>a && e>b && e>c && e>d){
            return (e);
        }
}

int media(int ma,int mi){
        return ((ma+mi)/2);
        }


int main(){
    int ma=0;
    int mi=0;
    int a=0;
    int b=0;
    int c=0;
    int d=0;
    int e=0;
    float medi=0;

    printf("escreva 5 numeros:\n");
    scanf("%d %d %d %d %d",&a,&b,&c,&d,&e);

    ma=max(a,b,c,d,e);
    mi=min(a,b,c,d,e);
    medi=media(ma,mi);

    printf("a media entre o minimo e o maximo e %f",medi);

}




