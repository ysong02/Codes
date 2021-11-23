CFLAGS = -g -Wall -DDEBUG
LDLAGS = -g -Wall -DDEBUG

all: lireSudoku #resoudreSudoku 

utilSudoku.o: utilSudoku.c utilSudoku.h
	gcc ${CFLAGS} -c utilSudoku.c  -o utilSudoku.o

main.o:	main.c
	gcc ${CFLAGS} -c main.c -o main.o

lireSudoku: main.o utilSudoku.o
	gcc ${LDFLAGS} main.o utilSudoku.o -o lireSudoku

resoudreSudoku: sudoku.o utilSudoku.o
	gcc ${LDFLAGS} sudoku.o   utilSudoku.o -o resoudreSudoku

sudoku.o: sudoku.c sudoku.h
	gcc ${CFLAGS} -c sudoku.c  -o sudoku.o

tar-sol: 
	cd ..; tar cvf  TP2-sol.tar code/main.c code/utilSudoku.c code/utilSudoku.h code/Makefile code/sudokuInit.txt  code/sudoku1.txt code/sudoku.c

tar: 
	cd ..; tar cvf  TP2.tar code/main.c code/utilSudoku.c code/utilSudoku.h code/Makefile code/sudokuInit.txt  code/sudoku1.txt 

clean: 
	\rm -rf *.o sudoku solution lireSudoku  resoudreSudoku  *~
