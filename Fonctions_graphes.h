#ifndef __FONCTIONS_GRAPHES__
#define __FONCTIONS_GRAPHES__

#define INF 9999999

#include "Entete_Fonctions.h"
#include "API_Gene_instance.h"

typedef struct sommet Sommet;

/* Element d'une liste chainee de pointeurs sur Sommets */
typedef struct cellule_som{
    Sommet *sommet;
    struct cellule_som *suiv;
}Cellule_som;

struct sommet {
    int num; /* Numero du sommet (sert uniquement a l'affichage)*/
    int cl; /* Couleur d'origine du sommet-zone */ 
    int marque; /* Indique le statut d'un sommet : 0 si dans Zsg, 1 si dans Bordure 2 si pas encore visite */
    int distance; /* Nombre d’aretes reliant ce sommet a la racine du parcours en largeur */
    Sommet *pere; /* Pere du sommet dans l’arborescence du parcours en largeur */
    ListeCase cases; /* Listes des cases du sommet-zone */
    int nbcases_som; /* Nombre de cases de cette liste */
    int ok;
    Cellule_som *sommet_adj; /*Liste des arretes pointeurs sur les sommets adjacents */
};

typedef struct graphe_zone {
    int nbsom; /* Nombre de sommets dans le graphe */
    Cellule_som *som; /* Liste chainee des sommets du graphe */
    Sommet ***mat; /* Matrice de pointeurs sur mes sommets indiquant a quel sommet appartient une case i,j de la grille */ 
} Graphe_zone;


/*fonction qui initialise la structure Graphe_zone */
void init_Graphe(Graphe_zone *G,int dim);

void init_Matrice(Sommet ****mat, int dim);

/* fonction qui initialise la structure Cellule_som */
void init_CellSom(Cellule_som **cell);


/*fonction qui detruit un sommet */
void detruit_sommet(Sommet *s);

/*fonction qui elibere la memoire alloue pour la matrice des sommets */
void detruit_matrice(Sommet ***mat, int dim);

/* fonction qui elibere la memoire occuppe par le graphe */
void detruit_Graphe (Graphe_zone *G, int dim);

/*fonction qui initialise la structure Sommet */
Sommet* nouveau_Sommet(int num, int cl);

/*fonction qui ajoute un pointer Sommet a une liste chainee de Cellule_som */
int ajoute_liste_sommet(Sommet *s, Cellule_som **cellSom);

/* fonction qui detruit une liste chainee de Cellule_som sans detruire les sommets pointees par cette liste */
void detruit_liste_sommet(Cellule_som **cellSom);

/* fonction qui met a jour deux sommets en indiquant qu'ils sont adjacents l'un de l'autre */
int ajoute_voisin(Sommet *s1, Sommet *s2);

/*fonction qui verifie si deux sommets sont adjacents */
int adjacent(Sommet *s1, Sommet *s2);

Sommet *enleve_en_tete_CelluleSom(Cellule_som **cell);

/* fonction qui va creer le graphe tout entier */
Graphe_zone cree_graphe_zone(int **M,int dim);

/*fonction qui affiche le graphe */
void afficher_graphe(Graphe_zone G);


#endif