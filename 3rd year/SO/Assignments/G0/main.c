#include "header.h"
#include <stdio.h>
#include <stdlib.h>

int main(){

	int vector[5]={0,0,0,0,0};
	int first, second, third, fourth, fifth;
	for(int i=0;i<5;i++){
		printf("Values Before:%d\n", vector[i]);
	}

	printf("First number:");
	scanf("\n%d", &first);
	printf("Second number");
	scanf("\n%d", &second);
	printf("Third number:");
	scanf("\n%d", &third);
	printf("Fourth Number:");
	scanf("\n%d", &fourth);
	printf("Fifth number:");
	scanf("\n%d", &fifth);

	fill(vector, 0, first);
	fill(vector, 1, second);
	fill(vector, 2, third);
	fill(vector, 3, fourth);
	fill(vector, 4, fifth);

	//find(vector, 2, 12);

	for(int i=0;i<5;i++){
		printf("Values After:%d\n", vector[i]);
	}
}
