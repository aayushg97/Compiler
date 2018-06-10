#include <stdio.h>
#include "mylex.h"

int main(){
	yyin = fopen("ass3_test_2.mm","r");
	//FILE *fp = fopen("lexout.txt","w");
	int token;
	
	while(token=yylex()){
		switch(token){
			case 1: printf("<Keyword, %s> \n",yytext); break;
			case 2: printf("<Identifier, %s> \n",yytext); break;
			case 3: printf("<Constant, %s> \n",yytext); break;
			case 4: printf("<String Literal, %s> \n",yytext); break;
			case 5: printf("<Punctuator, %s> \n",yytext); break;
		}
	}
	
	//fclose(fp);
}
