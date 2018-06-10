a.out:	lexmain.o liblex.yy.a
	gcc lexmain.o -L. -llex.yy
	
lexmain.o: lexmain.c mylex.h
	gcc -c lexmain.c
	
liblex.yy.a: lex.yy.o
	ar -rcs liblex.yy.a lex.yy.o
	
ass2_15CS10001.o:  lex.yy.c mylex.h
	gcc -c lex.yy.c
	
lex.yy.c:	lexer.l
	flex lexer.l
	
clean:
	rm a.out lexmain.o liblex.yy.a lex.yy.o
