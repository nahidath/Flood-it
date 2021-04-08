#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Fonctions_exo5.h"


/* fonction qui initialise une case du tableau de bordure*/
void init_Bordure_Zsg(Bordure_Zsg *B){
    B->len=0;
    B->som=NULL;
}

/* initialisation de la structure principale du programme */
void init_Zone(Zsg *Z, int **M,int dim, int nbcl){
    /* les champs de dim et nbcl seront initialisees avec les valeurs correspondantes de la matrice*/
    Z->dim=dim;
    Z->nbcl=nbcl;
    /*la liste des cases de la zone superieure gauche est initialement vide */
    Z->cellSom=NULL;
    /*on initialise chaque case du tableau qui gere la bordure*/
    Z->B= (Bordure_Zsg *) malloc(sizeof(Bordure_Zsg)* nbcl);
    for(int i=0; i<nbcl; i++)
        init_Bordure_Zsg(Z->B+i);
    /* on cree directement le graphe des zones a l'aide des fonctions definies precedement*/
    Z->G= (Graphe_zone *) malloc(sizeof(Graphe_zone));
    *(Z->G)=cree_graphe_zone(M,dim);
}
/* fonction qui detruit la liste chainee d'une case du tableau */
void detruit_Bordure_Zsg(Bordure_Zsg *B){
    detruit_liste_sommet(&(B->som));
}

/* fonction qui elibere la  memoire allouee pour la structure zone*/
void detruit_Zone(Zsg *Z, int dim, int nbcl){
    detruit_liste_sommet(&(Z->cellSom));
    for(int i=0;i<nbcl; i++){
        detruit_Bordure_Zsg((Z->B)+i);
    }
    free(Z->B);
    detruit_Graphe(Z->G, dim);
    free(Z->G);
    free(Z);
}

/* fonction d'affichage des cases d'une liste chainee*/
void affiche_ListeCase(ListeCase *L){
    ListeCase temp=*L;
    printf("Liste de cases contenues dans le sommet: \n");
    while(temp){
        printf("Case (%d, %d)\n",temp->i, temp->j);
        temp=temp->suiv;
    }
}


// void affiche_Cell(Cellule_som *c){
//     Cellule_som *temp=c;
//     while(temp){
//         printf("s%d \t",temp->sommet->num);
//         temp=temp->suiv;
//     }
//     printf("\n");
// }


/* fonction qui renvoie 1 si toutes les cases du tableau bordure sont vides */
int Bordure_vide(Bordure_Zsg *B,int nbcl){
    for(int i=0;i<nbcl;i++){
        if(B[i].som!=NULL)
            return 0;
    }
    return 1;
}

/* fonction qui retourne la couleur la plus representee dans la bordure */
int couleur_max(Bordure_Zsg *B, int nbcl){
    int max=0;
    int cl=-1;
    for(int i=0;i<nbcl;i++){
        if( max<B[i].len){
            max=B[i].len;
            cl=B[i].som->sommet->cl;
        }
    }
    return cl;
}

/* fonction qui fait le mis a jour de la bordure et de la zone superieure gauche*/
void agrandit_Zone(Zsg *Z, int cl){
    Sommet *sCurr;
    Sommet *s;
    Cellule_som *listeAdj, *cell;
    /* on regarde les cases de la bordure de couleur cl */
    cell=((Z->B+cl)->som);
    while(cell){
        /* on les ajoute a la zone superieure gauche, vu qu'elles ont la meme couleur que la Zsg */
        ajoute_liste_sommet(cell->sommet,&(Z->cellSom));
        cell->sommet->marque=0;
        //cell->sommet->cl=cl;
        sCurr=cell->sommet;
        /* on regarde mainentant les sommets adjacents au sommet courrant pour pouvoir mettre a jour la bordure */
        listeAdj=sCurr->sommet_adj;
        while(listeAdj){
            s=listeAdj->sommet;
            /* si le sommet ne fait pas deja partie de la bordure, on l'ajoute a la liste chainee de la case de couleur correspondante */
            if(s->marque==2){
                ajoute_liste_sommet(s,&((Z->B[s->cl]).som));
                /* on modifie aussi le fait que maintenant le sommet est dans la bordure */
                s->marque=1;
                /* on ajoute a la longeur de la liste le nombre de cases du sommet qu'on vient d'ajouter */
                Z->B[s->cl].len+= s->nbcases_som;
            }
            /* on passe au sommet voisin suivant */
            listeAdj=listeAdj->suiv;
        }
        /* on passe au sommet suivant dans la liste de bordure de couleur cl*/
        cell=cell->suiv;
    }
    /* apres avoir regarde tous les sommets qui appartiennent a la bordure de couleur cl, on peut les enlever, en n'oublient pas de  mettre aussi la longueur a 0 */
    detruit_liste_sommet(&((Z->B[cl]).som)); 
    (Z->B[cl]).len=0;
   
}

/* fonction qui colorie les cases de la zone superieure gauche en fonction de la couleur cl pour faire l'affichage graphique */
void colorier(Zsg *Z, int **M, int cl){
    Cellule_som *cell = Z->cellSom;
        while(cell){
            ListeCase liste=cell->sommet->cases;
            while(liste){
                M[liste->i][liste->j]= cl;
                liste=liste->suiv;
            }
            cell=cell->suiv;
        }
}

/* fonction qui fait le mis a jour de la grille graphique */
void afficher_Grille(Zsg *Z, Grille *G, int dim, int cl){
    for (int i=0;i<dim;i++)
        for (int j=0;j<dim;j++){
            if (Z->G->mat[i][j]->marque != 0){
                Grille_attribue_couleur_case(G,i,j,Z->G->mat[i][j]->cl);
            }
            else{
                Grille_attribue_couleur_case(G,i,j,cl);
            }
        }
    Grille_redessine_Grille();
    sleep(1);
}        

/* fonction qui resoud le jeu en appliquant la strategie max_bordure */
int max_bordure(int **M, Grille *G,int dim, int nbcl, int aff){
    int cl;
    int essais=0;
    /* on initialise la zone qui gere le jeu */
    Zsg *Z = (Zsg *) malloc(sizeof(Zsg));
    init_Zone(Z, M, dim, nbcl); 

    /* on ajoute le sommet associe a la case 0,0 a la bordure*/
    Sommet *s = (Z->G->mat)[0][0] ;
    ajoute_liste_sommet(s,&((Z->B[s->cl]).som));
    Z->B[s->cl].len+= s->nbcases_som;

    /* on applique la fonction agrandit_Zone jusqu'au moment ou la bordure est vide, aka tous les sommets font partie de la Zsg */
    while(!Bordure_vide(Z->B,nbcl)){
        /* on choisit la couleur qui est la plus representee dans la bordure pour l'efficacite */
        cl=couleur_max(Z->B,nbcl);
        /* on fait le mis a jour de la zone et de la bordure en fonction de la couleur choisie */
        agrandit_Zone(Z,cl);
        essais++;
        if(aff==1){
            afficher_Grille(Z,G,dim,cl);
            colorier(Z,M,cl);
        }

        
    }
    /* on elibere la memoire alloue par la zone qui gerait le jeu */
    detruit_Zone(Z,dim,nbcl);
    return essais-1;
}