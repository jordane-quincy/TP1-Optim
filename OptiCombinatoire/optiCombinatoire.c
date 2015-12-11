/*
    TP Optimisation Combinatoire Jordane QUINCY et Jean-Baptiste DURIEZ
    Etat : Exercice 1 fini, en cours : Exercice 2
    Version : 11/12/2015 (Première séance de TP)
*/

#include <stdio.h>
#include <stdlib.h>

//Structure de données pour une instance du sac à dos

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
    ObjetSolution *objetsSolution;
    int poidsTotalSolution;
}SolutionSacADos;

typedef struct {
    InstanceSacADos instance;
    SolutionSacADos solution;
}InstanceSacADosSolution;

InstanceSacADosSolution initialisation(int nbrObjet, int capaciteMax,Objet *objets) {
    InstanceSacADosSolution S;
    int i;
    S.instance.nbrObjet = nbrObjet;
    S.instance.capaciteMax = capaciteMax;
    S.instance.objets = malloc(nbrObjet * (sizeof(Objet)));
    S.solution.poidsTotalSolution = 0;

    //Initialiser objets et solution a vide et tous les objets non présent
    S.solution.objetsSolution = malloc(nbrObjet * (sizeof(ObjetSolution)));
    for (i = 0; i < nbrObjet; i++) {
        S.instance.objets[i].profit = objets[i].profit;
        S.instance.objets[i].poids = objets[i].poids;
        S.solution.objetsSolution[i].objet.profit = objets[i].profit;
        S.solution.objetsSolution[i].objet.poids =objets[i].poids;
        S.solution.objetsSolution[i].present = 0;
    }
    return S;
};

/*
    chargementSacADos : permet de charger les donnees dans une InstanceSacADosSolution depuis le fichier ayant comme chemin d'acces la variable nom
    nom : nom du fichier à lire
*/
InstanceSacADosSolution chargementSacADos (char *nom) {
    int nbObjets, capaciteMax;
    FILE *f = fopen(nom, "r");
    //Lire le nbr d'objets et la capacite maximale du sac
    fscanf(f,"%d %d\n",&nbObjets, &capaciteMax);

    //On va stocker les profits et poids dans un tableau .
    Objet *tabObjet = malloc(nbObjets*sizeof(Objet));

    int n, curProfit, curPoids;
    //On lit la ligne des profits
    for (n = 0; n < nbObjets; n++) {
        fscanf(f, "%d", &curProfit);
        tabObjet[n].profit = curProfit;
    }
    //On lit la ligne des poids
    for (n = 0; n < nbObjets; n++) {
        fscanf(f, "%d", &curPoids);
        tabObjet[n].poids = curPoids;
    }

    fclose(f);
    return initialisation(nbObjets, capaciteMax, tabObjet);
};

void afficherSacADosComplet(InstanceSacADosSolution S) {

};

void ajouterTrie (int *tab, Objet objet, int index, Objet *objets, int taille) {
    int i = 0;
    int tmp;
    int ajoute = 0;
    while (!ajoute && i < taille) {
        if (tab[i] == -1) {
            tab[i] = index;
        }
        else {
            if (objets[tab[i]].profit / objets[tab[i]].poids > objet.profit / objet.poids) {
                ajoute = 1;
                tmp = tab[i];
                tab[i] = index;
            }
        }
        i++;
    }
    int j;
    for (j = )

};

void triAlgoGlouton (InstanceSacADosSolution *S) {
    int i;
    //Création du tableau d'index dans le bon ordre pour l'algorithme
    int tab[S->instance.nbrObjet];
    //init
    for (i = 0; i < S->instance.nbrObjet; i++) {
        tab[i] = -1;
    }
    for (i = 0; i < S->instance.nbrObjet; i++) {
        ajouterTrie(&tab, S->instance.objets[i], i, S->instance.objets, S->instance.nbrObjet);
    }

};

int main()
{
    char nomFichier[50];

    printf("Hello world!\n");
    scanf("%s", nomFichier);
    printf("\n");
    InstanceSacADosSolution S;
    S = chargementSacADos(nomFichier);

    return 0;
}
