CFLAGS = -g -Wall -DDEBUG 
LDFLAGS = -g -Wall -DDEBUG

all: main 

utilMatrice.o: utilMatrice.c utilMatrice.h matrice.h
	gcc ${CFLAGS} -c utilMatrice.c  -o utilMatrice.o

matProd.o: matProd.c matProd.h matrice.h
	gcc ${CFLAGS} -c matProd.c -o matProd.o

main.o:	main.c matrice.h
	gcc ${CFLAGS} -c main.c -o main.o


main: main.o matProd.o utilMatrice.o
	gcc ${LDFLAGS} main.o matProd.o utilMatrice.o -o main

mainSol: main.o matProdSol.o utilMatrice.o
	gcc ${LDFLAGS} main.o matProdSol.o utilMatrice.o -o main


tar: clean
	cd ..; tar cvf  TD3-code.tar code/main.c code/utilMatrice.* code/matProd.* code/Makefile code/matrice* 

troisFoisCinq: troisFoisCinq.c
	gcc -Wall troisFoisCinq.c -o troiFoisCinq 

produitScalaireSol: produitScalaireSol
	gcc -Wall produitScalaireSol.c -o produitScalaireSol

tar-sol: 
	cd ..; tar cvf  TD3-sol.tar code/troisFoisCinq.c code/produitScalaireSol.c code/matProdSol.c code/main.c code/utilMatrice.* code/matProd.* code/Makefile code/matrice* 

clean: 
	\rm -rf *.o main  *~ troisFoisCinq produitScalaireSol a.out
