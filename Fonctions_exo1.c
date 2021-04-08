#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>

#include "Entete_Fonctions.h"

void trouve_zone_rec(int **M, int dim, int i, int j, int *taille, ListeCase *L){

  /* on garde la couleur de la case pour pouvoir la comparer apres avec les couleurs des cases adjacentes */
  int cl=M[i][j];

  /*si on a deja visite la case on passe a la suite */
  if (M[i][j]==-1) return;

  /*si on traite la case courrante cela veut dire que les couleurs correspondent donc on l'ajoute a la zone, 
  on affecte -1 a la case pour montrer que elle etait deja visitee et on augument la taille de la zone */
  M[i][j]=-1;
  ajoute_en_tete(L,i,j);
  (*taille)++;

  /*appel recursif a la fonction pour les cases adjacentes */
  /*case a gauche */
  if(0<i && cl==M[i-1][j])
    trouve_zone_rec(M,dim,i-1,j,taille,L);
  
  /* case au dessus */
  if(0<j && cl==M[i][j-1])
    trouve_zone_rec(M,dim,i,j-1,taille,L);
  
  /*case a droite */
  if(i<dim-1 && cl==M[i+1][j])
    trouve_zone_rec(M,dim,i+1,j,taille,L);
  
  /* case en dessous */
  if(j<dim-1 && cl==M[i][j+1])
    trouve_zone_rec(M,dim,i,j+1,taille,L);
}


int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff){
  int cl,i,j;
  ListeCase *L= (ListeCase *)malloc(sizeof(ListeCase)); /* la liste qui gere la zone */
  if (L==NULL){
    printf("Erreur d'allocation\n");
  }
  ListeCase cell;
  
  //srand(time(NULL));
  int taille=0; /* la taille de la zone superieure gauche */
  int cpt=0; /* le nombre d'essais */

  while(taille != dim * dim ){ 
    /*l'affichage de la grille */ 
    if(aff==1){
      for (i=0;i<dim;i++)
        for (j=0;j<dim;j++){
	        Grille_attribue_couleur_case(G,i,j,M[i][j]);
        }
      Grille_redessine_Grille();
      sleep(1);
    }  
    /* a chaque tour de boucle on reinitialise la liste et la taille de la zone*/
    init_liste(L);    
    taille=0; 
    
    /*on choisit aleatoirement une couleur differente de la couleur de la zone */
    while((cl=rand() % nbcl) == M[0][0]);
    cpt++;
    /* on fait l'appel a la fonction recursive qui met a jour la zone associee a la couleur choisie */
    trouve_zone_rec(M,dim,0,0,&taille,L);
    cell = *L;
    /* on met la couleur choisie sur les cases qui font partie de la zone */
    while(cell){
      M[cell->i][cell->j]= cl;
      cell=cell->suiv;
    }
    /* on detruit la liste qui gere la zone pour pouvoir reprendre les instructions dans le suivant tour de boucle avec une nouvelle couleur */
    detruit_liste(L);
  }
  
  /*on elibere l'espace memoire allouee a la liste qui gere la zone */
  free(L);
  return cpt-1;
}


void trouve_zone_imp(int **M, int dim, int *taille, ListeCase *L){
  int i,j;

  /* on alloue de la memoire pour la pile et on ajoute la premiere case a traiter, aka la case de coordonees 0,0 */
  ListeCase *Pile=(ListeCase *) malloc(sizeof(ListeCase)); 
  init_liste(Pile);
  ajoute_en_tete(Pile,0,0);

  /*on garde la couleur de la zone dans cl pour pouvoir la comparer avec les prochaines cases a traiter */
  int cl=M[0][0];

  /* depile et empile et verification des cases adjacentes */
  while(!test_liste_vide(Pile)){
    /* first out */
    enleve_en_tete(Pile, &i, &j);
    
    /* si on a deja parcouru la case on passe a la suite */ 
    if(M[i][j]==-1) continue;
    
    /* si la case courrante a la meme couleur que la couleur choisie, on affecte -1 a la matrice et on ajoute la case a la pile */
    if(M[i][j]==cl){
      M[i][j]=-1;
      ajoute_en_tete(L,i,j);
      (*taille)++;
    }
    /*on verifie les cases adjacentes a la case courrante et si elle a la meme couleur on l'ajoute a la pile pour etre traitee */ 
    /* case a gauche */
    if(0<i && M[i][j]==-1){
        ajoute_en_tete(Pile,i-1,j);
    }
    /* case a droite */
    if(i<dim-1 && M[i][j]==-1){
        ajoute_en_tete(Pile,i+1,j);
    }
    /* case au dessus */
    if(0<j && M[i][j]==-1){
        ajoute_en_tete(Pile,i,j-1);
    }
    /* cause en dessous */
    if(j<dim-1 && M[i][j]==-1){
        ajoute_en_tete(Pile,i,j+1);
    }
  }
  /* on elibere la memoire allouee a la pile */
  free(Pile);
}

int sequence_aleatoire_imp(int **M, Grille *G, int dim, int nbcl, int aff){
  int cl,cpt,i,j;
  int taille=1; /* la taille de la zone */
  ListeCase *L= (ListeCase *)malloc(sizeof(ListeCase)); /*la liste qui gere la zone */
  if (L==NULL){
    printf("Erreur d'allocation\n");
  }
  ListeCase cell;
  
  //srand(time(NULL));
  cpt=0;

  while(taille != dim * dim ){ 
    /*l'affichage de la grille */  
    if(aff==1){
      for (i=0;i<dim;i++)
        for (j=0;j<dim;j++){
	        Grille_attribue_couleur_case(G,i,j,M[i][j]);
        }
      Grille_redessine_Grille();
      //sleep(1);
    }  

    /* a chaque tour de boucle on reinitialise la liste et la taille de la zone*/
    init_liste(L);
    taille=0;

    /*on choisit aleatoirement une couleur differente de la couleur de la zone */
    while((cl=rand() % nbcl) == M[0][0]);
    cpt++;

    /* on fait l'appel a la fonction qui met a jour la zone associee a la couleur choisie */
    trouve_zone_imp(M,dim,&taille,L);
   
    /* on met la couleur choisie sur les cases qui font partie de la zone */
    cell = *L;
    while(cell){
      M[cell->i][cell->j]= cl;
      cell=cell->suiv;
    }

    /* on detruit la liste qui gere la zone pour pouvoir reprendre les instructions dans le suivant tour de boucle avec une nouvelle couleur */
    detruit_liste(L);
  }
  
  /*on elibere l'espace memoire allouee a la liste qui gere la zone */
  free(L);
  return cpt-1;
} 