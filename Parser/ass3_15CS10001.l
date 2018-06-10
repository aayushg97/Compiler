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
	#include <stdio.h>
	#include <string.h>
	#include <math.h>
	#define ID 258
	#define CONST 259
	#define SLIT 260
	#define ARROW 261
	#define INC 262
	#define DEC 263
	#define SHL 264
	#define SHR 265
	#define LTE 266
	#define GTE 267
	#define EQ 268
	#define NEQ 269
	#define AND 270
	#define OR 271
	#define ADDC 272
	#define ANDC 273
	#define MODC 274
	#define MULC 275
	#define DIVC 276
	#define ORC 277
	#define SHLC 278
	#define SHRC 279
	#define SUBC 280
	#define XORC 281
	#define DOTAPOS 282
	#define BOOL 283
	#define BREAK 284
	#define CASE 285
	#define CHAR 286
	#define CONTINUE 287
	#define DEFAULT 288
	#define DO 289
	#define DOUBLE 290
	#define IF 291
	#define ELSE 292
	#define FLOAT 293
	#define FOR 294
	#define GOTO 295
	#define INT 296
	#define LONG 297
	#define MATRIX 298
	#define RETURN 299
	#define SIGNED 300
	#define SWITCH 301
	#define UNSIGNED 302
	#define VOID 303
	#define WHILE 304
	#define SHORT 305
	extern int yylval;
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

{ID_NDIG}(({ID_NDIG}|{DIG})*)	{return ID;}

({NZ_DIG}({DIG})*)|({FRAC_CONS}|({FRAC_CONS}{EXP})|({DIG_SEQ}{EXP}))|('({C_CHAR})+')|"0"	{return CONST;}

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
