#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#include "fonctions_test.h"
#include "Generation_matrice.h"

void sequence_graphe(int **M, Grille *G, int dim, int nbcl, int aff, int exo){
    FILE *f;
    int taille_max;
    int pas;
    char *nom_fichier;
    int essais;
    clock_t temps_initial, temps_final;
    float temps_cpu;  
    switch (exo)
    {
    case 1:
        nom_fichier="temps_rec.txt";
        f=fopen(nom_fichier,"w");
        
        for (int i=1; i<=323;i+=10){
            M=generer(dim);
            Gene_instance_genere_matrice(dim, nbcl, 1, 1, M);
            
            temps_initial=clock();
            essais=sequence_aleatoire_rec(M,G,i,nbcl,aff);
            temps_final=clock();
            temps_cpu=(temps_final-temps_initial) * 1e-6;
            fprintf(f,"\t%d\t%f\n",i,temps_cpu);
        }
        break;
    case 2:
        nom_fichier="temps_imp.txt";
        f=fopen(nom_fichier,"w");
        
        for (int i=1; i<=1000;i+=30){
            M=generer(dim);
            Gene_instance_genere_matrice(dim, nbcl, 1, 1, M);
            temps_initial=clock();
            essais=sequence_aleatoire_imp(M,G,i,nbcl,aff);
            temps_final=clock();
            temps_cpu=(temps_final-temps_initial) * 1e-6;
            fprintf(f,"\t%d\t%f\n",i,temps_cpu);
        }
        break;

    case 5:
        nom_fichier="temps_test.txt";
        f=fopen(nom_fichier,"w");
        
        for (int i=1; i<=1000;i+=50){
            for(int k=1;k<=nbcl;k++){
            M=generer(i);
            Gene_instance_genere_matrice(i, k, 1, 1, M);
            temps_initial=clock();
            essais=sequence_jeu(M,G,i,k,0);
            printf("%d\t%d\t\n",i,k);
            for(int j = 0 ; j< i ; j++)
                free(M[j]);
            free(M);
            //fprintf(f,"%d\t%d\t%d\n",i,k,essais);
            }
        }
        break;

    default:
        break;
    }
   
    
    fclose(f);
}