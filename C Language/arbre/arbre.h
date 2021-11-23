#ifndef ARBRE_H
#define ARBRE_H
#include "arbre_type.h"

ARBRE creerArbre(int val, ARBRE filsGauche, ARBRE filsDroit);
void afficher_noeud(NOEUD n,int decalage);
void afficher_arbre_interne(ARBRE a, int decalage);
void afficher_arbre(ARBRE a);
int hauteur_arbre(ARBRE a);


#endif
