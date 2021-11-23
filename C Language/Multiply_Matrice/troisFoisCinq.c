#include <stdio.h>

int troisFoisCinq()
{
  int rep;

  printf("Combien font 3 x 5 ? ");
  scanf("%d",&rep);

  while(rep!=15)
    {
      printf("Mauvaise réponse, essayez encore: combien font 3 x 5?\n");
      scanf("%d",&rep);
    }
  printf("Très bien!\n");
  return rep;
}
	
int main()
{
  
  troisFoisCinq();
  
}
