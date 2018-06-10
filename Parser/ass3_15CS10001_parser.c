#include <stdio.h>
#ifndef _MYL_H
#define _MYL_H
#define ERR 0
#define OK 1
int yyparse(void);
int yydebug;
FILE *yyin;
char *yytext;
#endif

int main(){
	int rule;
	//yyin = fopen("ass3_test_1.mm","r");
	//yydebug = 1;
	yyparse();
	//fclose(yyin);
	return 0;
}
