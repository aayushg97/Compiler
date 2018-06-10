/*
	SYMBOL								ATTRIBUTES
	ID (Identifier)					'addr' (a pointer to entry of this Identifier in the Symbol Table) 
									'lexeme' (holds the lexeme of this identifier)
								
	INT_CONST						INT_CONST holds the integer value i.e. the lexeme that matches this token
	
	FLOAT_CONST						FLOAT_CONST holds the integer value i.e. the lexeme that matches this token
	
	CHAR_CONST						CHAR_CONST holds the integer value i.e. the lexeme that matches this token
	
	exp (expression)				addr - address of the temporary in the symbol table representing this expression
									type - type of this expression
									array - stores address of base of array if the expression is of type Matrix
									truelist - list of indices of goto instructions that are generated when this bool expression is true
									falselist - list of indices of goto instructions that are generated when this bool expression is false
								
	Following symbols have attributes same as exp (expression) above :-
		a) primexp (primary-expression)
		b) postexp (postfix-expression) 
		c) unaryexp (unary-expression)
		d) castexp (cast-expression)
		e) multexp (multiplicative-expression)
		f) addexp (additive-expression)
		g) shiftexp (shift-expression) 
		h) relexp (relational-expression)
		i) eqexp (equality-expression) 
		j) andexp (and-expression)
		k) xorexp (exclusive-or-expression)
		l) orexp (exclusive-or-expression)
		m) logandexp (logical-and-expression)
		n) logorexp (logical-or-expression)
		o) condexp (conditional-expression)
		p) assignexp (assignment-expression)
		q) exp (expression)
		r) constexp (constant-expression)
		s) initdector (init-declarator)
		t) initdectorlist (init-declarator-list)
		u) directdector (direct-declarator)
		v) dector (declarator)
		w) expstmt (expression-statement)
		x) expopt (expression-optional)
		
	initializer 								val - pointer to a string which is the initialized value of a variable
												express - a structure of type expattr (has attributes same as expression)
												
	initializerrow 								val - pointer to a string which is the initialized value of a variable
												express - a structure of type expattr (has attributes same as expression)
	
	initializerrowlist							val - pointer to a string which is the initialized value of a variable
												express - a structure of type expattr (has attributes same as expression)
	
	argexplist (argument-expression-list)		head - a list of pointer where each pointer points to an expression
												pnum - an integer which represents the number of arguments
												
	unaryop (unary-operator)					unaryop is a character that stores one of '&', '*', '+', '-'
	
	M											stores index of next instruction in the quad array
	
	N											a list of indices to instructions in the quad array
	
	Following symbols have attributes same as N above :-
		a) stmt (statement)
		b) compstmt (compound statement)
		c) selstmt (selection statement)
		d) itrstmt (iteration statement)
		e) jumpstmt (jump statement)
		f) blockitemlist
		g) blockitem
		
	dectionspec (declaration-specifier)			type - to store a type
												width - to store width of above type
												
	typespec (type-specifier)					type - to store a type
												width - to store width of above type
											
	pointer										type - to store a type
												width - to store width of above type

*/

%{
	#include "ass4_15CS10001_translator.h"
	#define YYDEBUG 1
	extern int yylex();
	extern char *yytext;
	void yyerror(const char *s);
	mtype *T;
	int W;
	int diff;
%}

%union{
	idattr idt;			// to store attributes of an identifier 
	int val_int;		
	list *after;		// to store attributes of statements
	float val_float;
	int dummy;
	char val_char;
	typeattr tspec;		// to store attributes of type-specifiers
	paramlist plist;	// to store attributes of argument-expression-list
	string *word;
	initial ini;		// to store attributes of initializer
	expattr expr;		// to store attributes of an expression 
}

%token <idt> ID  
%token <val_int> INT_CONST 
%token <val_float> FLOAT_CONST 
%token <val_char> CHAR_CONST 
%token ZERO_CONST SLIT 
%token ARROW INC DEC SHL SHR LTE GTE EQ NEQ AND OR ADDC ANDC MODC MULC DIVC ORC SHLC SHRC SUBC XORC DOTAPOS
%token BOOL BREAK CASE CHAR CONTINUE DEFAULT DO DOUBLE IF ELSE FLOAT FOR GOTO INT LONG MATRIX RETURN SIGNED SWITCH UNSIGNED VOID WHILE SHORT

%type <expr> primexp postexp unaryexp castexp multexp addexp shiftexp relexp eqexp andexp xorexp orexp logandexp logorexp condexp assignexp exp constexp initdector initdectorlist directdector dector expstmt expopt

%type <ini> initializer initializerrow initializerrowlist  

%type <plist> argexplist

%type <val_char> unaryop

%type <dummy> M  

%type <after> N stmt lblstmt compstmt selstmt itrstmt jumpstmt blockitemlist blockitem 

%type <tspec> dectionspec typespec pointer

%start transunit

%left '+' '-'
%left '*' '/' '%'

%%

/*EXPRESSIONS*/
primexp: ID			{	
						// Check if it is a global variable/function
						entrySym *p = curr_symtab->find(*($1.lexeme));
						if(p){
							if(p->type->isarr==1){
								//It is an array
								$$.array = p;
								$$.type = p->type;
								$$.addr = tableSym::gentemp();  		// Temporary for offset of array
								mtype *t;
								t = new mtype();
								t->ntype = "int";
								t->isarr = 0;
								t->width = int_width;
								t->next = NULL;
								($$.addr)->type = t;					// $$.addr stores offset of array
								($$.addr)->initval = "0";
								($$.addr)->size = int_width; 
								curr_symtab->offset += ($$.addr)->size;	
								quadArray::emit(($$.addr)->initval,($$.addr)->name);   // Copy instruction for array offset 
							}
							else{
								// Not an array. 
								$$.type = p->type;        // Set the attributes of primexp i.e. $$
								$$.addr = p;			
								$$.array = NULL;
							}
						}
						else{
							// it is a global variable/function
							p = global_symtab->find(*($1.lexeme));
							if(p->type->ntype!="function"){
								if(p->type->isarr==1){
									//It is an array
									$$.array = p;
									$$.type = p->type;
									$$.addr = tableSym::gentemp();  		// Temporary for offset of array
									mtype *t;
									t = new mtype();
									t->ntype = "int";
									t->isarr = 0;
									t->width = int_width;
									t->next = NULL;
									($$.addr)->type = t;					// $$.addr stores offset of array
									($$.addr)->initval = "0";
									($$.addr)->size = int_width; 
									curr_symtab->offset += ($$.addr)->size;	
									quadArray::emit(($$.addr)->initval,($$.addr)->name);   // Copy instruction for array offset 
								}
								else{
									// Not an array. 
									$$.addr = p;			
									$$.type = p->type;        // Set the attributes of primexp i.e. $$
									$$.array = NULL;
								}
							}
							else{
								//Set the attributes of primexp i.e. $$
								$$.addr = p;  
								$$.type = p->type;
								$$.array = NULL;
							}
						}
						
						$$.point = 0;
						$$.matop = 0;
					}
													
	| INT_CONST		{	
						$$.addr = tableSym::gentemp();							// generate temporary
						($$.addr)->type = new mtype();							//
						($$.addr)->type->ntype = "int";							//
						($$.addr)->type->isarr = 0;								// set the type
						($$.addr)->type->width = int_width;						//
						($$.addr)->type->next = NULL;							//
						char str[10];
						sprintf(str,"%d",$1);									// set initial value
						string str1(str);
						($$.addr)->initval = str1;
						($$.addr)->size = ($$.addr)->type->width;				// set size
						curr_symtab->offset += ($$.addr)->size;					// update total offset of the symbol table
						quadArray::emit(($$.addr)->initval,($$.addr)->name);	// emit quad that represents initialization instruction
						$$.type = ($$.addr)->type;								// 
						$$.array = NULL;										// set the attributes of $$
						$$.point = 0;											//
						$$.matop = 0;											//
					}
					
	| FLOAT_CONST	{	
						$$.addr = tableSym::gentemp();							// generate temporary
						($$.addr)->type = new mtype();							//
						($$.addr)->type->ntype = "double";						//
						($$.addr)->type->isarr = 0;								// set the type
						($$.addr)->type->width = double_width;					//
						($$.addr)->type->next = NULL;							//
						char str[50];
						sprintf(str,"%.3f",$1);									// set initial value
						string str1(str);
						($$.addr)->initval = str1;
						($$.addr)->size = ($$.addr)->type->width;				// set size
						curr_symtab->offset += ($$.addr)->size;					// update total offset of the symbol table
						quadArray::emit(($$.addr)->initval,($$.addr)->name);	// emit quad that represents initialization instruction
						$$.type = ($$.addr)->type;								//
						$$.array = NULL;										// set the attributes of $$
						$$.point = 0;											//
						$$.matop = 0;											//
					}
					
	| CHAR_CONST	{		
						$$.addr = tableSym::gentemp();							// generate temporary
						($$.addr)->type = new mtype();							//
						($$.addr)->type->ntype = "char";						//
						($$.addr)->type->isarr = 0;								// set the type
						($$.addr)->type->width = char_width;					//
						($$.addr)->type->next = NULL;							//
						char str[50];
						sprintf(str,"%c",$1);									// set initial value
						string str1(str);
						($$.addr)->initval = str1;
						($$.addr)->size = ($$.addr)->type->width;				// set size
						curr_symtab->offset += ($$.addr)->size;					// update total offset of the symbol table
						quadArray::emit(($$.addr)->initval,($$.addr)->name);	// emit quad that represents initialization instruction
						$$.type = ($$.addr)->type;								//
						$$.array = NULL;										// set the attributes of $$
						$$.point = 0;											//
						$$.matop = 0;											//
					}
	
	| SLIT			{}
	
	| '(' exp ')'	{$$ = $2;}
	;

postexp:    primexp					{
										$$ = $1;
										if($$.array)$$.matop = 1;			// a matrix operation may be performed
									}

	| postexp '[' exp ']'			{	
										$$.array = $1.array;				// set base address of array
										$$.type = $1.type->next;			// set type
										$$.point = $1.point;
										$$.matop = 0;						// matrix operation is not being performed
										$1.matop = 0;
										
										$$.addr = tableSym::gentemp();						// generate temporary for offset of array	
										($$.addr)->type = new mtype();						//
										($$.addr)->type->ntype = "int";						//
										($$.addr)->type->isarr = 0;							// set type
										($$.addr)->type->width = int_width;					//
										($$.addr)->type->next = NULL;						//
										($$.addr)->size = int_width;						// set size
										curr_symtab->offset += ($$.addr)->size;				// update total offset of symbol table

										entrySym *t = tableSym::gentemp();					// generate temporary
										t->type = new mtype();								//
										t->type->ntype = "int";								//
										t->type->isarr = 0;									// set type
										t->type->width = int_width;							//
										t->type->next = NULL;								//
										t->size = int_width;								// set size
										curr_symtab->offset += t->size;						// update total offset of symbol table
										
										char str[10];
										sprintf(str,"%d",($$.type)->width);
										
										quadArray::emit("*",str,($3.addr)->name,t->name);				// quad to set the value
										quadArray::emit("+",($1.addr)->name,t->name,($$.addr)->name); // of array offset					
									}
									
	| postexp '(' argexplist ')'	{	
										$$.addr = tableSym::gentemp();					// generate temporary
										($$.addr)->type = ($1.type)->rtype;				// set type
										($$.addr)->size = ($1.type)->rtype->width;		// set size
										$$.type = ($1.type)->rtype;						// 			
										$$.array = NULL;								// set attributes of $$ 
										$$.point = $1.point;							//
										$$.matop = 0;									// matrix operation is not being performed
										$1.matop = 0;									//
										
										// update total offset of symbol table
										if(($$.addr)->size>=0)curr_symtab->offset += ($$.addr)->size;			
										
										parpmlist *p = $3.head;
										
										while(p){
											quadArray::emit("param","",p->express.addr->name);	// emit quads of function parameters
											p = p->next;
										}
										
										char str[10];
										sprintf(str,"%d",$3.pnum);
										quadArray::emit("call",($1.addr)->name,str,($$.addr)->name);	// emit quad of function call
									}
									
	| postexp '(' ')'				{	
										$$.addr = tableSym::gentemp();					// generate temporary
										($$.addr)->type = ($1.type)->rtype;				// set type
										($$.addr)->size = ($1.type)->rtype->width;		// set size
										$$.type = ($1.type)->rtype;						//
										$$.array = NULL;								// set attributes
										$$.point = $1.point;							//
										$$.matop = 0;									// matrix operation is not being performed
										$1.matop = 0;									//
										
										// update total offset of symbol table
										if(($$.addr)->size>=0)curr_symtab->offset += ($$.addr)->size;			
										
										quadArray::emit("call",($1.addr)->name,"0",($$.addr)->name);	// emit quad of function call
									}
									
	| postexp '.' ID				{}
	| postexp ARROW ID				{}
	
	| postexp INC					{	
										$$.addr = tableSym::gentemp();					// generate temporary
										($$.addr)->type = $1.type;						// set type
										($$.addr)->size = ($1.type)->width;				// set size
										$$.type = $1.type;								//
										$$.array = NULL;								// set attributes
										$$.point = $1.point;							//
										$$.matop = 0;									// matrix operation is not being performed
										$1.matop = 0;									//
										curr_symtab->offset += ($$.addr)->size;
										
										entrySym *t = tableSym::gentemp();
										t->type = $1.type;	
										t->size = ($1.type)->width;	
										curr_symtab->offset += t->size;
										
										if($1.array){
											// emit quads of indexed copy and arithmetic instructions
											quadArray::emit("rindexed",($1.array)->name,($1.addr)->name,($$.addr)->name);
											quadArray::emit("rindexed",($1.array)->name,($1.addr)->name,t->name);
											quadArray::emit("+",t->name,"1",t->name);
											quadArray::emit("lindexed",($1.addr)->name,t->name,($1.array)->name);
										}
										else{
											// emit quads of arithmetic and copy instructions
											quadArray::emit(($1.addr)->name,($$.addr)->name);
											quadArray::emit("+",($1.addr)->name,"1",t->name);
											quadArray::emit(t->name,($1.addr)->name);
										}																	
									}
									
	| postexp DEC					{	
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;	
										($$.addr)->size = ($1.type)->width;
										$$.type = $1.type;
										$$.array = NULL;
										$$.point = $1.point;	
										$$.matop = 0;									// matrix operation is not being performed
										$1.matop = 0;									//
										curr_symtab->offset += ($$.addr)->size;
										
										entrySym *t = tableSym::gentemp();
										t->type = $1.type;	
										t->size = ($1.type)->width;	
										curr_symtab->offset += t->size;
										
										if($1.array){
											// emit quads of indexed copy and arithmetic instructions
											quadArray::emit("rindexed",($1.array)->name,($1.addr)->name,($$.addr)->name);
											quadArray::emit("rindexed",($1.array)->name,($1.addr)->name,t->name);
											quadArray::emit("-",t->name,"1",t->name);
											quadArray::emit("lindexed",($1.addr)->name,t->name,($1.array)->name);
										}
										else{
											// emit quads of arithmetic and copy instructions
											quadArray::emit(($1.addr)->name,($$.addr)->name);
											quadArray::emit("-",($1.addr)->name,"1",t->name);
											quadArray::emit(t->name,($1.addr)->name);
										}
									}
									
	| postexp DOTAPOS				{	
										$$.array = tableSym::gentemp();
										$$.type = new mtype();			// set type of transpose of postexp i.e. $1
										($$.type)->ntype = "array";
										($$.type)->width = ($1.type)->width;
										($$.type)->isarr = 1;
										($$.type)->numelem = ($1.type)->next->numelem; 		// Reverse row and column sizes
										($$.type)->next = new mtype();						// in transpose matrix i.e. $$
										($$.type)->next->ntype = "array"; 
										($$.type)->next->numelem = ($1.type)->numelem;		
										($$.type)->next->width = ($$.type)->next->numelem * ($1.type)->next->next->width;
										($$.type)->next->isarr = 1;
										($$.type)->next->next = new mtype();
										($$.type)->next->next->ntype = ($1.type)->next->next->ntype;
										($$.type)->next->next->width = ($1.type)->next->next->width;
										($$.type)->next->next->next = NULL;
										($$.type)->next->next->isarr = 0;
										
										($$.array)->type = $$.type;						//
										($$.array)->size = ($$.type)->width;			// symbol table entries
										curr_symtab->offset += ($$.array)->size;		//
										
										$$.point = 0;		//
										$$.addr = NULL;		// set other attributes
										$$.matop = 1;		//
										
										quadArray::emit(".'",($1.array)->name,($$.array)->name); // emit quad of matrix transpose operation
									}
	;

argexplist:    assignexp			{	
										// Insertion of $1 in list of expressions in $$
										$$.head = new parpmlist();
										($$.head)->express = $1;
										($$.head)->next = NULL;
										$$.pnum = 1;
									}
									
	| argexplist ',' assignexp		{	
										// Insertion of $3 at the end of list of expressions in $$
										parpmlist *p,*q;	
										p = $1.head;	
										q = p;		
										while(p){
											q = p;
											p = p->next;
										}	
										
										p = new parpmlist();
										p->express = $3;
										p->next = NULL;
										q->next = p;
										$1.pnum = $1.pnum + 1;
										$$ = $1;
									}
	;

unaryexp:    postexp				{$$ = $1;}

	| INC unaryexp					{	
										$$.addr = tableSym::gentemp();				// generate temporary
										($$.addr)->type = $2.type;					// set type in the symbol table
										($$.addr)->size = ($2.type)->width;			// set size in the symbol table
										$$.type = $2.type;							//
										$$.array = NULL;							// set attributes of $$
										$$.point = $2.point;						//
										$$.matop = $2.matop;						//
										curr_symtab->offset += ($$.addr)->size;		// update total offset in symbol table
										
										entrySym *t = tableSym::gentemp();
										t->type = $2.type;	
										t->size = ($2.type)->width;	
										curr_symtab->offset += t->size;
										
										if($2.array){
											// emit quads of indexed copy and arithmetic instructions
											quadArray::emit("rindexed",($2.array)->name,($2.addr)->name,t->name);
											quadArray::emit("+",t->name,"1",t->name);
											quadArray::emit("lindexed",($2.addr)->name,t->name,($2.array)->name);
											quadArray::emit(t->name,($$.addr)->name);
										}
										else{
											// emit quads of arithmetic and copy instructions
											quadArray::emit("+",($2.addr)->name,"1",t->name);
											quadArray::emit(t->name,($2.addr)->name);
											quadArray::emit(t->name,($$.addr)->name);
										}
									}
									
	| DEC unaryexp					{	
										$$.addr = tableSym::gentemp();				// generate temporary
										($$.addr)->type = $2.type;					// set type in the symbol table
										($$.addr)->size = ($2.type)->width;			// set size in the symbol table
										$$.type = $2.type;							//
										$$.array = NULL;							// set attributes of $$
										$$.point = $2.point;						//
										$$.matop = $2.matop;						//
										curr_symtab->offset += ($$.addr)->size;		// update total offset in symbol table
										
										entrySym *t = tableSym::gentemp();
										t->type = $2.type;	
										t->size = ($2.type)->width;	
										curr_symtab->offset += t->size;
										
										if($2.array){
											// emit quads of indexed copy and arithmetic instructions
											quadArray::emit("rindexed",($2.array)->name,($2.addr)->name,t->name);
											quadArray::emit("-",t->name,"1",t->name);
											quadArray::emit("lindexed",($2.addr)->name,t->name,($2.array)->name);
											quadArray::emit(t->name,($$.addr)->name);
										}
										else{
											// emit quads of arithmetic and copy instructions
											quadArray::emit("-",($2.addr)->name,"1",t->name);
											quadArray::emit(t->name,($2.addr)->name);
											quadArray::emit(t->name,($$.addr)->name);
										}
									}
									
	| unaryop castexp				{	
										switch($1){
											case '+':
												$$.addr = tableSym::gentemp();	// generate temporary 
												($$.addr)->type = $2.type;		// set the address attribute
												($$.addr)->size = ($2.type)->width;		//
												if(($2.addr)->initval!="")($$.addr)->initval = "+" + ($2.addr)->initval;
												curr_symtab->offset += ($$.addr)->size;		// update total offset of symbol table
												
												$$.array = NULL;		//
												$$.type = $2.type;      // set other attributes
												$$.point = $2.point;	//
												$$.matop = $2.matop;	//
												
												quadArray::emit("+",($2.addr)->name,($$.addr)->name);	// emit quad for unary plus
												break;
											
											case '&': 
												$$.addr = tableSym::gentemp();					// generate temporary
												($$.addr)->type = new mtype();					//
												($$.addr)->type->ntype = "pointer";				//
												($$.addr)->type->next = $2.type;				// construct the type of unary expression
												($$.addr)->type->width = int_width;				//
												($$.addr)->type->isarr = 0;						//
												($$.addr)->size = int_width;					// set size of temporary in symbol table
												curr_symtab->offset += ($$.addr)->size;			// update total offset of symbol table
												
												$$.array = NULL;				//
												$$.type = ($$.addr)->type;		// set attributes of $$
												$$.point = $2.point;			//
												$$.matop = $2.matop;			//
												
												quadArray::emit("ref",($2.addr)->name,($$.addr)->name);
												break;
																								
											case '-':
												$$.addr = tableSym::gentemp();	// generate temporary 
												($$.addr)->type = $2.type;		// set the address attribute
												($$.addr)->size = ($2.type)->width;	//
												if(($2.addr)->initval!="")($$.addr)->initval = "-" + ($2.addr)->initval;
												curr_symtab->offset += ($$.addr)->size;	// update total offset of symbol table
												
												$$.array = NULL;		//
												$$.type = $2.type;		// set other attributes
												$$.point = $2.point;	//
												$$.matop = $2.matop;	//
												
												quadArray::emit("-",($2.addr)->name,($$.addr)->name);	// emit quad for unary minus
												break;
												
											case '*':
												$$.addr = $2.addr;				// set addr attribute						
												$$.array = NULL;				// set array attribute	
												$$.type = ($2.type)->next;		// set type attribute
												$$.point = 1;					// set point attribute
												$$.matop = $2.matop;			//
												break;
										}
									}
	;

unaryop:    '&'						{$$ = '&';}
	| '*'							{$$ = '*';}
	| '+'							{$$ = '+';}
	| '-'							{$$ = '-';}
	;

castexp:    unaryexp				{	
										if($1.array){
											if($1.matop==0){
												// if unaryexp is an array
												$$.addr = tableSym::gentemp();
												($$.addr)->type = $1.type;
												($$.addr)->size = ($1.type)->width;
												curr_symtab->offset += ($$.addr)->size;
											
												$$.array = NULL;
												$$.type = $1.type;
												$$.point = $1.point;
												$$.matop = $1.matop;
											
												// emit quad of right indexed copy instruction
												quadArray::emit("rindexed",($1.array)->name,($1.addr)->name,($$.addr)->name);
											}
											else $$ = $1;
										}
										else{
											if($1.point==1){
												// if unaryexp is a pointer
												$$.addr = tableSym::gentemp();
												($$.addr)->type = $1.type;
												($$.addr)->size = ($1.type)->width;
												$$.type = $1.type;
												$$.array = $1.array;
												$$.point = 0;
												$$.matop = $1.matop;
												
												// emit quad of right dereference
												quadArray::emit("rdref",($1.addr)->name,($$.addr)->name);
											}
											else{
												// if unaryexp is a simple expression
												$$ = $1;
											}
										}									
									}
	;

multexp:    castexp					{$$ = $1;}

	| multexp '*' castexp			{
										typecheck(&($1),&($3));					// check the types
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;			//
										$$.array = $1.array;		// set other attributes
										$$.point = $1.point;		//
										$$.matop = $1.matop;		//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr;
											$$.type = new mtype();			// set type of transpose of postexp i.e. $1
											($$.type)->ntype = "array";
											($$.type)->width = ($1.type)->numelem * ($3.type)->next->numelem * ($3.type)->next->next->width;
											($$.type)->isarr = 1;
											($$.type)->numelem = ($1.type)->numelem; 		// row number of $$ matrix is same as $1 matrix
											($$.type)->next = new mtype();						
											($$.type)->next->ntype = "array"; 
											($$.type)->next->numelem = ($3.type)->next->numelem;		// col number of $$ matrix is same as $3 matrix
											($$.type)->next->width = ($$.type)->next->numelem * ($1.type)->next->next->width;
											($$.type)->next->isarr = 1;
											($$.type)->next->next = new mtype();
											($$.type)->next->next->ntype = ($1.type)->next->next->ntype;
											($$.type)->next->next->width = ($1.type)->next->next->width;
											($$.type)->next->next->next = NULL;
											($$.type)->next->next->isarr = 0;
										
											($$.array)->type = $$.type;						//
											curr_symtab->offset -= ($$.array)->size;		// subtract old size
											($$.array)->size = ($$.type)->width;			// set new size
											curr_symtab->offset += ($$.array)->size;		// add new size
											
											// emit quad of matrix multiplication instruction 
											quadArray::emit("*",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of multiplication instruction 
											quadArray::emit("*",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}
									}
									
	| multexp '/' castexp			{
										typecheck(&$1,&$3);					// check the types
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;			//
										$$.array = $1.array;		// set other attributes
										$$.point = $1.point;		//
										$$.matop = $1.matop;		//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr; 
											quadArray::emit("/",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of division instruction
											quadArray::emit("/",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}	
									}
									
	| multexp '%' castexp			{
										typecheck(&$1,&$3);					// check the types
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;			//
										$$.array = $1.array;		// set other attributes
										$$.point = $1.point;		//
										$$.matop = $1.matop;		//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr;
											quadArray::emit("%",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of modulo instruction
											quadArray::emit("%",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}	
									}
	;

addexp:    multexp					{$$ = $1;}

	| addexp '+' multexp			{
										typecheck(&$1,&$3);					// check the types
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;			//
										$$.array = $1.array;		// set other attributes
										$$.point = $1.point;		//
										$$.matop = $1.matop;		//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr;
											
											// emit quad of matrix addition instruction
											quadArray::emit("+",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of addition instruction
											quadArray::emit("+",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}	
									}
									
	| addexp '-' multexp			{
										typecheck(&$1,&$3);					// check the types
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;			//
										$$.array = $1.array;		// set other attributes
										$$.point = $1.point;		//
										$$.matop = $1.matop;		//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr;
											
											// emit quad of matrix subtraction instruction
											quadArray::emit("-",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of subtraction instruction
											quadArray::emit("-",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}
									}
	;

shiftexp:    addexp					{$$ = $1;}

	| shiftexp SHL addexp			{
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;			//
										$$.array = $1.array;		//
										$$.point = $1.point;		// set other attributes
										$$.matop = $1.matop;		//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr;
											
											// emit quad of matrix left shift instruction
											quadArray::emit("<<",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of left shift instruction
											quadArray::emit("<<",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}
									}
									
	| shiftexp SHR addexp			{
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;			//
										$$.array = $1.array;		// set other attributes
										$$.point = $1.point;		//
										$$.matop = $1.matop;		//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr;
											
											// emit quad of matrix right shift instruction
											quadArray::emit(">>",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of right shift instruction
											quadArray::emit(">>",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}
									}
	;

relexp:    shiftexp					{$$ = $1;}

	| relexp '<' shiftexp			{
										typecheck(&$1,&$3);					// check the types
										$$.type = new mtype();
										($$.type)->ntype = "bool";
										($$.type)->width = bool_width;
										($$.type)->next = NULL;
										($$.type)->isarr = 0;
										
										$$.truelist = makelist(nextinstr);			// generate truelist of $$
										
										// emit quad of if x<y goto _ instruction
										quadArray::emit("iflt",($1.addr)->name,($3.addr)->name,"_");
										
										$$.falselist = makelist(nextinstr);		// generate falselist of $$
										quadArray::emit("_");	
									}
									
	| relexp '>' shiftexp			{
										typecheck(&$1,&$3);					// check the types
										$$.type = new mtype();
										($$.type)->ntype = "bool";
										($$.type)->width = bool_width;
										($$.type)->next = NULL;
										($$.type)->isarr = 0;
										
										$$.truelist = makelist(nextinstr);			// generate truelist of $$
										
										// emit quad of if x>y goto _ instruction
										quadArray::emit("ifgt",($1.addr)->name,($3.addr)->name,"_");
										
										$$.falselist = makelist(nextinstr);		// generate falselist of $$
										quadArray::emit("_");	
									}
									
	| relexp LTE shiftexp			{
										typecheck(&$1,&$3);					// check the types
										$$.type = new mtype();
										($$.type)->ntype = "bool";
										($$.type)->width = bool_width;
										($$.type)->next = NULL;
										($$.type)->isarr = 0;
										
										$$.truelist = makelist(nextinstr);			// generate truelist of $$
										
										// emit quad of if x<=y goto _ instruction
										quadArray::emit("iflte",($1.addr)->name,($3.addr)->name,"_");
										
										$$.falselist = makelist(nextinstr);		// generate falselist of $$
										quadArray::emit("_");	
									}
									
	| relexp GTE shiftexp			{
										typecheck(&$1,&$3);						// check the types
										$$.type = new mtype();
										($$.type)->ntype = "bool";
										($$.type)->width = bool_width;
										($$.type)->next = NULL;
										($$.type)->isarr = 0;
										
										$$.truelist = makelist(nextinstr);			// generate truelist of $$
										
										// emit quad of if x>=y goto _ instruction
										quadArray::emit("ifgte",($1.addr)->name,($3.addr)->name,"_");
										
										$$.falselist = makelist(nextinstr);		// generate falselist of $$
										quadArray::emit("_");	
									}
	;

eqexp:    relexp					{$$ = $1;}

	| eqexp EQ relexp				{
										typecheck(&$1,&$3);						// check the types
										$$.type = new mtype();
										($$.type)->ntype = "bool";
										($$.type)->width = bool_width;
										($$.type)->next = NULL;
										($$.type)->isarr = 0;
										
										$$.truelist = makelist(nextinstr);			// generate truelist of $$
										
										// emit quad of if x==y goto _ instruction
										quadArray::emit("ifeq",($1.addr)->name,($3.addr)->name,"_");
										
										$$.falselist = makelist(nextinstr);		// generate falselist of $$
										quadArray::emit("_");	
									}
									
	| eqexp NEQ relexp				{
										typecheck(&$1,&$3);						// check the types
										$$.type = new mtype();
										($$.type)->ntype = "bool";
										($$.type)->width = bool_width;
										($$.type)->next = NULL;
										($$.type)->isarr = 0;
										
										$$.truelist = makelist(nextinstr);			// generate truelist of $$
										
										// emit quad of if x!=y goto _ instruction
										quadArray::emit("ifneq",($1.addr)->name,($3.addr)->name,"_");
										
										$$.falselist = makelist(nextinstr);		// generate falselist of $$
										quadArray::emit("_");	
									}
	;

andexp:    eqexp					{$$ = $1;}

	| andexp '&' eqexp				{
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;				//
										$$.array = $1.array;			// set other attributes
										$$.point = $1.point;			//
										$$.matop = $1.matop;			//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr;
											
											// emit quad of matrix & operation
											quadArray::emit("&",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of & operation
											quadArray::emit("&",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}
									}
	;

xorexp:    andexp					{$$ = $1;}

	| xorexp '^' andexp				{
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;			//
										$$.array = $1.array;		//
										$$.point = $1.point;		// set other attributes
										$$.matop = $1.matop;		//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr;
											
											// emit quad of matrix ^ operation
											quadArray::emit("^",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of ^ operation
											quadArray::emit("^",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}	
									}
	;

orexp:    xorexp					{$$ = $1;}

	| orexp '|' xorexp				{
										$$.addr = tableSym::gentemp();
										($$.addr)->type = $1.type;
										($$.addr)->size = ($1.type)->width;
										curr_symtab->offset += ($$.addr)->size;
										
										$$.type = $1.type;			//
										$$.array = $1.array;		// set other attributes
										$$.point = $1.point;		//
										$$.matop = $1.matop;		//
										
										if($1.matop==1 && $3.matop==1){
											$$.array = $$.addr;
											
											// emit quad of matrix & operation
											quadArray::emit("|",($1.array)->name,($3.array)->name,($$.array)->name);
										}
										else{
											// emit quad of & operation
											quadArray::emit("|",($1.addr)->name,($3.addr)->name,($$.addr)->name);
										}
									}
	;
	
M:									{
										$$ = nextinstr; // store the index of next instruction
									}	
;

logandexp:    orexp					{ $$ = $1;}

	| logandexp AND M orexp			/* Non-Terminal M is used to jump to 2nd Boolean Expression if 1st Boolean Expression is true */
									{
										convtobool(&$4);			// convert expression to boolean
										convtobool(&$1);			// convert expression to boolean
										
										$$.type = new mtype();
										($$.type)->ntype = "bool";
										($$.type)->width = bool_width;
										($$.type)->next = NULL;
										($$.type)->isarr = 0;	
										
										backpatch($1.truelist,$3);	
										$$.truelist = $4.truelist;							// generate truelist of $$
										$$.falselist = merge($1.falselist,$4.falselist);	// generate falselist of $$
									}
	;

logorexp:    logandexp 				{$$ = $1;}

	| logorexp OR M logandexp		/* Non-Terminal M is used to jump to 2nd Boolean Expression if 1st Boolean Expression is false */
									{
										convtobool(&$4);				// convert expression to boolean
										convtobool(&$1);				// convert expression to boolean
										
										$$.type = new mtype();
										($$.type)->ntype = "bool";
										($$.type)->width = bool_width;
										($$.type)->next = NULL;
										($$.type)->isarr = 0;	
										
										backpatch($1.falselist,$3);
										$$.falselist = $4.falselist;						// generate falselist of $$
										$$.truelist = merge($1.truelist,$4.truelist);		// generate truelist of $$
									}
	;
	
N:									{
										// generates goto instruction to jump to some specific quad. Mostly used in if-else statements
										$$ = makelist(nextinstr); 
										quadArray::emit("_");
									}
;

condexp:    logorexp				{$$ = $1;}

	| logorexp '?' exp ':' condexp	/* if logorexp (i.e. $1) is true jump to the code [t = exp (i.e. $3)] and go to the end of condexp(i.e. $$) else jump to the code [t = condexp (i.e. $5)].*/
										{
											convtobool(&$1);				// convert expression to boolean
											$$.addr = tableSym::gentemp();
											($$.addr)->type = $3.type;
											($$.addr)->size = ($3.type)->width;
											curr_symtab->offset += ($$.addr)->size;
											
											$$.type = $3.type;			//
											$$.array = $3.array;		// set other attributes
											$$.point = $3.point;		//
											$$.matop = $3.matop;		//
											
											backpatch($1.truelist,nextinstr);		// if $1 is true jump to [t = exp (i.e. $4)]
											backpatch($1.falselist,nextinstr+2);	// if $1 is false jump to [t = condexp (i.e. $7)]		
											
											if($3.matop==1 && $5.matop==1){
												$$.array = $$.addr;
											
												// emit quad of matrix copy instruction
												quadArray::emit(($3.array)->name,($$.array)->name);
											}
											else{
												// emit quad of copy instruction
												quadArray::emit(($3.addr)->name,($$.addr)->name);
											}
											
											char str[10];
											sprintf(str,"%d",nextinstr+2);
											string str1(str);
											quadArray::emit(str);		// jump to instruction at nextinstr+1 if $$ is assigned to $3
											
											if($3.matop==1 && $5.matop==1){
												$$.array = $$.addr;
											
												// emit quad of matrix copy instruction
												quadArray::emit(($5.array)->name,($$.array)->name);
											}
											else{
												// emit quad of copy instruction
												quadArray::emit(($5.addr)->name,($$.addr)->name);
											}
										}
	;

assignexp:    condexp				{$$ = $1;}

	| unaryexp assignop assignexp	{
										if($1.point==0){
											
											if($1.array){
												typecheck(&$1,&$3);
												// emit quad of matrix copy instruction
												if($1.matop==1 && $3.matop==1)quadArray::emit(($3.array)->name,($1.array)->name);
												// emit quad of left indexed copy instruction
												else quadArray::emit("lindexed",($1.addr)->name,($3.addr)->name,($1.array)->name);
											}
											else{
												typecheck(&$1,&$3);
												// emit quad of copy instruction
												quadArray::emit(($3.addr)->name,($1.addr)->name);
											}
										}
										else{
											// emit quad of left dereference instruction
											quadArray::emit("ldref",($3.addr)->name,($1.addr)->name);
										}
										
										$$ = $1;
									}
	;

assignop:    '='	{}
	| MULC			{}
	| DIVC			{}
	| MODC			{}
	| ADDC			{}
	| SUBC			{}
	| SHLC			{}
	| SHRC			{}
	| ANDC			{}
	| XORC			{}
	| ORC			{}
	;

exp:    assignexp			{$$ = $1;}
	| exp ',' assignexp		{}
	;

constexp:    condexp		{$$ = $1;}
	;



/*DECLARATIONS*/
dection:    dectionspec initdectorlist ';'	{}
	| dectionspec ';'						{}
	;

dectionspec:    typespec dectionspec		{}
	| typespec								{
												$$ = $1;
												T = $1.type;	// store the type
												W = $1.width;	// store width of the type
											}
	;

initdectorlist:    initdector				{$$ = $1;}
	| initdectorlist ',' initdector			{$$ = $1;}
	;

initdector:    dector						{
												$$ = $1;	
												if(($1.type)->ntype=="function"){
													curr_symtab = global_symtab;		// restore global symbol table
												}
											}
											
	| dector '=' initializer				// Non-terminal Q sets a flag that prevents generation of quads for initialization of a variable
											{
												if(($1.type)->isarr==0){
													if((*($3.val))!="")($1.addr)->initval = *($3.val);		// set initval in symbol table
													quadArray::emit(($3.express.addr)->name,($1.addr)->name);
												}
												else{
													($1.array)->initval = *($3.val);		// set initval in symbol table
													if($3.express.matop==1)quadArray::emit(($3.express.array)->name,($1.array)->name);
												}
																					
												$$ = $1;
												if(($1.type)->ntype=="function"){
													curr_symtab = global_symtab;	// restore global symbol table if $1 is of type function
												}	
											}
	;
	
typespec:    VOID		{
							$$.type = new mtype();			
							($$.type)->ntype = "void";		
							($$.type)->isarr = 0;			// set type attribute
							($$.type)->width = void_width;			
							($$.type)->next = NULL;			
							
							$$.width = void_width;		
						}
						
	| CHAR				{
							$$.type = new mtype();
							($$.type)->ntype = "char";
							($$.type)->isarr = 0;			// set type attribute
							($$.type)->width = char_width;
							($$.type)->next = NULL;	
							
							$$.width = char_width;
						}
						
	| SHORT				{}
	
	| INT				{
							$$.type = new mtype();
							($$.type)->ntype = "int";			// set type attribute
							($$.type)->isarr = 0;
							($$.type)->width = int_width;
							($$.type)->next = NULL;	
							
							$$.width = int_width;
						}
						
	| LONG				{}
	| FLOAT				{}
	
	| DOUBLE			{
							$$.type = new mtype();
							($$.type)->ntype = "double";
							($$.type)->isarr = 0;					// set type attribute
							($$.type)->width = double_width;
							($$.type)->next = NULL;	
							
							$$.width = double_width;
						}
	
	| MATRIX			{
							$$.type = new mtype();
							($$.type)->ntype = "double";
							($$.type)->isarr = 0;
							($$.type)->width = double_width;				// set type attribute
							($$.type)->next = NULL;	
							
							$$.width = double_width;
						}
	
	| SIGNED			{}
	| UNSIGNED			{}
	| BOOL				{}
	;

dector:    directdector			{
									$$ = $1;
									
									if(($$.type)->ntype!="function"){
										if(($$.type)->isarr==0){
											// dector i.e. $1 is a simple declarator
											($$.addr)->type = $$.type;
											($$.addr)->initval = "---"; 					// update symbol table entry
											($$.addr)->size = ($$.type)->width;
											curr_symtab->offset += ($$.addr)->size;
										}
										else{
											// dector i.e. $1 is of type matrix
											($$.array)->type = $$.type;
											($$.array)->initval = "---";					// update symbol table entry
											($$.array)->size = ($$.type)->width;
											curr_symtab->offset += ($$.array)->size + (2*int_width);
										}
									}
									else{
										// dector i.e. $1 is of type function
										if(($$.addr)->nestTab==NULL){
											// function is declared for the first time
											($$.addr)->type = $$.type;
											($$.addr)->initval = "---";						// update symbol table entry
											($$.addr)->size = 0;	
											($$.addr)->nestTab = curr_symtab;
											global_symtab->offset += ($$.addr)->size;
										}
									}
								}

	| pointer directdector		{
									$$ = $2;
									
									
									if(($$.type)->ntype!="function"){
										$$.type = $1.type;			//
										mtype *p;					//
										p = $$.type;				//
										while(p->next){				// set type of declarator i.e. $$
											p = p->next;			//
										}							//
										p->next = $2.type;			//
									
										if(($$.type)->isarr==0){
											// dector i.e. $1 is a simple declarator
											($$.addr)->type = $$.type;
											($$.addr)->initval = "---";
											($$.addr)->size = ($$.type)->width;			// update symbol table entry
											curr_symtab->offset += ($$.addr)->size;
										}
										else{
											// dector i.e. $1 is of type matrix
											($$.array)->type = $$.type;
											($$.array)->initval = "---";				// update symbol table entry
											($$.array)->size = ($$.type)->width;
											curr_symtab->offset += ($$.array)->size + (2*int_width);
										}
									}
									else{
										// dector i.e. $1 is of type function
										if(($$.addr)->nestTab==NULL){
											// function is declared for the first time
											mtype *p;					//
											p = $1.type;				//
											while(p->next){				// set type of declarator i.e. $$
												p = p->next;			//
											}							//
											p->next = ($2.type)->rtype;	//
											($2.type)->rtype = $1.type;
										
											($$.addr)->type = $$.type;
											($$.addr)->initval = "---";
											($$.addr)->size = 0;						// update symbol table entry
											($$.addr)->nestTab = curr_symtab;
											global_symtab->offset += ($$.addr)->size;
										}
									}
								}
	;	
	
P:			// creates new symbol table							
			{
				curr_symtab = new tableSym();	/*create new symbol table*/	
			}
	;

directdector:	ID							{
												$1.addr = curr_symtab->lookup(*($1.lexeme)); 	// create symbol table entry for identifier
												$$.addr = $1.addr;		//
												$$.type = T;			//	
												$$.array = NULL;		//	set the attributes
												$$.point = 0; 			//
											}
											
	| '(' dector ')'						{
												$$ = $2;	
											}
	| directdector '[' assignexp ']'		{
												if(($1.type)->isarr==1){
													// if $1 is already an array
													mtype *p = $1.type->next;
													mtype *q = $1.type;
													while(p->next){
														q->width *= atoi(($3.addr)->initval.c_str());
														q = p;
														p = p->next;
													}
													q->width *= atoi(($3.addr)->initval.c_str());		// update the type of array variable
													
													mtype *r = new mtype();
													r->ntype = "array";
													r->numelem = atoi(($3.addr)->initval.c_str());
													
													r->width = r->numelem * p->width;
													r->isarr = 1;
													r->next = p;
													q->next = r;
													$$ = $1;	
												}
												else{
													// if $1 is not an array
													$$.type = new mtype();
													($$.type)->ntype = "array";
													($$.type)->numelem = atoi(($3.addr)->initval.c_str());
													
													($$.type)->width = ($$.type)->numelem * ($1.type)->width;
													($$.type)->isarr = 1;
													($$.type)->next = $1.type;			// make the variable of type array
													$$.array = $1.addr;
													$1.array = $1.addr;	
													$1.type = $$.type;
												}
												
												curr_symtab->deltemp(($3.addr)->name);	
											}
											
	| directdector '[' ']'					{}
											
	| directdector '(' P paramtypelist ')'	/* Non-Terminal P is used to create a new symbol table for this function and to store the parameter list in this new symbol table */
											{
												if(($1.type)->ntype!="function"){
													curr_symtab->name = ($1.addr)->name;	// name of symbol table of this function
													($1.type)->rtype = new mtype();
													($1.type)->rtype->ntype = ($1.type)->ntype;
													($1.type)->rtype->width = ($1.type)->width;
													($1.type)->rtype->isarr = ($1.type)->isarr;
													($1.type)->rtype->next = ($1.type)->next;
													
													($1.type)->ntype = "function";			//
													($1.type)->width = curr_symtab->offset;	//
													($1.type)->isarr = 0;					// set type of this function in global symbol table
													($1.type)->next = NULL;					//
													
													$$ = $1;
												}
												else{
													curr_symtab = ($1.addr)->nestTab;
													$$ = $1;	
												}
											}
											
	| directdector '(' idlist ')'			{}
	
	| directdector '(' P ')'				/* Non-Terminal P is used to create a new symbol table for this function */
											{
												if(($1.type)->ntype!="function"){
													curr_symtab->name = ($1.addr)->name;	// name of symbol table of this function
													($1.type)->rtype = new mtype();
													($1.type)->rtype->ntype = ($1.type)->ntype;
													($1.type)->rtype->width = ($1.type)->width;
													($1.type)->rtype->isarr = ($1.type)->isarr;
													($1.type)->rtype->next = ($1.type)->next;
													($1.type)->ntype = "function";			//
													($1.type)->width = curr_symtab->offset;	//
													($1.type)->isarr = 0;					// set type of function in global symbol table
													($1.type)->next = NULL;					//
												
													$$ = $1;
												}
												else{
													curr_symtab = ($1.addr)->nestTab;
													$$ = $1;	
												}
												
											}
	;

pointer:	'*'			{
							mtype *ty = new mtype();		//
							ty->ntype = "pointer";			//
							ty->next = NULL;				//	set type of $$
							ty->isarr = 0;					//
							ty->width = int_width;					//
							$$.type = ty;
							$$.width = int_width;	
						}
						
	| '*' pointer		{
							mtype *ty = new mtype();		//
							ty->ntype = "pointer";			//
							ty->isarr = 0;					//	set type of $$
							ty->width = int_width;			//
							ty->next = $2.type;				//
							$$.type = ty;
							$$.width = int_width;	
						}
	;

paramtypelist:	paramlist	{}
	;

paramlist:	paramdection			{}
	| paramlist ',' paramdection		{}
	;

paramdection:	dectionspec dector	{}
									
	| dectionspec					{}
	;

idlist:	ID				{}
	| idlist ',' ID		{}
	;

initializer:	assignexp			{
										if(($1.type)->isarr==0)$$.val = new string(($1.addr)->initval);		// saving the initial value
										else $$.val = new string(($1.array)->initval);
										$$.express = $1;
									}

	| '{' initializerrowlist '}'	{
										$$.express = $2.express;
										$$.val = new string("{"+(*($2.val))+"}");		// saving the initial value
									}
	;

initializerrowlist:	initializerrow				{
													$$.val = new string(*($1.val));		// saving the initial value
													$$.express = $1.express;
												}
	| initializerrowlist ';' initializerrow		{
													$$.val = new string((*($1.val)) + ";" + (*($3.val)));	// saving the initial value
													$$.express = $1.express;	
												}
	;

initializerrow:	initializer						{
													$$.val = new string(*($1.val));			// saving the initial value
													$$.express = $1.express;
												}
	| denation initializer						{}
	| initializerrow ',' denation initializer	{}
	| initializerrow ',' initializer			{
													$$.val = new string((*($1.val)) + "," + (*($3.val)));		// saving the initial value
													$$.express = $1.express;
												}
	;

denation:	denatorlist '='			{}
	;

denatorlist:	denator				{}
	| denatorlist denator			{}
	;

denator:	'[' constexp ']'		{}
	| '.' ID						{}
	;


/*STATEMENTS*/
stmt:	lblstmt			{}
	| compstmt			{$$ = $1;}
	| expstmt			{$$ = NULL;}
	| selstmt			{$$ = $1;}
	| itrstmt			{$$ = $1;}
	| jumpstmt			{$$ = $1;} 
	;

lblstmt:	ID ':' stmt			{}
	| CASE constexp ':' stmt	{}
	| DEFAULT ':' stmt			{}
	;

compstmt:	'{' '}'				{$$ = NULL;}
	| '{' blockitemlist '}'		{$$ = $2;}
	;

blockitemlist:	blockitem			{$$ = $1;}

	| blockitemlist M blockitem		/* Non-Terminal M is used to jump to 2nd statement block i.e. $3 from 1st statement block i.e. $1 */
									{
										backpatch($1,$2);
										$$ = $3;
									}
	;

blockitem:	dection			{$$ = NULL;}
	| stmt					{$$ = $1;}
	;

expstmt:	exp ';'			{$$ = $1;}
	| ';'					{$$.addr = NULL;}
	;

selstmt:	IF '(' exp ')' M stmt			/* Non-Terminal M is used to jump to stmt i.e. $6 if the Boolean Expression is true */
											{
												backpatch($3.truelist,$5);
												$$ = merge($3.falselist,$6);	
											}
											
	| IF '(' exp ')' M stmt N ELSE M stmt	/* 1st M is used to jump to 1st stmt i.e. $6 if the Boolean Expression is true and 2nd M is used to jump to 2nd stmt i.e. $10 if the Boolean Expression is false. Non-Terminal N is used to jump from 1st stmt i.e. $6 to end of selstmt i.e. $$ */
											{
												backpatch($3.truelist,$5);	
												backpatch($3.falselist,$9);
												list *tep = merge($6,$7); 	
												$$ = merge(tep,$10);
											}
											
	| SWITCH '(' exp ')' stmt				{}
	;

itrstmt:	WHILE M '(' exp ')' M stmt		/* 1st M is used to jump to beginning of while loop after dealing with stmt i.e. $7. 2nd M is used to jump to stmt i.e. $7 if the boolean expression exp i.e. $4 is true */		
										{
											backpatch($4.truelist,$6);
											backpatch($7,$2);
											$$ = $4.falselist;
											char str[10];
											sprintf(str,"%d",$2);
											quadArray::emit(str);	// emit quad of goto instruction
										}
											
	| DO M stmt WHILE '(' exp ')' ';'	/* Non-Terminal M is used to jump to stmt i.e. $3 if the boolean expression exp i.e. $6 is true */
										{
											backpatch($6.truelist,$2);
											$$ = $6.falselist;	
										}
											
	| FOR '(' expopt ';' M expopt ';' M expopt N ')' M stmt		/*3rd M is used to jump to stmt i.e. $13 if the Bool Expression i.e. $6 is true. 2nd M is used to jump to expression represented by $9 after dealing with stmt. Non-Terminal N is used to generate a goto quad after executing the expression represented by $9. 1st M is used to jump to the Bool Expression from the goto quad generated by N. */
															{
																backpatch($13,$8);
																backpatch($10,$5);
																backpatch($6.truelist,$12);
																$$ = $6.falselist;
																char str[10];
																sprintf(str,"%d",$8);
																quadArray::emit(str);	// emit quad of goto instruction
															}
															
	| FOR '(' dection expopt ';' expopt ')' stmt		{}
	;

expopt: 				{$$.addr = NULL;}
	| exp				{$$ = $1;}
	;

jumpstmt:	GOTO ID ';'		{}
	| CONTINUE ';'			{}
	| BREAK ';'				{}
	| RETURN expopt ';'		{
								if($2.addr){
									quadArray::emit("return","",($2.addr)->name);	// emit quad of return statement instruction
								}	
								$$ = NULL;
							}
	;


/*EXTERNAL DECLARATIONS*/
transunit:	extdection			{}
	| transunit extdection		{}
	;

extdection:	funcdef			{}
	| dection				{}
	;

funcdef:	dectionspec dector dectionlist compstmt		{
															($2.type)->width = curr_symtab->offset;
															($2.addr)->type = $2.type;
															($2.addr)->initval = "---";
															diff = ($2.type)->width - ($2.addr)->size;
															//($2.addr)->size = ($2.type)->width;
															($2.addr)->nestTab = curr_symtab;		// save symbol table of this function
															curr_symtab = global_symtab;			// restore old symbol table
															
															if($4)backpatch($4,nextinstr);
														}
														
	| dectionspec dector compstmt						{
															($2.type)->width = curr_symtab->offset;
															($2.addr)->type = $2.type;
															($2.addr)->initval = "---";
															diff = ($2.type)->width - ($2.addr)->size;
															//($2.addr)->size = ($2.type)->width;
															($2.addr)->nestTab = curr_symtab;		// save symbol table of this function
															curr_symtab = global_symtab;			// restore old symbol table
															
															if($3)backpatch($3,nextinstr);		// backpatch last statement of compound statement to next instruction
														}
	;

dectionlist:	dection			{}
	| dectionlist dection		{}
	;

%%

void yyerror(const char *s){
	printf("%s at %s \n",s,yytext);
}
