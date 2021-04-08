#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Fonctions_exo5.h"


/*fonction qui initialise la structure qui gere la file */
void init_Queue(Queue **queue){
    (*queue)->head=NULL;
    (*queue)->last=NULL;
}
/*fonction qui elibere l'espace memoire allouee par la file */
void detruit_Queue(Queue *queue){
    Cellule_som *tmp;
    while(queue->head){
        tmp=queue->head;
        queue->head=queue->head->suiv;
        free(tmp);
    }
    free(queue);
}

/*fonction qui ajoute a la fin de la file */
int ajoute_Queue(Queue **queue, Sommet *s){
    /* on cree la nouvelle cellule qu'on veut inserer */
    Cellule_som *new=(Cellule_som*) malloc(sizeof(Cellule_som));
    new->sommet=s;
    new->suiv=NULL;
    /* si la file est vide les deux pointeurs doivent pointer vers la meme nouvelle cellule */
    if((*queue)->head==NULL){
        (*queue)->head=new;
        (*queue)->last=new;
        return 1;
    }    
    /* on insere la nouvelle cellule a la fin*/
    (*queue)->last->suiv=new;
    /* la nouvelle fin sera la cellule qu'on vient d'inserer */
    (*queue)->last=new;
    return 1;
}

/* fonction qui enleve et renvoie la premiere cellule de la file */
Sommet *enleve_Queue(Queue **queue){
    /*si la file est vide il y a rien a envoyer */
    if(*queue==NULL || (*queue)->head==NULL)
        return NULL;
    
    /*on garde le premier sommet de la file */
    Sommet *s=(*queue)->head->sommet;
    /*on enleve le sommet de la file*/
    Cellule_som *q=(*queue)->head;
    (*queue)->head=q->suiv;
    free(q);
    return s;
}

/* fonction qui calcule le plus court chemin du sommet start vers le sommet finish */
void BFS (Graphe_zone *G,Sommet *start, Sommet *finish){
    /*on initialise la distance du sommet de depart a 0 */
    start->distance=0;
    /* on alloue l'espace memoire pour la file ou on va inserer les suivants sommets a traiter */
    Queue *queue=(Queue*) malloc(sizeof(Queue));
    init_Queue(&queue);
    /*on ajoute le premier sommet a la file */
    ajoute_Queue(&queue,start);
    Sommet *s, *u;
    Cellule_som *voisins;
    /* on parcourt la file  first in first out*/
    while(queue->head!=NULL){
        s=enleve_Queue(&queue);
        voisins=s->sommet_adj;
        /* on regarde les sommets adjacents au sommet currant */
        while(voisins){
            u=voisins->sommet;
            /* on verifie si on n'a pas deja parcouru le sommet aka si la distance n'est pas calcule */
            if(u->distance==INF){
                /*si c'est le cas, on ajoute 1 a la distance entre le sommet courrant et le start */
                u->distance=s->distance + 1;
                /* on indique aussi quel etait le pere du sommet pour pouvoir recuperer le chemin */
                u->pere=s;
                /*on ajoute le sommet voisin a la file pour le traiter */
                ajoute_Queue(&queue,u);
            }
            /*si le sommet qu'on vient de regarder est le sommet cible, on peut arreter la fonction et detruire la file */
            if(u==finish){
                detruit_Queue(queue);
                return;
            }
            /* sinon on continue a parcourir les sommes voisins */
            voisins=voisins->suiv;
        }
    }
}

/* fonction qui construit le chemin entre les sommets start et finish */
void liste_Chemin(Cellule_som **listeChemin, Sommet *start, Sommet *finish){
    Sommet *pere=finish;

    while(pere!=start){
        ajoute_liste_sommet(pere,listeChemin);
        pere=pere->pere;
    }
}


/* fonction qui joue le jeu */
int sequence_jeu(int **M, Grille *G, int dim, int nbcl, int aff){
    /* on initialise la structure qui gere le jeu */
    Zsg *Z = (Zsg *) malloc(sizeof(Zsg));
    init_Zone(Z, M, dim, nbcl); 
    Graphe_zone *graphe=Z->G;
    int essais=0;
    int cl;

    /* la strategie choisie est de trouver le plus court chemin entre les sommets les plus eloignes (la diagonale)*/
    /* ainsi, la bordure sera plus grande et donc, on pourra ajouter plusieurs cases a la Zsg avec un seul changement de couleur */ 
    Sommet *start=(Z->G)->mat[0][0];
    Sommet *finish=(Z->G)->mat[dim-1][dim-1];
    /* on ajoute a la zone le sommet correspondant a la case 0,0 */
    ajoute_liste_sommet(start,&((Z->B[start->cl]).som));
    Z->B[start->cl].len+= start->nbcases_som;
    agrandit_Zone(Z,start->cl);

    /* on essaie de trouver le plus court chemin entre le sommet 0, 0 et le sommet qui contient la case de dim-1 dim-1 */
    BFS(graphe,(Z->G)->mat[0][0],(Z->G)->mat[dim-1][dim-1]);
    Cellule_som *cell=NULL;
    /* on garde dans la liste cell le chemin qu'il faut parcourir pour ajouter le sommet finish a la zone */
    liste_Chemin(&cell, start, finish);
   
    /* on parcourt les sommets du chemin pour faire LA DIAGONALE */
    Cellule_som *listeChemin=cell;
    while(listeChemin){
        /*on regarde quelle est la couleur suivante qu'il faut choisir pour arriver a finish */
        int couleur=listeChemin->sommet->cl;
        /* on applique la fonction qui mets a jour la bordure et la Zsg en fonction de la couleur */
        agrandit_Zone(Z,couleur);
        essais++;
        /* on affiche la grille */
        if(aff==1){
            afficher_Grille(Z,G,dim,couleur);
            colorier(Z,M,couleur);
        }
        /* on passe au sommet suivant du chemin */
        listeChemin=listeChemin->suiv;

    }

    /* on a reussi integrer la diagonale a la Zsg, maintenant on applique la strategie de choisir la couleur la plus representee dans la bordure */
    while(!Bordure_vide(Z->B,nbcl)){
        cl=couleur_max(Z->B,nbcl);
        agrandit_Zone(Z,cl);
       
        essais++;
        if(aff==1){
             colorier(Z,M,cl);
            afficher_Grille(Z,G,dim,cl);
        }
        
    }
    /* on elibere l'espace memoire allouee pour la zone qui gere le jeu et pour la liste de chemin */
    detruit_liste_sommet(&cell);
    detruit_Zone(Z,dim,nbcl);
    
    return essais;
 }