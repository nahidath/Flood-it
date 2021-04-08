GCC_FLAGS = -g -Wall

all: Flood-It_Partie1 Flood-It_Partie2

API_Gene_instance.o: API_Gene_instance.c API_Gene_instance.h
	gcc $(GCC_FLAGS) -c API_Gene_instance.c

API_Grille.o: API_Grille.c API_Grille.h
	gcc $(GCC_FLAGS) -c API_Grille.c

Liste_case.o: Liste_case.c Liste_case.h
	gcc $(GCC_FLAGS) -c Liste_case.c

Fonctions_exo1.o: Fonctions_exo1.c Entete_Fonctions.h Liste_case.h
	gcc $(GCC_FLAGS) -c Fonctions_exo1.c

Fonctions_exo3.o: Fonctions_exo3.c Fonctions_exo3.h Liste_case.h
	gcc $(GCC_FLAGS) -c Fonctions_exo3.c

Fonctions_graphes.o: Fonctions_graphes.c Fonctions_graphes.h Liste_case.h Entete_Fonctions.h
	gcc $(GCC_FLAGS) -c Fonctions_graphes.c

Fonctions_exo5.o: Fonctions_exo5.c Fonctions_exo5.h Fonctions_graphes.h
	gcc $(GCC_FLAGS) -c Fonctions_exo5.c

Fonctions_exo6.o: Fonctions_exo6.c Fonctions_exo5.h Fonctions_graphes.h
	gcc $(GCC_FLAGS) -c Fonctions_exo6.c

Fonctions_bonus.o: Fonctions_bonus.c Fonctions_bonus.h
	gcc $(GCC_FLAGS) -c Fonctions_bonus.c


fonctions_test.o: fonctions_test.c fonctions_test.h Fonctions_exo3.h Entete_Fonctions.h Fonctions_graphes.h
	gcc $(GCC_FLAGS) -c fonctions_test.c

Generation_matrice.o: Generation_matrice.c Generation_matrice.h
	gcc $(GCC_FLAGS) -c Generation_matrice.c

Flood-It_Partie1.o: Flood-It_Partie1.c
	gcc $(GCC_FLAGS) -c Flood-It_Partie1.c 

Flood-It_Partie2.o: Flood-It_Partie2.c
	gcc $(GCC_FLAGS) -c Flood-It_Partie2.c 

Flood-It_Partie1: Flood-It_Partie1.o Liste_case.o  API_Grille.o API_Gene_instance.o Fonctions_exo1.o Fonctions_exo3.o  Generation_matrice.o
	gcc $(GCC_FLAGS) -o Flood-It_Partie1 Flood-It_Partie1.o Liste_case.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o Fonctions_exo3.o Generation_matrice.o -lSDL


Flood-It_Partie2: Flood-It_Partie2.o Liste_case.o  API_Grille.o API_Gene_instance.o Fonctions_graphes.o Fonctions_exo1.o Fonctions_exo5.o Fonctions_exo6.o Fonctions_bonus.o
	gcc $(GCC_FLAGS) -o Flood-It_Partie2 Flood-It_Partie2.o Liste_case.o API_Grille.o API_Gene_instance.o Fonctions_graphes.o Fonctions_exo1.o Fonctions_exo5.o Fonctions_exo6.o Fonctions_bonus.o -lSDL

clean:
	rm -f *.o Flood-It_Partie1 Flood-It_Partie2
