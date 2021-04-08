
#include "Fonctions_bonus.h"

/* fonction qui met a 0 toutes les distances dans le graphe */
void reinit_distances (Graphe_zone *graphe ){
    Cellule_som *cell = graphe->som;
    while (cell != NULL){
        cell->sommet->distance = INF;
        cell = cell->suiv;
    }
}
/* la strategie qu'on a choisie a comme point de depart la strategie de la diagonale */
/* apres avoir parcouru la diagonale, on maximise la bordure en essayant de trouver les plus courts chemins vers les points strategiques: */
/* les points les plus eloignes du milieu de la matrice, les coins */
int sequence_bonus( int **M , Grille *G , int dim , int nbcl , int aff){
    Cellule_som *listeChemin=NULL;
    Zsg *Z = (Zsg *) malloc(sizeof(Zsg));
    init_Zone(Z, M, dim, nbcl); 
    Graphe_zone *graphe=Z->G;
    int essais=0;
    int cl;


    Sommet *s=(Z->G)->mat[0][0];
    ajoute_liste_sommet(s,&((Z->B[s->cl]).som));
    Z->B[s->cl].len+= s->nbcases_som;
    agrandit_Zone(Z,s->cl);

    

     BFS(graphe,(Z->G)->mat[dim/2][dim/2],(Z->G)->mat[dim/2][dim-1]);
    liste_Chemin(&listeChemin, (Z->G)->mat[dim/2][dim/2],(Z->G)->mat[dim/2][dim-1]);
    reinit_distances(Z->G);
    
    BFS(graphe,(Z->G)->mat[dim/2][dim/2],(Z->G)->mat[dim-1][dim/2]);
    liste_Chemin(&listeChemin, (Z->G)->mat[dim/2][dim/2],(Z->G)->mat[dim-1][dim/2]);
    reinit_distances(Z->G);

    BFS(graphe,(Z->G)->mat[dim/2][dim/2],(Z->G)->mat[0][dim/2]);
    liste_Chemin(&listeChemin, (Z->G)->mat[dim/2][dim/2],(Z->G)->mat[0][dim/2]);
    reinit_distances(Z->G);

    BFS(graphe,(Z->G)->mat[dim/2][dim/2],(Z->G)->mat[dim/2][0]);
    liste_Chemin(&listeChemin, (Z->G)->mat[dim/2][dim/2],(Z->G)->mat[dim/2][0]);
    reinit_distances(Z->G);
    
    /*coin bas gauche */
    BFS(graphe,(Z->G)->mat[dim/2][dim/2],(Z->G)->mat[dim-1][0]);
    liste_Chemin(&listeChemin, (Z->G)->mat[dim/2][dim/2],(Z->G)->mat[dim-1][0]);
    reinit_distances(Z->G);

   

    /*coin haut droite */
    BFS(graphe,(Z->G)->mat[dim/2][dim/2],(Z->G)->mat[0][dim-1]);
    liste_Chemin(&listeChemin, (Z->G)->mat[dim/2][dim/2],(Z->G)->mat[0][dim-1]);
    reinit_distances(Z->G);

    /*diagonale principale */
    BFS(graphe,(Z->G)->mat[0][0],(Z->G)->mat[dim-1][dim-1]);
    liste_Chemin(&listeChemin, (Z->G)->mat[0][0],(Z->G)->mat[dim-1][dim-1]);

    Cellule_som *cell=listeChemin;
    int couleur;
    while(cell){
        if(cell->sommet->marque==0){
            cell=cell->suiv;
            continue;
        }
        couleur =cell->sommet->cl;
        agrandit_Zone(Z,couleur);
        essais++;
        if(aff==1){
            afficher_Grille(Z,G,dim,couleur);
            colorier(Z,M,couleur);
        }
        cell=cell->suiv;

    }

    while(!Bordure_vide(Z->B,nbcl)){
        cl=couleur_max(Z->B,nbcl);
        agrandit_Zone(Z,cl);
       
        essais++;
        if(aff==1){
             colorier(Z,M,cl);
            afficher_Grille(Z,G,dim,cl);
        }
        
    }
    detruit_Zone(Z,dim,nbcl);
    return essais;
}