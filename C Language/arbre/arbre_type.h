#ifndef ARBRE_TYPE_H
#define ARBRE_TYPE_H
struct model_noeud
{
  int val;
  struct model_noeud *filsGauche ;
  struct model_noeud *filsDroit ;
} ;

typedef struct model_noeud NOEUD;

typedef NOEUD *ARBRE;
#endif
