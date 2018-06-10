code:	ass5_15CS10001.tab.o lex.yy.o ass5_15CS10001_translator.o ass5_15CS10001_target_translator.o
	g++ -o code ass5_15CS10001.tab.o lex.yy.o ass5_15CS10001_translator.o ass5_15CS10001_target_translator.o -lfl
	
lex.yy.o:	lex.yy.c	
	g++ -g -c lex.yy.c
	
ass5_15CS10001.tab.o:	ass5_15CS10001.tab.c ass5_15CS10001.tab.h
	g++ -g -c ass5_15CS10001.tab.c
	
ass5_15CS10001_translator.o:	ass5_15CS10001_translator.cxx ass5_15CS10001_translator.h ass5_15CS10001.tab.h
	g++ -g -c ass5_15CS10001_translator.cxx
	
ass5_15CS10001_target_translator.o:		ass5_15CS10001_target_translator.cxx ass5_15CS10001_translator.h ass5_15CS10001.tab.h
	g++ -g -c ass5_15CS10001_target_translator.cxx
	
lex.yy.c:	ass5_15CS10001.l ass5_15CS10001_translator.h ass5_15CS10001.tab.h
	flex ass5_15CS10001.l	
	
ass5_15CS10001.tab.c:	ass5_15CS10001.y ass5_15CS10001_translator.h
	bison ass5_15CS10001.y
	
ass5_15CS10001.tab.h:	ass5_15CS10001.y ass5_15CS10001_translator.h
	bison -d ass5_15CS10001.y
	
libass2_15CS10001.a:	ass2_15CS10001.o
	ar -rcs libass2_15CS10001.a ass2_15CS10001.o
	
ass2_15CS10001.o:	ass2_15CS10001.c
	gcc -c ass2_15CS10001.c
	
clean:
	rm code lex.yy.o ass5_15CS10001.tab.o ass5_15CS10001_translator.o ass5_15CS10001_target_translator.o lex.yy.c ass5_15CS10001.tab.c ass5_15CS10001.tab.h
	
run:
	./code <ass5_15CS10001_test4.c >output.s
	gcc -c ass2_15CS10001.c
	ar -rcs libass2_15CS10001.a ass2_15CS10001.o
	gcc -c output.s
	gcc output.o -L. -lass2_15CS10001
	
remov:
	rm a.out output.s output.o
