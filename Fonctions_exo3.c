#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>

#include "Fonctions_exo3.h"
#include "Entete_Fonctions.h"
#include "API_Gene_instance.h"
#include "API_Grille.h"


void init_Zsg(S_Zsg *Z, int dim, int nbcl){
    /*on initialise les champs de la structure correspondantes a la dimension et au nombre de couleurs de jeu*/
    Z->dim=dim;
    Z->nbcl=nbcl;
    
    /*on initialise la liste des cases de la zone a NULL*/
    init_liste(&(Z->Lzsg));

    /*on alloue d'espace memoire pour le tableau qui gere la bordure*/ 
    /*la dimension du tableau correspond au nombre de couleurs choisi par l'utilisateur*/
    Z->B=(ListeCase *) malloc(nbcl * sizeof(ListeCase));
    if (Z->B==NULL){
        printf("Erreur d'allocation\n");
        return;
    }
    for(int i=0; i<nbcl; i++){
        /*on initialise chaque case du tableau a NULL*/
        init_liste(Z->B+i);
    }

    /*on alloue d'espace memoire pour la matrice d'appartenance*/
    /*la dimension de la matrice correspond a la dimension de la grille*/
    Z->App= (int**) malloc(sizeof(int*)*dim);
    if (Z->App==NULL){
    printf("Erreur d'allocation\n");
  }
    for(int i=0; i<dim;i++){
        /*on alloue d'espace memoire pour les colonnes de la matrice*/
        Z->App[i]=(int*) malloc(sizeof(int)*dim);
        if (Z->App[i]==NULL){
            printf("Erreur d'allocation\n");
            return;
        }
        for (int j=0; j<dim; j++){
            /*on initialise chaque case de la matrice a -2, c'est a dire aucune case n'etait visitée*/
            Z->App[i][j]=-2;
        }
    }
}


void detruit_Zsg(S_Zsg *Z){
    /* on elibere la memoire occupee par les elements du tableau Lzsg, c'est a dire les cases qui appartiennent a la zone superieure gauche*/
    detruit_liste(&Z->Lzsg);
    /* pour chaque case du tableau B, on detruit les listes chainees sur lesquelles B[i] pointe */
    for(int i=0;i<Z->nbcl; i++){
        detruit_liste(Z->B+i);
    }
    free (Z->B);
    /* on elibere la memoire allouee a la matrice d'appartenance */
    for(int i=0;i<Z->dim; i++){
        free(Z->App[i]);
    }
    free (Z->App);
    /* on elibere l'espace alloue pour la structure */
    free(Z);
}


int ajoute_Zsg(S_Zsg *Z, int i, int j){
    /*on affecte a la case dans la matrice d'appartenance la valeur qui montre que la case est dans la zone */
    Z->App[i][j]=-1;
    /*on ajoute la case a la liste chainee qui gere la zone superieure gauche */
    return ajoute_en_tete(&(Z->Lzsg), i, j);
}

int ajoute_Bordure(S_Zsg *Z, int i, int j, int cl){
    /*on affecte a la case dans la matrice d'appartenance la valeur qui montre que la case est dans la bordure*/
    Z->App[i][j]=cl;
    /*on ajoute la case a la liste chainee correspondante a sa couleur dans le tableau qui gere la bordure */
    return ajoute_en_tete(&(Z->B[cl]),i,j);
}

int appartient_Zsg(S_Zsg *Z, int i, int j){
    /* si la valeur de la case dans la matrice d'appartenance est -1, cela veut dire que la case apparteint a la zone */
    return Z->App[i][j]==-1;
}

int appartient_Bordure(S_Zsg *Z, int i, int j, int cl){
    /* si la valeur de la case dans la matrice d'appartenance corresponde a la couleur cl, cela veut dire que la case apparteint a la bordure */
    return Z->App[i][j]==cl;
}

int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l){
    int taille=0;/* le nombre de cases ajoutees a la zone */
    int couleurCurr;/* la couleur de la case visite dans la pile */
    int i,j;
    ListeCase *Pile=(ListeCase*)malloc(sizeof(ListeCase));
    if (Pile==NULL){
        printf("Erreur d'allocation\n");
        return -1;
    }
    init_liste(Pile);
    /* on ajoute la premiere  case a traiter dans la pile */
    ajoute_en_tete(Pile,k,l);
    /* on commence le parcours de la pile */
    while(!test_liste_vide(Pile)){
        /*first out */
        enleve_en_tete(Pile, &i,&j);
        couleurCurr=M[i][j];
        
        /* si la case courrante a la meme couleur que la zone mais elle n'est pas deja dans la liste, on l'ajoute et on increment la taille de la zone*/
        if(couleurCurr==cl && !appartient_Zsg(Z,i,j)){
            ajoute_Zsg(Z,i,j);
            taille++;
        }
        
        /* si la case courrante a une autre couleur que la zone cela veut dire qu'on est en train de visiter une case qui appartient a la bordure*/
        /* des qu'on arrive dans ce cas, il n'est pas neccessaire de parcourir les cases adjacentes, donc on passera a la case suivante a traiter dans la pile */
        if(couleurCurr!=cl){
            /* si la case n'est pas encore dans la bordure il faut bien evidemment l'ajouter */
            if(!appartient_Bordure(Z,i,j,couleurCurr)){
                ajoute_Bordure(Z,i,j,couleurCurr);
            }
            continue;
        }

        /* on ajoute dans la pile les cases adjacentes a la case courrente qui n'etaient pas encore traitees */ 
        /* case a gauche */
        if(0<i){
            if(!appartient_Bordure(Z,i-1,j,M[i-1][j]) && !appartient_Zsg(Z,i-1,j)){
                ajoute_en_tete(Pile,i-1,j);
            }
        }
        /* case a droite */
        if(i<Z->dim-1){
            if(!appartient_Bordure(Z,i+1,j,M[i+1][j]) && !appartient_Zsg(Z,i+1,j)){
                ajoute_en_tete(Pile,i+1,j);
            }
        }

        /* case au dessus */
        if(0<j){
            if(!appartient_Bordure(Z,i,j-1,M[i][j-1]) && !appartient_Zsg(Z,i,j-1)){
                ajoute_en_tete(Pile,i,j-1);
            }
        }
        /* case en dessous */
        if(j<Z->dim-1){
            if(!appartient_Bordure(Z,i,j+1,M[i][j+1]) && !appartient_Zsg(Z,i,j+1)){
                ajoute_en_tete(Pile,i,j+1);
            }
        }
        
    }
    /* on elibere la memoire allouee a la pile */
    free(Pile);
    return taille;
}

int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff){
    int cl,cpt,taille,i,j;
    S_Zsg *Z= (S_Zsg *)malloc(sizeof(S_Zsg)); 
    if (Z==NULL){
        printf("Erreur d'allocation\n");
        return -1;
    }
    init_Zsg(Z,dim,nbcl);
    ListeCase cell;
    ListeCase *bordure;
    taille=agrandit_Zsg(M,Z,M[0][0],0,0);      


    //srand(time(NULL));
    cpt=0;
    /* la couleur de debut sera la couleur de la case de debut, aka la case 0,0 */
    cl =M[0][0];
    
    while(taille != dim * dim ){    
        /* mettre la couleur choisie dans toutes les cases de la zone */
        cell = Z->Lzsg;
        while(cell){
            M[cell->i][cell->j]= cl;
            cell=cell->suiv;
        }
        /* l'affichage de la grille */
        if(aff==1){
            for (i=0;i<dim;i++)
                for (j=0;j<dim;j++){	        
                    Grille_attribue_couleur_case(G,i,j,M[i][j]);
                }
            Grille_redessine_Grille();
            //sleep(1);
        }       
        /*on choisit une couleur differente de la couleur de la zone, et on incremente le compteur pour les essais */
        while((cl=rand() % nbcl) == M[0][0]);
        cpt++;    
        bordure=Z->B;
        /* on parcourt la liste des cases de couleur choisie aleatoirement de la bordure */
        /*on enleve chaque case de la bordure pour pouvoir la mettre dans la zone a l'aide de la fonction agradit_Zsg */ 
        /*la fonction enleve_en _tete supprime automatiquement l'element dans la memoire, donc pas neccessaire de faire detruit_liste a la fin de la boucle sur B[cl] */
        while(bordure[cl]!=NULL){
            enleve_en_tete(&bordure[cl],&i,&j);
            taille+=agrandit_Zsg(M,Z,cl,i,j);
        }
    }
    /* on elibere la memoire occuppee par la liste chainee qui gere la zone superieure gauche */
    detruit_Zsg(Z);
    return cpt;
    }