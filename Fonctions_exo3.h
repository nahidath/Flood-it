#ifndef __Fonctions_exo3__
#define __Fonctions_exo3__

#include "Liste_case.h"
#include "API_Grille.h"


typedef struct {
int dim ; /* dimension de la grille */
int nbcl ; /* nombre de couleurs */
ListeCase Lzsg ; /* Liste des cases de la zone Zsg */
ListeCase *B ; /* Tableau de listes de cases de la bordure*/
int ** App ; /* Tableau a double entree des appartenances */
} S_Zsg ;

/* fonction qui initialise les champs de la structure S_Zsg */
void init_Zsg(S_Zsg *Z, int dim, int nbcl);

/*fonction qui ajoute une case de coordonees i,j a la liste qui gere la zone superieure gauche */
int ajoute_Zsg(S_Zsg *Z, int i, int j);

/*fonction qui ajoute une case de coordonees i,j a la bordure de couleur cl */
int ajoute_Bordure(S_Zsg *Z, int i, int j, int cl);

/*fonction qui verifie l'appartenance de la case  i,j a la zone superieure gauche */
int appartient_Zsg(S_Zsg *Z, int i, int j);

/*fonction qui verifie l'appartenance de de la case i,j a la bordure de couleur cl */
int appartient_Bordure(S_Zsg *Z, int i, int j, int cl);

/* fonction qui met a jour les champs qui gerent la bordure et la zone superieure gauche */
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int i, int j);

/* fonction qui utilise la fonction agradit_Zsg pour jouer le jeu avec un tirage aleatoire des couleurs */
int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff);

/* fonction qui elibere la memoire allouee par la structure S_Zsg */
void detruit_Zsg(S_Zsg *Z);
#endif