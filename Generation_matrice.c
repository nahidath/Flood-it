#include "Generation_matrice.h"

int ** generer(int dim){
    int **M=(int **) malloc(sizeof(int*)*dim);
    int i;
  for (i=0;i<dim;i++){
    M[i]=(int*) malloc(sizeof(int)*dim);
    if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
  }
    return M;


}