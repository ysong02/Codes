#########################################################
# Makefile pour les programme du TD3 AGP
# Tanguy Risset, 2005
#########################################################
CC            =gcc 
LD	      =gcc
CFLAGS	+= -Wall -c -O3  -g -I./

all: main main2

main.o: main.c pile.h pile_type.h
	${CC} ${CFLAGS}  main.c -o main.o 

main2.o: main2.c pile.h pile_type.h
	${CC} ${CFLAGS}  main2.c -o main2.o 

pile.o: pile.c pile.h pile_type.h
	${CC}  ${CFLAGS} pile.c -o pile.o 

hanoi.o: hanoi.c pile.h pile_type.h
	${CC}  ${CFLAGS} hanoi.c -o hanoi.o 

main: main.o pile.o hanoi.o
	${LD} ${LDFLAGS} main.o hanoi.o pile.o -o main

main2: main2.o pile.o hanoi.o
	${LD} ${LDFLAGS} main2.o hanoi.o pile.o -o main2


tar : dummy clean
	cd ..; tar cvf TP1-init.tar code/*.h code/main.c  

tarSol : clean dummy
	cd ..; tar cvf TP1-sol.tar  code/*.h code/*.c code/Makefile 


clean:
	-rm -rf *~ *.out main *.o *.dvi *.aux *.log *.ps *.pdf html 

dummy:
