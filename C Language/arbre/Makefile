all: main

main: main.o arbre.o
	gcc main.o arbre.o -o main

main.o: main.c arbre.h arbre_type.h
	gcc -c -g main.c -o main.o

arbre.o: arbre.c arbre.h arbre_type.h
	gcc -c -g arbre.c -o arbre.o

ABR: ABR.c
	gcc -Wall ABR.c -o ABR

tar-sol: clean
	cd ..; tar cvf TD5-sol.tar code/


clean:
	\rm -rf *~ *.o main ABR a.out
