#include <stdio.h>

#include "pile.h"

#define N 9
 
/*!***********************************************
* \brief  Programme de test des fonction du module pile.c
**************************************************/    

int main(int argc,char **argv)	
{
  PILE pile1;
  int val,i;

  fprintf(stdout,"/********** test de pile d'entier *************\n");

  pile1=InitPile();
  for (i=N;i>0;i--)
    {
      val=i;
      pile1=Empiler(pile1,val);
    }
  fprintf(stdout,"après mise en place d'une pile de taille %d\n",N);
  afficherPile(pile1);
  fprintf(stdout,"/**** fin du programme de test de pile d'entier **\n");
 
}



