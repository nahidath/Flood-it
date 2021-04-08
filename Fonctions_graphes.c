#include <stdio.h>
#include <stdlib.h>

#include "Fonctions_graphes.h"


void init_Matrice(Sommet ****mat, int dim){
    /*on malloc  les lignes et les colonnes de la matrice */
    *mat=(Sommet ***) malloc(sizeof(Sommet**)*dim);
    for(int i=0;i<dim;i++){
        (*mat)[i]=(Sommet **)malloc(sizeof(Sommet *)*dim);
        for(int j=0;j<dim;j++){
            /*on initialise chaque case de la matrice a NULL */
            (*mat)[i][j]=NULL;
        }
    }
}

/* on initialise une liste chainee de sommets a NULL */
void init_CellSom(Cellule_som **cell){
    *cell=NULL;
}

/* on initialise chaque champ de la structure qui gerer le graphe */
void init_Graphe(Graphe_zone *G, int dim){
    G->nbsom=0;
    init_CellSom(&G->som);
    init_Matrice(&G->mat,dim);
}


/* on cree un nouveau sommet avec le nom s[num] et de couleur cl */
Sommet * nouveau_Sommet(int num, int cl){
    Sommet *s=(Sommet *) malloc(sizeof(Sommet));
    s->num=num;
    s->cl=cl;
    s->marque=2;
    s->cases=NULL;
    s->nbcases_som=0;
    s->sommet_adj=NULL;
    s->distance= INF;
    s->pere=NULL;
    return s;
}

/*on ajoute un sommet a une liste chainee de sommets  */
int ajoute_liste_sommet(Sommet *s, Cellule_som **cellSom){
    /* on alloue l'espace memoire neccessaire pour une case de la liste chainee de Cellule_som */
    Cellule_som *new=(Cellule_som *)malloc(sizeof(Cellule_som));
    if(new==NULL){
        printf("Erreur d'allocation\n");
        return 0;
    }
    /*on met le sommet dans la case qu'on vient de creer */
    new->sommet=s;
    /* on fait le chainage pour ajouter en tete la nouveau cellule */
    new->suiv=*cellSom;
    *cellSom=new;
    return 1;
}

/* on detruit une liste chainee de sommets */
void detruit_liste_sommet(Cellule_som **cellSom){
    Cellule_som *cell=*cellSom,*temp;
    while(cell){
        temp=cell;
        temp->sommet=NULL;
        cell=cell->suiv;
        free(temp);
    }
    *cellSom=NULL;
}

/* on detruit un sommet */
void detruit_sommet(Sommet *s){
    detruit_liste(&s->cases);
    detruit_liste_sommet(&s->sommet_adj);
    free(s);
}
/* on detruit la matrice des sommets du graphe */
void detruit_matrice(Sommet ***mat, int dim){
    for(int i=0;i<dim;i++)
        free(mat[i]);
    free(mat);
}

/* on detruit tous les champs du graphe */  
void detruit_Graphe(Graphe_zone *graphe , int dim){
    Cellule_som *tmp, *cell;
    cell=graphe->som;

    /* on elibere la memoire occuppee par les sommets de la zone et par la liste Cellule_som*/
    while (cell != NULL){
        tmp = cell;
        detruit_sommet(cell->sommet); 
        cell=cell->suiv; 
        free(tmp);
    }
    /* on detruit la matrice */
    detruit_matrice(graphe->mat,dim);

}

/*on cree les arrets du graphe pour deux sommets adjacentes; s1 est adjacent a s2 et vice versa */
int ajoute_voisin(Sommet *s1, Sommet *s2){
    return ajoute_liste_sommet(s1,&(s2->sommet_adj)) && ajoute_liste_sommet(s2,&(s1->sommet_adj));
}

/* on teste si deux sommets sont adjacents */
int adjacent(Sommet *s1, Sommet *s2){
    Cellule_som *liste =s1->sommet_adj;
    /*on parcourt la liste des sommets adjacents d'un d'entre eux et on verifie si l'autre se trouve dans cette liste */
    while(liste){
        if (liste->sommet==s2)
           return 1;
        liste=liste->suiv;
    }
    return 0;
}

Sommet *enleve_en_tete_CelluleSom(Cellule_som **cell){
    Sommet *s=(*cell)->sommet;
    Cellule_som *head=*cell;
    *cell=(*cell)->suiv;
    free(head);
    return s;
}

/* on cree le graphe qui gere les zones du jeu */
Graphe_zone cree_graphe_zone(int **M,int dim){
    Graphe_zone G;
    ListeCase caseSom;
    Sommet *s;
    int cptSommet=0; /* variable qui sert a nommer les sommets qu'on va creer */ 
    int taille=0; /* variable qui sert a stocker la taille d'une zone */
    /* on initialise tous les champs du graphe */
    init_Graphe(&G,dim);
    int i,j;
    /*on parcourt pour une premiere fois la matrice pour creer les sommets correspondants aux differentes zones de la grille */
    for (i=0;i<dim;i++){
        for(j=0;j<dim;j++){
            /* si une case ne fait pas partie d'une zone (representee par un sommet), alors il faut en creer un nouveau sommet */
            if(G.mat[i][j]==NULL){
                /*on cree le nouveau sommet de couleur correspondante a la couleur de la case courrente */
                s=nouveau_Sommet(cptSommet,M[i][j]);
                /*on le nouveau sommet a la liste des sommets du graphe */
                ajoute_liste_sommet(s,&G.som);
                taille=0;                
                /* on trouve la zone de la meme couleur que la case courrente et on met les cases de la zone dans la liste des cases du sommet  */
                trouve_zone_rec(M,dim,i,j,&taille ,&(s->cases));
                /* on met a jour la taille de la zone-sommet */
                s->nbcases_som=taille;
                /*on parcourt la liste des cases de la zone-sommet pour associer a chaque case de la matrice le nouveau sommet correspondant */
                caseSom=s->cases;
                while (caseSom){
                    G.mat[caseSom->i][caseSom->j]=s;
                    caseSom=caseSom->suiv;
                }                
                cptSommet++;
                /*on increment le nombre des sommets du graphe */
                G.nbsom++;                
            }
        }
    }
    /* on parcourt la deuxieme fois la matrice pour creer les arretes du graphe */
    Sommet ***matrice=G.mat;
    for(i=0;i<dim; i++){
        for (j=0;j<dim;j++){
            /*on teste si on ne depasse pas les dimensions de la matrice */
            if(i<dim-1)
                /* on verifie si la case a gauche de la case courrante fait partie d'une zone-sommet differente */
                if (matrice[i][j]!=matrice[i+1][j] && !adjacent(matrice[i][j],matrice[i+1][j])){
                    /*si on a rencontre un nouveau sommet qu'on n'a pas deja verifie, on l'ajoute aux listes des sommets adjacentes des deux zones-sommets */
                    ajoute_voisin(matrice[i][j],matrice[i+1][j]);
            }
            /*on teste si on ne depasse pas les dimensions de la matrice */
            if(j<dim-1)
                /* on verifie si la case en dessous de la case courrante fait partie d'une zone-sommet differente */
                if (matrice[i][j]!=matrice[i][j+1] && !adjacent(matrice[i][j],matrice[i][j+1])){
                    /*si on a rencontre un nouveau sommet qu'on n'a pas deja verifie, on l'ajoute aux listes des sommets adjacentes des deux zones-sommets */
                    ajoute_voisin(matrice[i][j],matrice[i][j+1]);

            }
        }
    }
    return G;
}

/*on affiche le graphe */
void afficher_graphe(Graphe_zone G){
    printf("Le graphe a %d sommets qui correspondent a %d zones\n\n",G.nbsom, G.nbsom);
    Cellule_som *cell=G.som;
    Sommet *sommet;
    while(cell){
        sommet=cell->sommet;
        printf("s%d a la couleur %d et la taille %d et a comme voisins : ",sommet->num, sommet->cl, sommet->nbcases_som);
        Cellule_som *listeadj=sommet->sommet_adj;
        while(listeadj){
            printf("s%d ",listeadj->sommet->num);
            listeadj=listeadj->suiv;
        }
        printf("\n");
        cell=cell->suiv;
    }
}

