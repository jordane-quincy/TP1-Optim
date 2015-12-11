#include <stdio.h>
#include <stdlib.h>

int main()
{
    char nomFichier[50];

    printf("Hello world!\n");
    scanf("%s", nomFichier);
    printf("\n");
    chargementGraphe(nomFichier);

    return 0;
}

/*
    chargementGraphe : permet de charger le graphe depuis le fichier ayant comme nom la variable nom
    nom : nom du fichier à lire
*/
void chargementGraphe (char *nom) {
    int nbObjets, capaciteMax;
    FILE *f = fopen(nom, "r");
    //Lire le nbr d'objets et la capacite maximale du sac
    fscanf(f,"%d %d\n",&nbObjets, &capaciteMax);

    //On va stocker les profits et poids dans un tableau .
    int *matrice = malloc(nbObjets*sizeof(Objet));

    int n, curProfit, curPoids;
    //On lit la ligne des profits
    for (n = 1; n <= nbObjets; n++) {
        fscanf(f, "%d", &curProfit);
        matrice[n]->profit = curProfit;
    }
    //On lit la ligne des poids
    for (n = 1; n <= nbObjets; n++) {
        fscanf(f, "%d", &curPoids);
        matrice[n]->poids = curPoids;
    }

    fclose(f);
    return G;
}
