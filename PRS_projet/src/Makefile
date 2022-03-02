CFLAGS = -Wall -lm

all: main

main: serveur1 serveur2 serveur3

serveur1: serveur1-lePic.c
	gcc ${CFLAGS} serveur1-lePic.c -o ../bin/serveur1-lePic

serveur2: serveur2-lePic.c
	gcc ${CFLAGS} serveur2-lePic.c -o ../bin/serveur2-lePic

serveur3: serveur3-lePic.c
	gcc ${CFLAGS} serveur3-lePic.c -o ../bin/serveur3-lePic

test: serveurTest.c
	gcc ${CFLAGS} serveurTest.c -o ../bin/serveurTest

tar:
	cd ../..; tar cvf lePic.tar.gz PRS_projet
