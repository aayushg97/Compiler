#ifndef myl_h
#define myl_h

#include <stdio.h>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <string>
#include <string.h>
using namespace std;

struct regdet;
struct mtype;						// data structure to store type of an expression
struct list;						// data structure to store attributes of a statement

class tableSym;						// data structure to store details of a symbol table
struct arec;
class artable;
struct entrySym;					// data structure to store an entry of a symbol table
class quadArray;					// data structure to store array of quads
struct quad;						// data structure to store a quad

struct idattr;						// data structure to store attributes of an identifier
struct expattr;						// data structure to store attributes of an expression
struct parpmlist;
struct paramlist;					// data structure to store attributes of a parameter list
struct typeattr;					// data structure to store attributes of a type specifier
struct initial;

extern int numtemp;					// count of temporary variable
extern int nextinstr;				// stores the count of next instruction
extern int int_width;
extern int point_width;
extern int double_width;
extern int char_width;
extern int bool_width;
extern int void_width;
extern tableSym *global_symtab;		// pointer to global symbol table
extern tableSym *curr_symtab;		// pointer to current symbol table
extern quadArray global_quad;		// represents quad array
//mtype *T;
//int W;

struct regdet{
	string name[3];
	int free;
	string var;
};

struct mtype{			// data structure to store type of a variable
	string ntype;			// name of the type
	int numelem;			// number of elements in case the type is array
	mtype *next;			// pointer to base type in an array or a pointer variable
	int width;				// width of the type
	int isarr;				// flag to tell whether the type is an array or not
	mtype *rtype;			// return type if ntype=function
};

struct list{			
	int index;
	list *next;
};

struct entrySym{			// data structure to store an entry in a symbol table
	string name;			// name of the symbol
	mtype *type;			// type of the symbol
	string initval;			// initial value of the symbol
	int size;				// size of the symbol
	int offset;				// offset of the symbol
	tableSym *nestTab;		// pointer to a symbol table if the symbol represents a function
	entrySym *next;			// pointer to next entry
};

class tableSym{				// class definition of a symbol table
	public:
		tableSym();							// constructor
		string name;						// name of the symbol table
		int offset;							// total offset  of the symbol table
		int strcount;
		entrySym *head;						// pointer to a list of symbol table entries
		entrySym* lookup(string lexeme);	// function to lookup an entry with name = lexeme
		entrySym* find(string lexeme);		// function to search an entry with name = lexeme
		static entrySym* gentemp();			// function to generate a temporary in the symbol table
		entrySym* deltemp(string tname);	// function to delete a temporary from the symbol table
		void update(entrySym *entry,string name,mtype *type,string initval,int size,int offset,tableSym *nestTab);
											// function to update an entry in the symbol table
		void print();						// function to print the symbol table in a particular format
		void handle_global();		 		// function to handle global variables
		int qbegin,qend,param_no;
		artable *actrectab;
		void assign_reg(string s1,string s2);
		void replace_var(string arg,string res);
		void prologue(int func_no);
		void funchand();
		void epilogue(int func_no);
};

struct arec{
	string name;
	int offset;
	int inMem;
	int hasreg;
	int regind;
	int regbit;
	char regch;
	string regname;
	int lastuse;
	arec *next;
};

class artable{
	public:
		artable(tableSym *en,int pcount);
		tableSym *tab;
		string name;
		arec *head;
		int poffset;
		int getoffset(string s);
		int getinMem(string s);
		int gethasreg(string s);
		int getregind(string s);
		int getregbit(string s);
		char getregch(string s);
		string getregname(string s);
		void setoffset(string s,int off);
		void setinMem(string s,int im);
		void sethasreg(string s,int hr);
		void setreg(string s,int index,string rname);
		void addrecord(string s,int off,int im,int hr,int index,int bit,char ch,string rname,int luse);
		void updaterec(string s,int newoff,int im,int hr,int index,string rname);
		void spillall();
};

struct quad{				// data structure to store a quad
	string opcode;				// operator field 
	string arg1;				// 1st argument
	string arg2;				// 2nd argument
	string result;				// result field
	int islabel;
	string label;
	quad *next;					// pointer to next quad
};

class quadArray{		// class definition of quad array
	public:
		quadArray();		// constructor
		quad *head;			// pointer to a list of quads
		int quadcount;
		static void emit(string opcode,string arg1,string arg2,string result);  // function to emit a quad with given parameters
		static void emit(string opcode,string arg1,string result);		// function to emit a quad with given parameters
		static void emit(string arg1,string result);			// function to emit a quad with given parameters
		static void emit(string result);				// function to emit a quad with given parameters
		void print();							// function to print the quad array
};

list* makelist(int i);						// function to create a list with i (an index to the list of instructions) as the only element and return the pointer to that list
list* merge(list *p1,list *p2);				// function to merge two lists and return a pointer to the resulting list
void backpatch(list *head,int i);			// function to set index i as the target in all instructions whose indices are stored in the list
void typecheck(expattr *E1,expattr *E2);		// function to check the type of E1 and E2 for compatibility
void convchartoint(expattr *E);				// function to change the type of expression E from char to int
void convinttodouble(expattr *E);			// function to change the type of expression E from int to double
void convtobool(expattr *E);				// function to change the type of expression E to boolean
char* cstr(string s);
int getlastuse(int s,int e,string name);
void setregbank();

struct idattr{				// data structure to store attributes of an identifier
	entrySym *addr;				// a pointer to entry of this Identifier in the Symbol Table
	string *lexeme;				// lexeme of this identifier
}; 

struct expattr{				// data structure to store attributes of an expression 
	entrySym *addr;				// address of the temporary representing this expression in the symbol table
	mtype *type;				// type of the expression
	entrySym *array;			// address of base of array if the expression is of type Matrix
	list *truelist;				// list of indices of goto instructions that are generated when this boolean expression is true
	list *falselist;            // list of indices of goto instructions that are generated when this boolean expression is false
	int point;
	int matop;
};

struct parpmlist{
	expattr express;
	parpmlist *next;
};

struct paramlist{			// data structures to store attributes of an argument list
	parpmlist *head;			// pointer to a list of addresses where each address is a pointer to an expression
	int pnum;					// number of arguments
};

struct typeattr{			// data structure to store attributes of a type specifier 
	mtype *type;				// type
	int width;					// width of the type
};

struct initial{				// data structure to store attributes of initializer 
	expattr express;			// an expression
	string *val;				// pointer to a string that holds the initial value of a variable
	vector <vector <entrySym*> *> *iarr1;
	vector <entrySym*> *iarr2; 
};

#endif
