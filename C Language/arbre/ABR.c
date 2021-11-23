#include <stdio.h>
#include <stdlib.h>


typedef struct noeud {
      int val;
      struct noeud *fD;
      struct noeud *fG;
} NOEUD;

typedef NOEUD *ARBRE; 

void affichageInfixe(ARBRE arbre, int decalage)
{
  int i;
  if (arbre!=NULL)
    {
      affichageInfixe(arbre->fD,decalage+3);
      for (i=0;i<decalage;i++)
	printf(" ");
      printf("%d\n",arbre->val);
      affichageInfixe(arbre->fG,decalage+3);
    }
} 

    

ARBRE ajouterABR(ARBRE a, int val)
{
  ARBRE newelem;
  
  if (a==NULL)
    { 
      newelem=(ARBRE)malloc(sizeof(NOEUD));
      newelem->val=val;
      if (newelem==NULL)
	{
	  printf("erreur d'allocation\n");
	  exit(-1);
	}
      return newelem;
    }
  else
    {
      if ((a->val)>val)
	{
	  a->fG=ajouterABR(a->fG,val);
	}
      else 
	{
	  a->fD=ajouterABR(a->fD,val);
	}
      return a;
    }
}

ARBRE rotationDroite(ARBRE a)
{
  ARBRE old,new,temp;

  if (a->fG==NULL)
    return a;
  else
    {
      old=a;
      new=a->fG;
      temp=new->fD;
      new->fD=old;
      old->fG=temp;
    }
  return new;
}

int profondeur(ARBRE a)
{
  int l,r;

  if (a==NULL)
    return(-1);
  else
    {
      l=profondeur(a->fG);
      r=profondeur(a->fD);
      return(l>r?l+1:r+1);
    }
}


int main()
{

  ARBRE a1,a2;

  a1=ajouterABR(NULL,10);
  a1=ajouterABR(a1,0);
  a1=ajouterABR(a1,5);
  a1=ajouterABR(a1,20);
  a1=ajouterABR(a1,15);
  a1=ajouterABR(a1,25);

  affichageInfixe(a1,0);
  printf("profondeur=%d\n",profondeur(a1));

  a2=ajouterABR(NULL,100);
  a2=ajouterABR(a2,105);
  a2=ajouterABR(a2,90);
  a2=ajouterABR(a2,95);
  a2=ajouterABR(a2,80);
  a2=ajouterABR(a2,85);
  a2=ajouterABR(a2,70);
  a2=ajouterABR(a2,75);
  a2=ajouterABR(a2,60);
  a2=ajouterABR(a2,65);
  a2=ajouterABR(a2,50);
  a2=ajouterABR(a2,55);
  a2=ajouterABR(a2,40);

  affichageInfixe(a2,0);
  printf("arbre a2\n");
  a2=rotationDroite(a2);
  printf("après rotation droite\n");
  affichageInfixe(a2,0);
 
  return 0;
}
  
