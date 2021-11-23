#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

/*!***********************************************
* \brief  affiche l'erreur et quitte le programme 
* \param message une chaine de caractère à afficher avant de quitter
**************************************************/    
int error1(char *message)	/*affiche l'erreur et sort*/
{
  fprintf(stderr,"Error: %s\n",message);
  exit(-1);
}

/*!***********************************************
* \brief affiche une pile d'entier
**************************************************/    
void afficherPile(PILE pile)	
{ELEMPILE *visitor;
 
 visitor=pile;
 while(visitor!=0)
   {
     fprintf(stdout,"|%d|",visitor->elem);
     visitor=visitor->suivant;
   }
 fprintf(stdout,"|--\n");
}	
/*****************************************************************/



/*!***********************************************
* \brief  initialise une pile
**************************************************/    
PILE InitPile()	
{
  PILE pile1;
  
  pile1=(ELEMPILE *)NULL;
  
  return(pile1);
}
/*****************************************************************/



/*!***********************************************
* \brief     ajoute l'élément elem au sommet de la pile pointée par ppile
**************************************************/    
PILE Empiler (PILE pile, int elem)	
{
  ELEMPILE *newelem;  
  
  newelem=(ELEMPILE *)malloc(sizeof(ELEMPILE));
  if (newelem==0) error1("Empiler: plus de place mémoire");
  
  newelem->elem=elem;
  newelem->suivant=pile;
  return newelem;
  
}
/*****************************************************************/

/*!***********************************************
* \brief    Retire l'element du sommet de la pile  et renvoit sa valeur
**************************************************/    
int Depiler(PILE *ppile)	
{
  ELEMPILE *elem1;
  int val; 
  
  if (*ppile==NULL)
    error1("Depiler: pile vide");
  elem1=*ppile;
  val=elem1->elem;
  *ppile=elem1->suivant;
  free(elem1);
  return(val);
}
/*****************************************************************/


/*!***********************************************
* \brief    Vide une pile complètement
* \param ppile un pointeur vers la pile 
* \pre Precondition: ppile pointe sur une pile valide
* \post Postcondition: La pile est vide, ppile pointe vers un pointeur Null 
* \result Le résultat de la fonction est 0 si il n'y a pas d'erreur
**************************************************/    
int ViderPile(PILE *ppile)	/*vider la pile*/
{
  ELEMPILE *visitor,*temp;
  
  for(visitor=*ppile; visitor!=NULL; visitor=visitor->suivant)
    {
      temp=visitor;
      visitor=visitor->suivant;
      free(temp);
    }
  *ppile=(ELEMPILE *)NULL;
  return(0);
}     



