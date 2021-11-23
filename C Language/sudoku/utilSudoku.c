#include <stdlib.h>
#include <stdio.h>
#include <string.h>
         
     
int lireSudoku(FILE *fich,int sudoku[9][9])
{int i,j,res;
 int val; 
 for (i=0; i<9; i++) 
   for (j=0; j<9; j++) 
     {
       res=fscanf(fich, "%d",&val);
       if (res==EOF)
         {
           fprintf(stderr,"Fin de ficher atteinte: manque des coefficients\n");
           exit(-1);
         }
       sudoku[i][j]=val;
     }
 return(0);
}

int ecrireSudoku(FILE *fich,int sudoku[9][9])
{int i,j;
 for (i=0; i<9; i++) 
   {
     for (j=0; j<9; j++) 
       {
         fprintf(fich, "%d ",sudoku[i][j]);
       }
     fprintf(fich, "\n");
   }
 return(0);
}

int sudokuValide (int sudoku[9][9])
{int i,j,k,ligne,colonne,a,b,c,d;
  for (i=0; i<9; i++)
    {
      for (j=0; j<9; j++)
        {
          for (k=j+1; k<9; k++)
            {
              if (sudoku[i][j]==sudoku[i][k])
                {
                 printf("Le probleme est dans i1=%d,j1=%d,k1=%d\n",i,j,k);
                 return(0);
                }
            }
        }
    }
  for (i=0; i<9; i++)
    {
      for (j=0; j<9; j++)
        {
          for (k=j+1; k<9; k++)
            { 
              if (sudoku[j][i]==sudoku[k][i])
                {
                 printf("Le problem est dans i=%d,j=%d,k=%d\n",i,j,k);
                 return(0);
                }   
            } 
        }
    }
  ligne=0;
  colonne=0;
  for (i=0; i<9; i++)
    {
      for (a=ligne; a<ligne+3; a++)
        {
          for (b=colonne; b<colonne+3; b++)
            {
              for (c=ligne; c<ligne+3; c++)
                {
                  for (d=colonne; d<colonne+3; d++)
                    { 
                      if( (a!=c)&&(b!=d)&&(sudoku[a][b]==sudoku[c][d]))
                        {
                          printf("Le probleme est dans i=%d,a=%d,b=%d,c=%d,d=%d,ligne=%d,colonne=%d\n",i,a,b,c,d,ligne,colonne);
                          return(0);  
                        }
                    }
                }
            }
        }      
      ligne=ligne+3;
      if (ligne==9)
        {
          ligne=0;
          colonne=colonne+3;
        }
    }
  return(1);
}

void resoudreSudoku(int sudoku[9][9])
{int M[9][9][9],i,j,k,m,n,a,b;
  for (i=0; i<9; i++)
    {
      for (j=0; j<9; j++)
        {
          if (sudoku[i][j]!=0)
            {
              for (k=0; k<9; k++)
                {
                  if (sudoku[i][j]==k+1)
                    {
                      for (m=0; m<9; m++)
                        {
                          M[m][j][k]=1;
                        }
                      for (n=0; n<9; n++)
                        {
                          M[i][n][k]=1;
                        }
                      for (a=i/3*3; a<i/3*3+3; a++)
                      {
                        for (b=j/3*3; b<j/3*3+3; b++)
                          {
                            M[a][b][k]=1;
                          }
                       }
                    }
                 }
              } 
         }
    }
  for (i=0; i<9; i++)
  {
   for (j=0; j<9; j++)
    {
     for (k=0;k<9;k++)
      {
        printf("M est %d",M[i][j][k]);
      }
    }
  }

}                  
