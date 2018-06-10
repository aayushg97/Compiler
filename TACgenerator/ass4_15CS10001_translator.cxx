#include "ass4_15CS10001_translator.h"
#include "ass4_15CS10001.tab.h"
extern int yylineno;
using namespace std;

int numtemp = 0;
int nextinstr;
int int_width = 4;
int double_width = 8;
int char_width = 1;
int bool_width = 1;
int void_width = -1;
tableSym *global_symtab;
tableSym *curr_symtab;
quadArray global_quad;

tableSym::tableSym(){				// constructor
	this->offset = 0;
	this->name = "";
	this->head = NULL;
}

entrySym* tableSym::lookup(string lexeme){
	entrySym *p = this->head;	
	entrySym *q = p;	
	while(p){
		if(p->name==lexeme)return p;
		q = p;
		p = p->next;
	}
	
	if(q){
		p = new entrySym();					//
		p->name = lexeme;					//
		p->type = NULL;						//
		p->initval = "---";					//
		p->size = 0;						// creating a new symbol table entry
		p->offset = curr_symtab->offset;	//
		p->nestTab = NULL;					//
		p->next = NULL;						//
		q->next = p;						//
		return p;							//
	}
	else{
		p = new entrySym();					//
		p->name = lexeme;					//
		p->type = NULL;						//
		p->initval = "---";					//
		p->size = 0;						// 
		p->offset = curr_symtab->offset;	// creating a new symbol table entry
		p->nestTab = NULL;					//
		p->next = NULL;						//
		this->head = p;						//
		return p;							//
	}
}

entrySym* tableSym::find(string lexeme){
	entrySym *p = this->head;
	while(p){
		if(p->name==lexeme)return p;			// searching for an entry in the symbol table
		p = p->next;
	}
	
	return NULL;
}

entrySym* tableSym::gentemp(){
	char lexeme[10];
	sprintf(lexeme,"t%d",numtemp);				// generating a temporary
	numtemp++;
	return curr_symtab->lookup(lexeme);
}

entrySym* tableSym::deltemp(string tname){
	entrySym *p = this->head;
	entrySym *q = p;
	int diff;
	
	if(q->name==tname){
		diff = q->size;
		this->head = q->next;
		p = q->next;
		while(p){
			p->offset -= diff;
			p = p->next;
		}	
		this->offset -= diff;
	}												
	else{
		while(p){
			if(p->name==tname){
				diff = p->size;
				q->next = p->next;
				p = q->next;
				while(p){
					p->offset -= diff;
					p = p->next;
				}
				this->offset -= diff;
				break;
			}
			q = p;
			p = p->next;
		}
	}
}

void tableSym::update(entrySym *entry,string name,mtype *type,string initval,int size,int offset,tableSym *nestTab){
	entry->name = name;
	entry->type = type;
	entry->initval = initval;				// function to update a symbol table entry
	entry->size = size;
	entry->offset = offset;
	entry->nestTab = nestTab;
}

//function to print the symbol table in a particular format
void tableSym::print(){
	printf("\n-------------------------------\n");
	printf("Symbol Table :- %s \n",(this->name).c_str());
	printf("Name \t Type \t Init_Value \t Size \t Offset \t Nested Table \t \n");
	entrySym *p = this->head;
	mtype *q;
	int brcount = 0;
	
	while(p){
		printf("%s \t",(p->name).c_str());
		
		q = p->type;
		if(q->isarr==1)printf("Matrix(%d, %d, double)\t",q->numelem,q->next->numelem);
		else{
			if(q->ntype=="int")printf("%s     \t",(q->ntype).c_str());
			else if(q->ntype=="double")printf("%s  \t",(q->ntype).c_str());
			else if(q->ntype=="char")printf("%s    \t",(q->ntype).c_str());
			else if(q->ntype=="void")printf("%s    \t",(q->ntype).c_str());
			else if(q->ntype=="pointer")printf("ptr(%s) \t",q->next->ntype.c_str());
			else printf("%s \t",(q->ntype).c_str());
		}
		
		if(q->isarr==1)printf("%s \t %d+%d \t %d \t",(p->initval).c_str(),p->size,8,p->offset);
		else printf("%s \t %d \t %d \t",(p->initval).c_str(),p->size,p->offset);
		
		if(p->nestTab){
			printf("\tptr-to-ST(%s) \n",(p->nestTab->name).c_str());
			
		}
		else printf("\t NULL \n");
		
		p = p->next;
	}
}

quadArray::quadArray(){
	this->head = NULL;
	nextinstr = 0;
}

//function to emit a quad with three addresses
void quadArray::emit(string opcode,string arg1,string arg2,string result){
	quad *p;
	quad *q;
	p = global_quad.head;
	q = p;
	while(p){
		q=p;
		p = p->next;
	}
	
	if(q){
		p = new quad();
		p->opcode = opcode;
		p->arg1 = arg1;
		p->arg2 = arg2;
		p->result = result;
		p->next = NULL;
		q->next = p;
	}
	else{
		p = new quad();
		p->opcode = opcode;
		p->arg1 = arg1;
		p->arg2 = arg2;
		p->result = result;
		p->next = NULL;
		global_quad.head = p;
	}
	
	nextinstr = nextinstr + 1;
}

//function to emit a quad with two addresses and an opcode
void quadArray::emit(string opcode,string arg1,string result){
	quad *p;
	quad *q;
	p = global_quad.head;
	q = p;
	while(p){
		q=p;
		p = p->next;
	}
	
	if(q){
		p = new quad();
		p->opcode = opcode;
		p->arg1 = arg1;
		p->arg2 = "";
		p->result = result;
		p->next = NULL;
		q->next = p;
	}
	else{
		p = new quad();
		p->opcode = opcode;
		p->arg1 = arg1;
		p->arg2 = "";
		p->result = result;
		p->next = NULL;
		global_quad.head = p;
	}
	
	nextinstr = nextinstr + 1;
}

// function to emit quad of copy instruction
void quadArray::emit(string arg1,string result){
	quad *p;
	quad *q;
	p = global_quad.head;
	q = p;
	while(p){
		q=p;
		p = p->next;
	}
	
	if(q){
		p = new quad();
		p->opcode = "";
		p->arg1 = arg1;
		p->arg2 = "";
		p->result = result;
		p->next = NULL;
		q->next = p;
	}
	else{
		p = new quad();
		p->opcode = "";
		p->arg1 = arg1;
		p->arg2 = "";
		p->result = result;
		p->next = NULL;
		global_quad.head = p;
	}
	
	nextinstr = nextinstr + 1;
}

//function to emit a quad with only one address addresses
void quadArray::emit(string result){
	quad *p;
	quad *q;
	p = global_quad.head;
	q = p;
	while(p){
		q=p;
		p = p->next;
	}
	
	if(q){
		p = new quad();
		p->opcode = "";
		p->arg1 = "";
		p->arg2 = "";
		p->result = result;
		p->next = NULL;
		q->next = p;
	}
	else{
		p = new quad();
		p->opcode = "";
		p->arg1 = "";
		p->arg2 = "";
		p->result = result;
		p->next = NULL;
		global_quad.head = p;
	}
	
	nextinstr = nextinstr + 1;
}

// function to print the quads in a quad array
void quadArray::print(){
	quad *p = this->head;
	int instrcount = 0;
	while(p){
		printf("%2d :- ",instrcount);
		// ARITHMETIC OPERATIONS
		if(p->arg1!="" && p->arg2!="" && (p->opcode=="+" || p->opcode=="-"|| p->opcode=="*"|| p->opcode=="/"|| p->opcode=="%"|| p->opcode=="<<"|| p->opcode==">>"|| p->opcode=="^"|| p->opcode=="&"|| p->opcode=="|"|| p->opcode=="&&"|| p->opcode=="||"|| p->opcode=="<"|| p->opcode=="<="|| p->opcode==">"|| p->opcode==">="|| p->opcode=="="|| p->opcode=="!=")){
			printf("%s = %s %s %s \n",(p->result).c_str(),(p->arg1).c_str(),(p->opcode).c_str(),(p->arg2).c_str());
		}
		else{ // TYPE CONVERSION 
			if(p->arg1!="" && p->arg2=="" && (p->opcode=="-" || p->opcode=="+" || p->opcode=="!" || p->opcode==".'" || p->opcode=="(double)" || p->opcode=="(int)")){
				if(p->opcode!=".'")printf("%s = %s %s \n",(p->result).c_str(),(p->opcode).c_str(),(p->arg1).c_str());
				else printf("%s = %s %s \n",(p->result).c_str(),(p->arg1).c_str(),(p->opcode).c_str());
			}
			else{ // COPY INSTRUCTIONS
				if(p->arg1!="" && p->arg2=="" && p->opcode==""){
					printf("%s = %s \n",(p->result).c_str(),(p->arg1).c_str());
				}
				else{ //GOTO INSTRUCTION
					if(p->arg1=="" && p->arg2=="" && p->opcode==""){
						printf("goto %s \n",(p->result).c_str());
					}
					else{ //UNCONDITIONAL AND CONDITIONAL JUMP INSTRUCTIONS
						if(p->opcode=="iftrue"){printf("if %s goto %s \n",(p->arg1).c_str(),(p->result).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="ifFalse"){printf("ifFalse %s goto %s \n",(p->arg1).c_str(),(p->result).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="iflt"){printf("if %s < %s goto %s \n",(p->arg1).c_str(),(p->arg2).c_str(),(p->result).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="iflte"){printf("if %s <= %s goto %s \n",(p->arg1).c_str(),(p->arg2).c_str(),(p->result).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="ifgt"){printf("if %s > %s goto %s \n",(p->arg1).c_str(),(p->arg2).c_str(),(p->result).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="ifgte"){printf("if %s >= %s goto %s \n",(p->arg1).c_str(),(p->arg2).c_str(),(p->result).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="ifeq"){printf("if %s == %s goto %s \n",(p->arg1).c_str(),(p->arg2).c_str(),(p->result).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="ifneq"){printf("if %s != %s goto %s \n",(p->arg1).c_str(),(p->arg2).c_str(),(p->result).c_str()); p = p->next; instrcount++; continue;} 
							
						
						// PARAMETER INSTRUCTIONS
						if(p->opcode=="param"){printf("param %s \n",(p->result).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="call"){printf("%s = call %s,%s \n",(p->result).c_str(),(p->arg1).c_str(),(p->arg2).c_str()); p = p->next; instrcount++; continue;}
							
						
						// RETURN INSTRUCTIONS
						if(p->opcode=="return"){printf("return %s \n",(p->result).c_str()); p = p->next; instrcount++; continue;}
						
						// INDEXED COPY INSTRUCTIONS
						if(p->opcode=="lindexed"){printf("%s[%s] = %s \n",(p->result).c_str(),(p->arg1).c_str(),(p->arg2).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="rindexed"){printf("%s = %s[%s] \n",(p->result).c_str(),(p->arg1).c_str(),(p->arg2).c_str()); p = p->next; instrcount++; continue;}
								
						//ADDRESS AND POINTER ASSIGNMENT INSTRUCTIONS
						if(p->opcode=="ref"){printf("%s = &%s \n",(p->result).c_str(),(p->arg1).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="ldref"){printf("*%s = %s \n",(p->result).c_str(),(p->arg1).c_str()); p = p->next; instrcount++; continue;}
						if(p->opcode=="rdref"){printf("%s = *%s \n",(p->result).c_str(),(p->arg1).c_str()); p = p->next; instrcount++; continue;}
					}
				}
			}
		}
		
		p = p->next;
		instrcount++;
	}
}


// input - index to an instruction in quad array
// output - pointer to a list containing i as the only element
// algo :-
//		1) Declare a variable of type list
//   	2) Allocate space using malloc and store the index i
// 		3) return a pointer to the list
list* makelist(int i){							
	list *head;									
	head = new list();							
	head->index = i;							
	head->next = NULL;							
	return head;								
}												


// input - two lists that are to be merged
// output - pointer to the merged list
// algo :-
//		1) Create a new list
//		2) Copy elements of both lists to the new list
// 		3) return pointer to the new list
list* merge(list *p1,list *p2){					
	if(p1!=NULL && p2!=NULL){					
		list *q,*p;									
		p = p1;										 
		q = p;										
		while(p){									
			q = p;									
			p = p->next;
		}
	
		q->next = p2;	
		return p1;
	}
	else{
		if(p1!=NULL && p2==NULL)return p1;
		else{
			if(p1==NULL && p2!=NULL)return p2;
			else return NULL;
		}
	}		
}


// input - list to be backpatched and index to an instruction in the quad array
// algo :- 	
// 		1) Traverse the list
// 		2) Each list element is an index to an instruction in quad array
// 		3) Traverse the quad array till that instruction is obtained
// 		4) Assign the index i to the result field of that instruction
void backpatch(list *head,int i){				
	quad *q;									
	list *p;									
	p = head;									
	while(p){									
		q = global_quad.head;				
		for(int j=0;j<p->index;j++){		
			q = q->next;					
		}										
		char str[10];						
		sprintf(str,"%d",i);				
		string str1(str);					
		q->result = str1; 					
		p = p->next;						
	}
}

int ttypecheck(mtype *t1,mtype *t2){
	if(t1->isarr==1 && t2->isarr==1)return ttypecheck(t1->next,t2->next);
	else{
		if(t1->isarr==0 && t2->isarr==0){
			if(t1->ntype==t2->ntype)return 1;
			else{
				if((t1->ntype=="int" && t2->ntype=="char") || (t1->ntype=="char" && t2->ntype=="int")){
					if(t1->ntype=="int" && t2->ntype=="char")return 2;
					else return 3;
				}
				else {
					if((t1->ntype=="int" && t2->ntype=="double") || (t1->ntype=="double" && t2->ntype=="int")){
						if(t1->ntype=="int" && t2->ntype=="double")return 4;
						else return 5;
					}
					else return 0;
				}
			}
		}
		else return 0;
	}
}


// input - pointers to expressions E1 and E2
// algo :-
//		1) If types of both expressions are same then they are compatible
//		2) If not then check whether one can be converted to another
//		3) if conversion is possible then do the conversion
// 		4) else report that expressions are not compatible
void typecheck(expattr *E1,expattr *E2){
	switch(ttypecheck(E1->type,E2->type)){			
		case 0:										
			printf("%s %s %s %s types not compatible %d \n",(E1->type->ntype).c_str(),(E1->addr->name).c_str(),(E2->addr->name).c_str(),(E2->type->ntype).c_str(),yylineno);
			exit(-1);							
			break;									
													
		case 2: convchartoint(E2); break;			
													
		case 3: convchartoint(E1); break;			
		
		case 4: convinttodouble(E1); break;
		
		case 5: convinttodouble(E2); break;
	}
}


// input - expression to be converted
// algo :- 
//		1) Generate a temporary and set its type to int type
// 		2) Initialize all field of this temporary
// 		3) Update the attributes of the input expression
void convchartoint(expattr *E){
	entrySym *temp = tableSym::gentemp();				
	temp->type = new mtype();							
	temp->type->ntype = "int";							
	temp->type->isarr = 0;								
	temp->type->width = int_width;								
	temp->type->next = NULL;
	temp->size = int_width;
	curr_symtab->offset += temp->size;
	quadArray::emit("(int)",E->addr->name,temp->name);	// emit quad of type conversion to int
	E->addr = temp;
	E->type = temp->type;
}


// input - expression to be converted
// algo :-
//		1) Generate a temporary and set its type to double type
// 		2) Initialize all field of this temporary
// 		3) Update the attributes of the input expression
void convinttodouble(expattr *E){
	entrySym *temp = tableSym::gentemp();				
	temp->type = new mtype();							 
	temp->type->ntype = "double";						
	temp->type->isarr = 0;								
	temp->type->width = double_width;								
	temp->type->next = NULL;
	temp->size = double_width;
	curr_symtab->offset += temp->size;
	quadArray::emit("(double)",E->addr->name,temp->name);	// emit quad of type conversion to double
	E->addr = temp;
	E->type = temp->type;
}


// input - expression to be converted
// algo :- 
//		1) construct a boolean type.
// 		2) change the type of E to bool type constructed above.
void convtobool(expattr *E){
	if(E->type->ntype!="bool"){							
		mtype *ty;										
		ty = new mtype();								
		ty->ntype = "bool";								
		ty->width = bool_width;
		ty->isarr = 0;
		ty->next = NULL;
		
		E->type = ty;
		E->addr->type = ty;
		
		E->falselist = makelist(nextinstr);
		quadArray::emit("ifeq",(E->addr)->name,"0","_");		// emit quad if x==0 goto _
		E->truelist = makelist(nextinstr);
		quadArray::emit("_");								// emit quad goto _
	}
}

int main(){
	global_symtab = new tableSym();
	curr_symtab = global_symtab;
	
	yyparse();
	global_symtab->name = "GLOBAL";
	
	printf("Quads for this program \n \n");
	global_quad.print();
	
	printf("\n\n\n\n");
	
	printf("Symbol Tables for the program \n");
	global_symtab->print();
	entrySym *p;
	p = global_symtab->head;
	while(p){
		if(p->nestTab)p->nestTab->print();
		p = p->next;
	}
	
	return 0;
}
