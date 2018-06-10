#include "myl.h"

int printStr(char *str){
	int i = 0,count,size;
	
	while(*(str+i) != '\0'){
		i++;
	}
	
	count = i;
	char string1[count+1];
	i=0;
	
	while(*(str+i) != '\0'){
		string1[i] = *(str+i);
		i++;
	}
	string1[i] = '\0';
	size = count+1;
	
	asm volatile(
		"movl $1, %%eax \n\t"
		"movq $1, %%rdi \n\t"
		"syscall \n\t"
		:
		:"S"(string1), "d"(size)
	);
	
	return count;
}

int readInt(int *n){
	char str[50];
	int bytes = 64,a=0,i,j;
	asm volatile(
		"movl $0, %%eax \n\t"
		"movq $1, %%rdi \n\t"
		"syscall \n\t"
		:
		:"S"(str), "d"(bytes)
	);
	
	if(str[0]!='-' && (str[0]-'0'<0 || str[0]-'0'>9))return 0;				//
	for(j=1;;j++){															//
		if(str[j]-'0'==(-38))break;											//RETURN 0; means FAILURE.
		if(str[j]-'0'<0 || str[j]-'0'>9)return 0;							//
	}																		//
	
	if(str[0]=='-')i=1;
	else i=0;
	
	while(1){
		if(str[i]-'0'<=9 && str[i]-'0'>=0){
			a = 10*a;
			a += (str[i] - '0');
		}
		else break;
		i++;
	}
	if(str[0]=='-')(*n) = (-1)*a;
	else (*n) = a;
	
	return 1;
}

int printInt(int a){
	char num[64],temp[64];
	int i=0,t,j,k,size;
	if(a==0)num[0] = '0';
	else{
		if(a<0){
			num[i] = '-';
			a = (-1)*a;
			i++;
		}
		
		t=a;
		
		while(t){
			num[i] = (char)(48+t%10);
			t /= 10;
			i++;
		}
		
		if(num[0]=='-'){
			j = 1;
			temp[0] = '-';
		}
		else j = 0;
		
		k = i-1;
		while(j<i){
			temp[j] = num[k];
			j++;
			k--;
		}
		temp[j] = '\0';
	}
	
	size = i;
	
	asm volatile(
		"movl $1, %%eax \n\t"
		"movq $1, %%rdi \n\t"
		"syscall \n\t"
		:
		:"S"(temp), "d"(size)
	);
	
	return size;
}

int readFlt(float *f){ 
	char str[50];
	int size = 64,i,j,flag = 1,af=1;
	float a = 0.0,mplier=10.0;
	asm volatile(
		"movl $0, %%eax \n\t"
		"movq $1, %%rdi \n\t"
		"syscall \n\t"
		:
		:"S"(str), "d"(size)
	);
	
	if(str[0]!='-' && str[0]!='.' && (str[0]-'0'<0 || str[0]-'0'>9))return 0;		//
	else {																			//	
		if(str[0]=='.')af=0;														//
	}																				//
																					//
	for(j=1;;j++){																	//
		if(str[j]-'0'==(-38))break;													//RETURN 0; means FAILURE.
		if(str[j]!='.' && (str[j]-'0'<0 || str[j]-'0'>9))return 0;									//
		if(str[j]=='.'){															//
			if(af==0)return 0;														//
			else af=0;																//
		}																			//
	}																				//
	
	if(str[0]=='-')i=1;
	else i=0;
	
	while(1){
		if((str[i]-'0')<=9 && (str[i]-'0')>=0){
			if(flag==1){
				a = 10.0*a;
				a += (float)(str[i] - '0');
			}
			else{
				a += (((float)(str[i] - '0')) / mplier);
				mplier *= 10.0;
			}
		}
		else{
			if(str[i]=='.')flag = 0;
			else break;
		}
		i++;
	}
	
	if(str[0]=='-')(*f) = (-1.0)*a;
	else (*f) = a;
	
	return 1;
}

int printFlt(float f){
	char str[2],temp[7];
	str[0] = '.';
	str[1] = '\0';
	float b;
	int a = (int)f,i,c1;
	b = f - a;
	if(b<0.0)b = -b;
	c1 = printInt(a);
	
	asm volatile(
		"movl $1, %%eax \n\t"
		"movq $1, %%rdi \n\t"
		"syscall \n\t"
		:
		:"S"(str), "d"(2)
	);
	
	for(i=0;i<6;i++){
		a = (int)(10*b);
		temp[i] = '0' + a;
		b = (10*b)-a;
	}
	temp[7] = '\0';
	
	asm volatile(
		"movl $1, %%eax \n\t"
		"movq $1, %%rdi \n\t"
		"syscall \n\t"
		:
		:"S"(temp), "d"(7)
	);
	
	return c1+7;
}
