#include <stdio.h>

#define N 5

int produit_scalaire(int A[N], int B[N])
{
  int i, res;

  res=0;
  for (i=0;i<N;i++)
    {
      res += A[i]*B[i];
    }
  return res;
}

int main()
{
  int A[N]={1,1,1,1,1};
  int B[N]={1,1,1,1,1};

  printf("le produit scalaire vaut %d\n", produit_scalaire(A,B));

  return 0;
}
	 
