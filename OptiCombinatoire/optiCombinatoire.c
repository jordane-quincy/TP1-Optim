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
    ObjetSolution *solution;
    InstanceSacADos instance;
}InstanceSacADosSolution;

InstanceSacADosSolution function(int nbrObjet, int capaciteMax,Objet *objets) {
    InstanceSacADosSolution S;
    int i;
    S.instance.nbrObjet = nbrObjet;
    S.instance.capaciteMax = capaciteMax;
    S.instance.objets = malloc(nbrObjet * (sizeof(Objet)));

    //Initialiser objets et solution a vide et tous les objets non présent
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

int main()
{
    printf("Hello world!\n");
    return 0;
}
