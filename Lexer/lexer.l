KEY "unsigned"|"break"|"return"|"void"|"case"|"float"|"short"|"char"|"for"|"signed"|"while"|"goto"|"Bool"|"continue"|"if"|"default"|"do"|"int"|"switch"|"double"|"long"|"else"|"Matrix"
ID_NDIG [a-zA-Z]
DIG [0-9]
NZ_DIG [1-9]
SIGN "+"|"-"
DIG_SEQ [0-9]+
FRAC_CONS ("."([0-9]+))|(([0-9]+)"."([0-9]+))|(([0-9]+)".")
EXP ("e"([0-9]+))|("e"("+"|"-")([0-9]+))|("E"([0-9]+))|("E"("+"|"-")([0-9]+))
ESC_SEQ "\""|"\'"|"\?"|"\\"|"\a"|"\b"|"\f"|"\n"|"\r"|"\t"|"\v"
C_CHAR ([^'\\\n])|("\\\"")|("\\'")|("\\?")|("\\\\")|("\\a")|("\\b")|("\\f")|("\\n")|("\\r")|("\\t")|("\\v")

PUNC ("["|"{"|"}"|"]"|"("|")"|"."|"->"|"++"|"--"|"&"|"*"|"+"|"-"|"~"|"!"|"/"|"%"|"<<"|">>"|"<"|">"|"<="|">="|"=="|"!="|"^"|"|"|"&&"|"||"|"?"|":"|";"|"="|"*="|"/="|"%="|"+="|"-="|"<<="|">>="|"&="|"^="|"|="|","|"#"|"'")

%{
	#include <stdio.h>
	#include <string.h>
	#include <math.h>
	#define KEY 1
	#define ID 2
	#define CONST 3
	#define SLIT 4
	#define PUNC 5
%}

%x comment
%x comment1

%%	
{KEY}	{return KEY;}

{ID_NDIG}|(({ID_NDIG}|{DIG})*)	{return ID;}

({NZ_DIG}({DIG})*)|({FRAC_CONS}|({FRAC_CONS}{EXP})|({DIG_SEQ}{EXP}))|('({C_CHAR})+')|"0"	{return CONST;}

(\"({C_CHAR})*\")	{return SLIT;}

"/*"	{BEGIN(comment);}
<comment>"*/"	{BEGIN(INITIAL);}
"//"	{BEGIN(comment1);}
<comment1>[\n]	{BEGIN(INITIAL);}

{PUNC} {return PUNC;}

[ _\n]	{}
.	{if((int)yytext[0]!=13)printf("Invalid Entity %s \n",yytext);}
<comment,comment1>(.|[\n])	{}
%%

int yywrap(){
	return 1;
}
