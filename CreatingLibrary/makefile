a.out:	main.o libass2_15CS10001.a
	gcc main.o -L. -lass2_15CS10001
	
main.o: main.c myl.h
	gcc -c main.c
	
libass2_15CS10001.a: ass2_15CS10001.o
	ar -rcs libass2_15CS10001.a ass2_15CS10001.o
	
ass2_15CS10001.o:  ass2_15CS10001.c myl.h
	gcc -c ass2_15CS10001.c
	
clean:
	rm a.out main.o libass2_15CS10001.a ass2_15CS10001.o
