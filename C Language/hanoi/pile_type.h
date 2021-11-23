#ifndef pileType_h_
#define pileType_h_

/*! \brief Structure d'un élément d'une pile d'entier */
struct model_elem {
  int elem ;
  struct model_elem* suivant;
};

/*! 
  \typedef ELEMPILE 
  \brief  type ELEMPILE: un élément d'une liste d'entier 
*/
typedef struct model_elem ELEMPILE;

/*! 
  \typedef PILE 
  \brief  type PILE: un pointeur sur un ELEMPILE
*/
typedef ELEMPILE *PILE; 
 
//! \def valuer vraie  
#define TRUE 1
//! \def valeur Fausse  
#define FALSE 0

#endif
