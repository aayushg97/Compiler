%{
	#include <stdio.h>
	#define YYDEBUG 1
	extern int yylex();
	int yylineno;
	char *yytext;
	int yyerror(char *s);
%}

%token ID CONST SLIT
%token ARROW INC DEC SHL SHR LTE GTE EQ NEQ AND OR ADDC ANDC MODC MULC DIVC ORC SHLC SHRC SUBC XORC DOTAPOS
%token BOOL BREAK CASE CHAR CONTINUE DEFAULT DO DOUBLE IF ELSE FLOAT FOR GOTO INT LONG MATRIX RETURN SIGNED SWITCH UNSIGNED VOID WHILE SHORT
%start transunit

%%

primexp: ID			{printf("primary-expression -> identifier \n");}								
	| CONST			{printf("primary-expression -> constant \n");}
	| SLIT			{printf("primary-expression -> string-literal \n");}
	| '(' exp ')'	{printf("primary-expression -> (expression) \n");}
	;

postexp:    primexp					{printf("postfix-expression -> primary-expression \n");}
	| postexp '[' exp ']'			{printf("postfix-expression -> postfix-expression [ expression ] \n");}
	| postexp '(' argexplist ')'	{printf("postfix-expression -> postfix-expression ( argument-expression-list ) \n");}
	| postexp '(' ')'				{printf("postfix-expression -> postfix-expression ( ) \n");}
	| postexp '.' ID				{printf("postfix-expression -> postfix-expression . identifier \n");}
	| postexp ARROW ID				{printf("postfix-expression -> postfix-expression -> identifier \n");}
	| postexp INC					{printf("postfix-expression -> postfix-expression ++ \n");}
	| postexp DEC					{printf("postfix-expression -> postfix-expression -- \n");}
	| postexp DOTAPOS				{printf("postfix-expression -> postfix-expression .' \n");}
	;

argexplist:    assignexp			{printf("argument-expression-list -> assignment-expression \n");}
	| argexplist ',' assignexp		{printf("argument-expression-list -> argument-expression-list , assignment-expression \n");}
	;

unaryexp:    postexp				{printf("unary-expression -> postfix-expression \n");}
	| INC unaryexp					{printf("unary-expression -> ++ unary-expression \n");}
	| DEC unaryexp					{printf("unary-expression -> -- unary-expression \n");}
	| unaryop castexp				{printf("unary-expression -> unary-operator cast-expression \n");}
	;

unaryop:    '&'						{printf("unary-operator -> & \n");}
	| '*'							{printf("unary-operator -> * \n");}
	| '+'							{printf("unary-operator -> + \n");}
	| '-'							{printf("unary-operator -> - \n");}
	;

castexp:    unaryexp				{printf("cast-expression -> unary-expression \n");}
	;

multexp:    castexp					{printf("multiplicative-expression -> cast-expression \n");}
	| multexp '*' castexp			{printf("multiplicative-expression -> multiplicative-expression * cast-expression \n");}
	| multexp '/' castexp			{printf("multiplicative-expression -> multiplicative-expression / cast-expression \n");}
	| multexp '%' castexp			{printf("multiplicative-expression -> multiplicative-expression %% cast-expression \n");}
	;

addexp:    multexp					{printf("additive-expression -> multiplicaive-expression \n");}
	| addexp '+' multexp			{printf("additive-expression -> additive-expression + multiplicaive-expression \n");}
	| addexp '-' multexp			{printf("additive-expression -> additive-expression - multiplicaive-expression \n");}
	;

shiftexp:    addexp					{printf("shift-expression -> additive-expression \n");}
	| shiftexp SHL addexp			{printf("shift-expression -> shift-expression << additive-expression \n");}
	| shiftexp SHR addexp			{printf("shift-expression -> shift-expression >> additive-expression \n");}
	;

relexp:    shiftexp					{printf("relational-expression -> shift-expression \n");}
	| relexp '<' shiftexp			{printf("relational-expression -> relational-expression < shift-expression \n");}
	| relexp '>' shiftexp			{printf("relational-expression -> relational-expression > shift-expression \n");}
	| relexp LTE shiftexp			{printf("relational-expression -> relational-expression <= shift-expression \n");}
	| relexp GTE shiftexp			{printf("relational-expression -> relational-expression >= shift-expression \n");}
	;

eqexp:    relexp					{printf("equality-expression -> relational-expression \n");}
	| eqexp EQ relexp				{printf("equality-expression -> equality-expression == relational-expression \n");}
	| eqexp NEQ relexp				{printf("equality-expression -> equality-expression != relational-expression \n");}
	;

andexp:    eqexp					{printf("AND-expression -> equality-expression \n");}
	| andexp '&' eqexp				{printf("AND-expression -> AND-expression & equality-expression \n");}
	;

xorexp:    andexp					{printf("exclusive-OR-expression -> AND-expression \n");}
	| xorexp '^' andexp				{printf("exclusive-OR-expression -> exclusive-OR-expression ^ AND-expression \n");}
	;

orexp:    xorexp					{printf("inclusive-OR-expression -> exclusive-OR-expression \n");}
	| orexp '|' xorexp				{printf("inclusive-OR-expression -> inclusive-OR-expression | exclusive-OR-expression \n");}
	;

logandexp:    orexp					{printf("logical-AND-expression -> inclusive-OR-expression \n");}
	| logandexp AND orexp			{printf("logical-AND-expression -> logical-AND-expression && inclusive-OR-expression \n");}
	;

logorexp:    logandexp 				{printf("logical-OR-expression -> logical-AND-expression \n");}
	| logorexp OR logandexp			{printf("logical-OR-expression -> logical-OR-expression || logical-AND-expression \n");}
	;

condexp:    logorexp				{printf("conditional-expression -> logical-OR-expression \n");}
	| logorexp '?' exp ':' condexp	{printf("conditional-expression -> logical-OR-expression ? expression : conditional-expression \n");}
	;

assignexp:    condexp				{printf("assignment-expression -> conditional-expression \n");}
	| unaryexp assignop assignexp	{printf("assignment-expression -> unary-expression assignment-operator assignment-expression \n");}
	;

assignop:    '='	{printf("assignment-operator -> = \n");}
	| MULC			{printf("assignment-operator -> *= \n");}
	| DIVC			{printf("assignment-operator -> /= \n");}
	| MODC			{printf("assignment-operator -> %%= \n");}
	| ADDC			{printf("assignment-operator -> += \n");}
	| SUBC			{printf("assignment-operator -> -= \n");}
	| SHLC			{printf("assignment-operator -> <<= \n");}
	| SHRC			{printf("assignment-operator -> >>= \n");}
	| ANDC			{printf("assignment-operator -> &= \n");}
	| XORC			{printf("assignment-operator -> ^= \n");}
	| ORC			{printf("assignment-operator -> |= \n");}
	;

exp:    assignexp			{printf("expression -> assignment-expression \n");}
	| exp ',' assignexp		{printf("expression -> expression , assignment-expression");}
	;

constexp:    condexp		{printf("constant-expression -> conditional-expression \n");}
	;


dection:    dectionspec initdectorlist ';'	{printf("declaraton -> declaration-specifiers init-declarator-list \n");}
	| dectionspec ';'						{printf("declaraton -> declaration-specifiers \n");}
	;

dectionspec:    typespec dectionspec		{printf("declaration-specifiers -> type-specifier declaration-specifiers \n");}
	| typespec								{printf("declaration-specifiers -> type-specifier \n");}
	;

initdectorlist:    initdector				{printf("init-declarator-list -> init-declarator \n");}
	| initdectorlist ',' initdector			{printf("init-declarator-list -> init-declarator-list , init-declarator \n");}
	;

initdector:    dector						{printf("init-declarator -> declarator \n");}
	| dector '=' initializer				{printf("init-declarator -> declarator = initializer \n");}
	;

typespec:    VOID		{printf("typespec -> void \n");}
	| CHAR				{printf("type-specifier -> char \n");}
	| SHORT				{printf("type-specifier -> short \n");}
	| INT				{printf("type-specifier -> int \n");}
	| LONG				{printf("type-specifier -> long \n");}
	| FLOAT				{printf("type-specifier -> float \n");}
	| DOUBLE			{printf("type-specifier -> double \n");}
	| MATRIX			{printf("type-specifier -> Matrix \n");}
	| SIGNED			{printf("type-specifier -> signed \n");}
	| UNSIGNED			{printf("type-specifier -> unsigned \n");}
	| BOOL				{printf("type-specifier -> Bool \n");}
	;

dector:    directdector			{printf("declarator -> direct-declarator \n");}
	| pointer directdector		{printf("declarator -> pointer direct-declarator \n");}
	;	

directdector:	ID							{printf("direct-declarator -> identifier \n");}
	| '(' dector ')'						{printf("direct-declarator -> (declarator) \n");}
	| directdector '[' assignexp ']'		{printf("direct-declarator -> direct-declarator [ assignment-expression ] \n");}
	| directdector '[' ']'					{printf("direct-declarator -> direct-declarator [ ] \n");}
	| directdector '(' paramtypelist ')'	{printf("direct-declarator -> direct-declarator ( parameter-type-list ) \n");}
	| directdector '(' idlist ')'			{printf("direct-declarator -> direct-declarator ( identifier-list ) \n");}
	| directdector '(' ')'					{printf("direct-declarator -> direct-declarator ( ) \n");}
	;

pointer:	'*'			{printf("pointer -> * \n");}
	| '*' pointer		{printf("pointer -> * pointer \n");}
	;

paramtypelist:	paramlist	{printf("parameter-type-list -> parameter-list \n");}
	;

paramlist:	paramdection			{printf("parameter-list -> parameter-declaration \n");}
	| paramlist ',' paramdection		{printf("parameter-list -> parameter-list , parameter-declaration \n");}
	;

paramdection:	dectionspec dector	{printf("parameter-declaration -> declaration-specifiers declarator \n");}
	| dectionspec					{printf("parameter-declaration -> declaration-specifiers \n");}
	;

idlist:	ID				{printf("identifier-list -> identifier \n");}
	| idlist ',' ID		{printf("identifier-list -> identifier-list , identifier \n");}
	;

initializer:	assignexp			{printf("initializer -> assignment-expression \n");}
	| '{' initializerrowlist '}'	{printf("initializer -> { initializer-row-list } \n");}
	;

initializerrowlist:	initializerrow				{printf("initializer-row-list -> initializer-row \n");}
	| initializerrowlist ';' initializerrow		{printf("initializer-row-list -> initializer-row-list ; initializer-row \n");}
	;

initializerrow:	initializer						{printf("initializer-row -> initializer \n");}
	| denation initializer						{printf("initializer-row -> designation initializer \n");}
	| initializerrow ',' denation initializer	{printf("initializer-row -> initializer-row , designation initializer \n");}
	| initializerrow ',' initializer			{printf("initializer-row -> initializer-row , initializer \n");}
	;

denation:	denatorlist '='			{printf("designation -> designator-list = \n");}
	;

denatorlist:	denator				{printf("designator-list -> designator \n");}
	| denatorlist denator			{printf("designator-list -> designator-list designator \n");}
	;

denator:	'[' constexp ']'		{printf("designator -> [ constant-expression ] \n");}
	| '.' ID						{printf("designator -> . identifier \n");}
	;


stmt:	lblstmt			{printf("statement -> labeled-statement \n");}
	| compstmt			{printf("statement -> compound-statement \n");}
	| expstmt			{printf("statement -> expression-statement \n");}
	| selstmt			{printf("statement -> selection-statement \n");}
	| itrstmt			{printf("statement -> iteration-statement \n");}
	| jumpstmt			{printf("statement -> jump-statement \n");}
	;

lblstmt:	ID ':' stmt			{printf("labeled-statement -> identifier : statement \n");}
	| CASE constexp ':' stmt	{printf("labeled-statement -> case constant-expression : statement \n");}
	| DEFAULT ':' stmt			{printf("labeled-statement -> default : statement \n");}
	;

compstmt:	'{' '}'				{printf("compound-statement -> { } \n");}
	| '{' blockitemlist '}'		{printf("compound-statement -> { block-item-list } \n");}
	;

blockitemlist:	blockitem			{printf("block-item-list -> block-item \n");}
	| blockitemlist blockitem		{printf("block-item-list -> block-item-list block-item \n");}
	;

blockitem:	dection			{printf("block-item -> declaration \n");}
	| stmt					{printf("block-item -> statement \n");}
	;

expstmt:	exp ';'			{printf("expression-statement -> expression ; \n");}
	| ';'					{printf("expression-statement -> ; \n");}
	;

selstmt:	IF '(' exp ')' stmt			{printf("selection-statement -> if ( expression ) statement \n");}
	| IF '(' exp ')' stmt ELSE stmt		{printf("selection-statement -> if ( expression ) statement else statement \n");}
	| SWITCH '(' exp ')' stmt			{printf("selection-statement -> switch ( expression ) statement \n");}
	;

itrstmt:	WHILE '(' exp ')' stmt						{printf("iteration-statement -> while ( expression ) statement \n");}
	| DO stmt WHILE '(' exp ')' ';'						{printf("iteration-statement -> do statement while ( expression ) ; \n");}
	| FOR '(' expopt ';' expopt ';' expopt ')' stmt		{printf("iteration-statement -> for ( expression-opt ; expression-opt ; expression-opt ) statement \n");}
	| FOR '(' dection expopt ';' expopt ')' stmt		{printf("iteration-statement -> for ( declaration expression-opt ; expression-opt ) statement \n");}
	;

expopt: 				{printf("expression-opt -> \n");}
	| exp				{printf("expression-opt -> expression \n");}
	;

jumpstmt:	GOTO ID ';'		{printf("jump-statement -> goto identifier ; \n");}
	| CONTINUE ';'			{printf("jump-statement -> continue ; \n");}
	| BREAK ';'				{printf("jump-statement -> break ; \n");}
	| RETURN expopt ';'		{printf("jump-statement -> return expression-opt \n");}
	;


transunit:	extdection			{printf("translation-unit -> external-declaration \n");}
	| transunit extdection		{printf("translation-unit -> translation-unit external-declaration \n");}
	;

extdection:	funcdef			{printf("external-declaration -> function-definition \n");}
	| dection				{printf("external-declaration -> declaration \n");}
	;

funcdef:	dectionspec dector dectionlist compstmt		{printf("function-definition -> declaration-specifiers declarator declaration-list compound-statement \n");}
	| dectionspec dector compstmt						{printf("function-definition -> declaration-specifiers declarator compound-statement \n");}
	;

dectionlist:	dection			{printf("declaration-list -> declaration \n");}
	| dectionlist dection		{printf("declaration-list -> declaration-list declaration \n");}
	;

%%

yyerror(char *s){
	printf("%s at %s %d\n",s,yytext,yylineno);
}
