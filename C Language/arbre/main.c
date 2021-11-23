#include <stdio.h>
#include <stdlib.h>

#include "arbre_type.h"
#include "arbre.h"
int  main()
{
  ARBRE t1,t2,t3;

  t1=creerArbre(10,NULL,NULL);
  t2=creerArbre(20,NULL,NULL);
  t3=creerArbre(30,t1,t2);

  afficher_arbre(t3);
  printf("hauteur: %d\n",hauteur_arbre(t3));
  return 0;
  
}
    
