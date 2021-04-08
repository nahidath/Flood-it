#ifndef __FONCTIONS_EXO5__
#define __FONCTIONS_EXO5__

#include "Fonctions_graphes.h"


typedef struct bordure_Zsg{
    int len; /* Longueur de la taille liste chainée de la même couleur */
    Cellule_som *som;  /* Liste chainee des sommets adjacents du graphes de la même couleur */
} Bordure_Zsg;

typedef struct zsg{
    int dim; /*dimension de la grille*/
    int nbcl; /*nombre de couleurs*/

    Cellule_som *cellSom; /* Liste chainee des sommets qui appartiennent a la zone */
    Bordure_Zsg *B; /*Tableau de listes de pointeurs sur sommets-zone*/
    Graphe_zone *G; /* Graphes de sommets de la grille*/
} Zsg;

typedef struct queue{
    Cellule_som *head;
    Cellule_som *last;
}Queue;

typedef struct listeCouleur{
    int cl;
    struct listeCouleur *suiv;
}ListeCouleur;

/* fonction qui initialise les champs de la structure Bordure_Zsg */
void init_Bordure_Zsg(Bordure_Zsg *B);

/* fonction qui initialise les champs de la Zone; en meme temps on cree le graphe de la zone pour initialiser directement Z->G au graphe correspondant a la matrice de jeu */
void init_Zone(Zsg *Z, int **M, int dim, int nbcl);

/* fonction qui detruit l'espace alloue a la liste de Cellule_Som d'une case du tableau des bordures */
void detruit_Bordure_Zsg(Bordure_Zsg *B);

/* fonction qui detruit l'espace alloue a la zone */
void detruit_Zone(Zsg *Z, int dim, int nbcl);

/* fonction qui affiche les cases contenues dans un sommet */
void affiche_ListeCase(ListeCase *L);

/* fonction qui affiche les sommets d'une liste de Cellule_som */
void affiche_Cell(Cellule_som *c);

/* fonction qui verifie si toutes les cases du tableau bordure sont vides */
int Bordure_vide(Bordure_Zsg *B,int nbcl);

/* fonction qui renvoie la couleur la plus representee dans la bordure */
int couleur_max(Bordure_Zsg *B, int nbcl);


/* fonction qui bascule un sommet de la bordure dans la zone */
void agrandit_Zone(Zsg *Z, int cl);

/* fonction qui met la couleur cl sur les cases de la zone */
void colorier(Zsg *Z, int **M, int cl);

/* fonction qui met a jour l'affichage sur la grille */
void afficher_Grille(Zsg *Z, Grille *G, int dim, int cl);

/* fonction qui joue le jeu en implementant la strategie de choisir la couleur la plus representee */
int max_bordure(int **M, Grille *G,int dim, int nbcl,int aff);

void BFS(Graphe_zone *G, Sommet *start, Sommet *finish);

int sequence_jeu(int **M, Grille *G, int dim, int nbcl, int aff);

int ajouteCouleur(ListeCouleur **L, int cl);

void detruit_ListeCouleur(ListeCouleur *L);

void detruit_Queue(Queue *queue);

void init_Queue(Queue **queue);

Sommet *enleve_Queue(Queue **queue);

void liste_Chemin(Cellule_som **listeChemin, Sommet *start, Sommet *finish);


#endif