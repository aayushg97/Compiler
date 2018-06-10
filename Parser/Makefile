all: lexer parser

lexer:	ass3_15CS10001_lexer.o liblex.yy.a
	gcc -o lexer ass3_15CS10001_lexer.o -L. -llex.yy

parser:	ass3_15CS10001_parser.o libass3_15CS10001.tab.a liblex.yy.a
	gcc -o parser ass3_15CS10001_parser.o -L. -lass3_15CS10001.tab -llex.yy
	
ass3_15CS10001_lexer.o: ass3_15CS10001_lexer.c
	gcc -c ass3_15CS10001_lexer.c

ass3_15CS10001_parser.o: ass3_15CS10001_parser.c
	gcc -c ass3_15CS10001_parser.c
	
liblex.yy.a:	lex.yy.o
	ar -rcs liblex.yy.a lex.yy.o
	
libass3_15CS10001.tab.a:	ass3_15CS10001.tab.o
	ar -rcs libass3_15CS10001.tab.a ass3_15CS10001.tab.o
	
ass3_15CS10001.tab.o:	ass3_15CS10001.tab.c
	gcc -c ass3_15CS10001.tab.c
	
lex.yy.o:	lex.yy.c
	gcc -c lex.yy.c
	
ass3_15CS10001.tab.c:	ass3_15CS10001.y
	bison ass3_15CS10001.y
	
lex.yy.c:	ass3_15CS10001.l
	flex ass3_15CS10001.l
	
clean:	
	rm lexer parser ass3_15CS10001_lexer.o ass3_15CS10001_parser.o libass3_15CS10001.tab.a liblex.yy.a ass3_15CS10001.tab.o lex.yy.o ass3_15CS10001.tab.c lex.yy.c

