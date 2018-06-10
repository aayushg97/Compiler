#include <stdio.h>
#include "myl.h"

int main(){	
	char str[50];
	int c;
	printf("Enter a string \n");
	scanf(" %[^\n]",str);
	printf("Using printStr :- ");
	c = printStr(str);
	printf("\n");
	printf("No of characters printed :- %d \n",c);
	
	int *n,a=0;
	n = &a;	
	printf("Enter an integer \n");
	c = readInt(n);
	
	if(c){
		c = printInt(*n);
		printf("No of characters printed :- %d \n",c);
	}
	else printf("Invalid number \n");
	
	float *f,b=0.0;
	f = &b;	
	printf("Enter a float \n");
	c = readFlt(f);
	
	if(c){
		c = printFlt(*f);
		printf("No of characters printed :- %d \n",c);
	}
	else printf("Invalid number \n");
}
