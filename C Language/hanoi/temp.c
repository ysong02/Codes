#include <stdio.h>
#include "pile.h"
#include "hanoi.h"

/*!***********************************************
* \brief     d�place le sommet de pile d'une pile source (*psource) vers une pile destination (*pdest)
* \param psource un pointeur vers la pile source  
* \param pdest un pointeur vers la pile destination  
* \pre Les deux piles point�es par psource et pdest sont 
        des piles de Hanoi valides (chaque �l�ment est plus petit 
	que le suivant dans la pile). La pile point�e par psource 
	est non-vide. ppile pointe sur une pile valide
* \post Si le sommet de la pile point�e par psource
est de plus petite taille que le sommet de la pile point�e par 
pdest les deux piles sont modifi�es: le sommet de pile de *psource 
est enlev� et rajout� � *pdest, Sinon la fonction affiche une 
erreur et  interrompt le programme. 
* \result La fonction renvoie 0 si aucune erreur n'est intervenue  
**************************************************/    
int deplacer(PILE *psource, PILE *pdest)
{int element;

 if (*psource==NULL) error1("deplacer: pile source vide");
 element=Depiler(psource);
 if (*pdest!=NULL) 
   if (element > (*pdest)->elem)
     error1("deplacer: op�ration invalide");
 Empiler(pdest,element);
 return(0);
}
/*****************************************************************/


/*!***********************************************
* \brief resoud le probl�me de Hanoi de taille N de la pile *pd�part vers la pile *parrive en utilisant la pile de passage *ppassage.
* \param pdepart un pointeur vers la pile de d�part  
* \param parrive un pointeur vers la pile d'arriv�e  
* \param ppassage un pointeur vers la pile de passage
* \param N Le nombre d'�l�ments � transferer de *pdepart � *parrive
* \pre Les trois piles point�es par pdepart, ppassage et 
        parrive sont des piles de Hanoi valides (chaque �l�ment est plus 
	petit que le suivant dans la pile). Les piles point�es par ppassage 
	et parrive sont telles qu'elle peuvent empiler les N premiers 
	�l�ments de la pile point�e par pdepart. 
* \post  Une tour de Hanoi de taille N a �t� d�plac�e de la pile 
	point�e par pdepart vers la pile point�e par parrive uniquement en 
	utilisant des d�placements valides. Si La tour de d�part contient moins
	de N �l�ments ou si le d�placement est impossible, une erreur 
	doit interompre le programme 
* \result La fonction renvoie 0 si aucune erreur n'est intervenue  
**************************************************/    
int hanoi(PILE *pdepart,PILE *ppassage, PILE *parrive,int N)
{int element;

 if (N==0)
   {
     return(0);
   }
 else
   {
     hanoi(pdepart, parrive, ppassage, N-1);
     deplacer(pdepart, parrive);
     hanoi(ppassage, pdepart, parrive, N-1);
   }
 return(0);
}
/*****************************************************************/
