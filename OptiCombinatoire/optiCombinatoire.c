#include <stdio.h>
#include <stdlib.h>

//Structure de donn�es pour une instance du sac � dos

typedef struct {
    int profit;
    int poids;
}Objet;

typedef struct {
    Objet objet;
    int present; //1 si present dans le sac, 0 sinon
}ObjetSolution;


typedef struct {
    int nbrObjet;
    int capaciteMax;
    Objet *objets; //Tableau stockant les objets objets[0][0] = le profit de l'object 0 et objets[0][1] = le poids de l'object 0
}InstanceSacADos;

typedef struct {
    ObjetSolution *solution;
    InstanceSacADos instance;
}InstanceSacADosSolution;

InstanceSacADosSolution initialisation(int nbrObjet, int capaciteMax,Objet *objets) {
    InstanceSacADosSolution S;
    int i;
    S.instance.nbrObjet = nbrObjet;
    S.instance.capaciteMax = capaciteMax;
    S.instance.objets = malloc(nbrObjet * (sizeof(Objet)));

    //Initialiser objets et solution a vide et tous les objets non pr�sent
    S.solution = malloc(nbrObjet * (sizeof(ObjetSolution)));
    for (i = 0; i < nbrObjet; i++) {
        S.instance.objets[i].profit = objets[i].profit;
        S.instance.objets[i].poids = objets[i].poids;
        S.solution[i].objet.profit = objets[i].profit;
        S.solution[i].objet.poids =objets[i].poids;
        S.solution[i].present = 0;
    }
    return S;
};

/*
    chargementGraphe : permet de charger le graphe depuis le fichier ayant comme nom la variable nom
    nom : nom du fichier � lire
*/
void chargementGraphe (char *nom) {
    int nbObjets, capaciteMax;
    FILE *f = fopen(nom, "r");
    //Lire le nbr d'objets et la capacite maximale du sac
    fscanf(f,"%d %d\n",&nbObjets, &capaciteMax);

    //On va stocker les profits et poids dans un tableau .
    Objet *matrice = malloc(nbObjets*sizeof(Objet));

    int n, curProfit, curPoids;
    //On lit la ligne des profits
    for (n = 0; n < nbObjets; n++) {
        fscanf(f, "%d", &curProfit);
        matrice[n].profit = curProfit;
    }
    //On lit la ligne des poids
    for (n = 0; n < nbObjets; n++) {
        fscanf(f, "%d", &curPoids);
        matrice[n].poids = curPoids;
    }

    fclose(f);
    InstanceSacADosSolution S = initialisation(nbObjets, capaciteMax, matrice);
};

int main()
{
    char nomFichier[50];

    printf("Hello world!\n");
    scanf("%s", nomFichier);
    printf("\n");
    chargementGraphe(nomFichier);

    return 0;
}
