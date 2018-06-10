a.out:	ass4_15CS10001.tab.o lex.yy.o ass4_15CS10001_translator.o
	g++  ass4_15CS10001.tab.o lex.yy.o ass4_15CS10001_translator.o -lfl
	
lex.yy.o:	lex.yy.c	
	g++ -g -c lex.yy.c
	
ass4_15CS10001.tab.o:	ass4_15CS10001.tab.c ass4_15CS10001.tab.h
	g++ -g -c ass4_15CS10001.tab.c
	
ass4_15CS10001_translator.o:	ass4_15CS10001_translator.cxx ass4_15CS10001_translator.h ass4_15CS10001.tab.h
	g++ -g -c ass4_15CS10001_translator.cxx
	
lex.yy.c:	ass4_15CS10001.l ass4_15CS10001_translator.h ass4_15CS10001.tab.h
	flex ass4_15CS10001.l	
	
ass4_15CS10001.tab.c:	ass4_15CS10001.y ass4_15CS10001_translator.h
	bison ass4_15CS10001.y
	
ass4_15CS10001.tab.h:	ass4_15CS10001.y ass4_15CS10001_translator.h
	bison -d ass4_15CS10001.y
	
clean:
	rm a.out lex.yy.o ass4_15CS10001.tab.o ass4_15CS10001_translator.o lex.yy.c ass4_15CS10001.tab.c ass4_15CS10001.tab.h
