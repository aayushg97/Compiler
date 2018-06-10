KEY "unsigned"|"break"|"return"|"void"|"case"|"float"|"short"|"char"|"for"|"signed"|"while"|"goto"|"Bool"|"continue"|"if"|"default"|"do"|"int"|"switch"|"double"|"long"|"else"|"Matrix"
ID_NDIG [_a-zA-Z]
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
	#include "ass4_15CS10001_translator.h"
	#include "ass4_15CS10001.tab.h"
%}

%option noyywrap
%option yylineno
%x comment1

%%	
"unsigned"	{return UNSIGNED;}
"break"		{return BREAK;}
"return"	{return RETURN;}
"void"		{return VOID;}
"case"		{return CASE;}
"float"		{return FLOAT;}
"short"		{return SHORT;}
"char"		{return CHAR;}
"for"		{return FOR;}
"signed"	{return SIGNED;}
"while"		{return WHILE;}
"goto"		{return GOTO;}
"Bool"		{return BOOL;}
"continue"	{return CONTINUE;}
"if"		{return IF;}
"default"	{return DEFAULT;}
"do"		{return DO;}
"int"		{return INT;}
"switch"	{return SWITCH;}
"double"	{return DOUBLE;}
"long"		{return LONG;}
"else"		{return ELSE;}
"Matrix"	{return MATRIX;}

{ID_NDIG}(({ID_NDIG}|{DIG})*)	{yylval.idt.lexeme = new string(yytext); return ID;}

({NZ_DIG}({DIG})*)|(("0")+)		{yylval.val_int = atoi(yytext); return INT_CONST;}

({FRAC_CONS}|({FRAC_CONS}{EXP})|({DIG_SEQ}{EXP}))	{yylval.val_float = atof(yytext); return FLOAT_CONST;}

('({C_CHAR})+')		{yylval.val_char = yytext[1]; return CHAR_CONST;}

(\"({C_CHAR})*\")	{return SLIT;}

"/*"([^"*/"]*)"*/"	{}
"/*"([^"*/"]*)		{return 1000;}


"//"	{BEGIN(comment1);}
<comment1>[\n]	{BEGIN(INITIAL);}

"["		{return '[';}
"{"		{return '{';}
"}"		{return '}';}
"]"		{return ']';}
"("		{return '(';}
")"		{return ')';}
"."		{return '.';}
"->"	{return ARROW;}
"++"	{return INC;}
"--"	{return DEC;}
"&"		{return '&';}
"*"		{return '*';}
"+"		{return '+';}
"-"		{return '-';}
"~"		{return '~';}
"!"		{return '!';}
"/"		{return '/';}
"%"		{return '%';}
"<<"	{return SHL;}
">>"	{return SHR;}
"<"		{return '<';}
">"		{return '>';}
"<="	{return LTE;}
">="	{return GTE;}
"=="	{return EQ;}
"!="	{return NEQ;}
"^"		{return '^';}
"|"		{return '|';}
"&&"	{return AND;}
"||"	{return OR;}
"?"		{return '?';}
":"		{return ':';}
";"		{return ';';}
"="		{return '=';}
"*="	{return MULC;}
"/="	{return DIVC;}
"%="	{return MODC;}
"+="	{return ADDC;}
"-="	{return SUBC;}
"<<="	{return SHLC;}
">>="	{return SHRC;}
"&="	{return ANDC;}
"^="	{return XORC;}
"|="	{return ORC;}
","		{return ',';}
"#"		{return '#';}
".'"	{return DOTAPOS;}

[ \n]	{}
.	{if((int)yytext[0]>=32)return 0;}
<comment1>(.|[\n])	{}
%%
