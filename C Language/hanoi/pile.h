#ifndef pile_h_
#define pile_h_

#include "pile_type.h"

/************************************************
* \brief  affiche l'erreur et quitte le programme 
* \param message une chaine de caractère à afficher avant de quitter
**************************************************/    
int error1(char *message);	

/************************************************
* \brief  initialise une pile
**************************************************/    
PILE InitPile();

/************************************************
* \brief     ajoute l'élément elem au sommet de la pile pointée par ppile
* \param ppile un pointeur vers la pile 
* \param elem L'élément à rajouter au sommet de pile
* \pre Precondition: ppile pointe sur une pile valide
* \post Postcondition: La pile pointée par ppile a été modifiée:
          un élément de valeur elem a été rajouté au sommet de pile. 
* \result La fonction renvoie 0 si aucune erreur n'est intervenue  
**************************************************/    
PILE Empiler (PILE pile, int);


/************************************************
* \brief    Retire l'element du sommet de la pile pointée par ppile et renvoie 
   sa valeur. 
* \param ppile un pointeur vers la pile 
* \pre Precondition: ppile pointe sur une pile valide
* \post Postcondition: La pile pointée par ppile a été modifiée: son premier 
          élément a été supprimé (le deuxième élément est donc devenu 
	  le sommet de pile). 
* \result Le résultat de la fonction est la 
	  valeur de l'élément supprimé si aucune erreur n'est intervenue
* \remark    Si la pile pointé par ppile est vide, la fonction affiche une erreur et interrompt le programme.
**************************************************/    
int Depiler(PILE* pile);	

/************************************************
* \brief affiche une pile d'entier
* \param liste une liste d'entier
**************************************************/    
void afficherPile (PILE pile);

#endif
