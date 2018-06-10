#include <stdio.h>
#ifndef _MYL_H
#define _MYL_H
#define ERR 0
#define OK 1
int yylex();
FILE *yyin;
char *yytext;
#endif

int main(){
	//yyin = fopen("ass3_test_1.mm","r");
	//FILE *fp = fopen("ass3_test_1_tokens_15CS10001","w");
	int token;
	
	while(token=(int)yylex()){
		if(token >= 283 && token <= 305){
			printf("Keyword \n");
		}
		else{
			if(token >= 261 && token <= 282){
				printf("Punctuator \n");
			}
			else{
				if((token>=33 && token<=47 && token!=34 && token!=36 && token!=39) || (token>=58 && token<=63) || token==91 || token==93 ||token==94  || (token>=123 && token<=126)){
					printf("Punctuator \n");
				}
				else{
					switch(token){
						case 258: printf("Identifier \n"); break;
						case 259: printf("Constant \n"); break;
						case 260: printf("String Literal \n"); break;
						case 0: printf("Failed to tokenize the file \n"); return 0; break;
						case 1000: printf("Unterminated comment \n"); return 0; break;
					}	
				}
			}
		}
	}
	
	//fclose(yyin);
	//fclose(fp);
}
/*91|93|123|125|40|41|46|38|42|43|45|126|33|47|37|60|62|94|124|63|58|59|61|44|35*/
