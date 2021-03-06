#include <stdio.h>
#include <stdlib.h>

#include "arbre_type.h"
#include "arbre.h"


ARBRE creerArbre(int val, ARBRE filsGauche, ARBRE filsDroit)
{
  ARBRE temp;

  temp= (ARBRE)malloc(sizeof(NOEUD));
  temp->val=val;
  temp->filsGauche=filsGauche;
  temp->filsDroit=filsDroit;

  return(temp);
}


void afficher_noeud(NOEUD n,int decalage)
{
  for (int i=0;i<decalage;i++)
    printf(" ");
  printf("%d\n",n.val);
}

void  afficher_arbre_interne(ARBRE a, int decalage)
{
  // parcours infixe
  if (a!=NULL)
    {
      afficher_arbre_interne(a->filsGauche,decalage+2);
      afficher_noeud(*a,decalage);
      afficher_arbre_interne(a->filsDroit,decalage+2);
    }
}
  

void afficher_arbre(ARBRE a)
{
  afficher_arbre_interne(a,0);
}

#define max(a,b) a<b?b:a 
int hauteur_arbre(ARBRE a)
{
  if (a==NULL) return -1;
  else return max(hauteur_arbre(a->filsGauche)+1,
		    hauteur_arbre(a->filsDroit)+1);
}
