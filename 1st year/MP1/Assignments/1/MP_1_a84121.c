#include <stdio.h>
#include <stdlib.h>

int main() {
    float precosemiva=0;
    float precocomiva=0;
      scanf("%f",&precosemiva);
      precocomiva=precosemiva+precosemiva*0.23;
         if (precocomiva<=100){
            printf("o preco do produto e %f",precocomiva);
           }else{
         if (precocomiva>100){
                    printf("o produto e caro");
             }
         }
}
