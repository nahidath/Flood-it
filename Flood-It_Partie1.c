#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "API_Grille.h"
#include "API_Gene_instance.h"
#include "Entete_Fonctions.h"
#include "Liste_case.h"
#include "Fonctions_exo3.h"
#include "fonctions_test.h"
#include "Generation_matrice.h"
 
 
int main(int argc,char**argv){

  int dim, nbcl, nivdif, graine, exo, aff;
  Grille *G;
  int i,j;
  int **M;

  clock_t
    temps_initial, /* Temps initial en micro-secondes */
    temps_final;   /* Temps final en micro-secondes */
  float
    temps_cpu;     /* Temps total en secondes */ 
 


  if(argc!=7){
    printf("usage: %s <dimension> <nb_de_couleurs> <niveau_difficulte> <graine> <exo:0-1-2-3> <aff 0/1>\n",argv[0]);
    return 1;
  }

  dim=atoi(argv[1]);
  nbcl=atoi(argv[2]);
  nivdif=atoi(argv[3]);
  graine=atoi(argv[4]);
  exo=atoi(argv[5]);
  aff=atoi(argv[6]);

  /* Generation de l'instance */

  M=(int **) malloc(sizeof(int*)*dim);
  for (i=0;i<dim;i++){
    M[i]=(int*) malloc(sizeof(int)*dim);
    if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
  }

  

  /* Affichage de la grille */

  if (aff==1){
    Grille_init(dim,nbcl, 500,&G);

    Grille_ouvre_fenetre(G);

    for (i=0;i<dim;i++)
      for (j=0;j<dim;j++){
	      Grille_attribue_couleur_case(G,i,j,M[i][j]);
      }

    Grille_redessine_Grille();
  }

 
Gene_instance_genere_matrice(dim, nbcl, nivdif, graine, M);


  temps_initial = clock ();
  if (exo==1){
     printf("%d essais\t", sequence_aleatoire_rec(M, G, dim, nbcl, aff)); 
  }

  if (exo==2){
     printf("%d essais\t", sequence_aleatoire_imp(M, G, dim, nbcl, aff)); 
  }
  
  if (exo==3){
    printf("%d essais\t",sequence_aleatoire_rapide(M,G,dim,nbcl,aff));
  }

  temps_final = clock ();
  temps_cpu = (temps_final - temps_initial) * 1e-6;
  printf("%f\n",temps_cpu);


  if (aff==1){
    Grille_ferme_fenetre();

    Grille_free(&G);
  }

  for(int i = 0 ; i< dim ; i++)
    free(M[i]);
  free(M);
  return 0;
}