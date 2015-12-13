/*
    TP Optimisation Combinatoire Jordane QUINCY et Jean-Baptiste DURIEZ
    Etat : Exercice 1 et 2 finis, en cours : Exercice 3
    Version : 11/12/2015 (Premi�re s�ance de TP)
*/

#include <stdio.h>
#include <stdlib.h>

//Structure de donn�es pour une instance du sac � dos

typedef struct
{
    int profit;
    int poids;
} Objet;

typedef struct
{
    Objet objet;
    float present; //1 si present dans le sac, 0 sinon
} ObjetSolution;


typedef struct
{
    int nbrObjet;
    int capaciteMax;
    Objet *objets; //Tableau stockant les objets objets[0][0] = le profit de l'object 0 et objets[0][1] = le poids de l'object 0
} InstanceSacADos;

typedef struct
{
    ObjetSolution *objetsSolution;
    int poidsTotalSolution;
} SolutionSacADos;

typedef struct
{
    InstanceSacADos instance;
    SolutionSacADos solution;
} InstanceSacADosSolution;

InstanceSacADosSolution initialisation(int nbrObjet, int capaciteMax,Objet *objets)
{
    InstanceSacADosSolution S;
    int i;
    S.instance.nbrObjet = nbrObjet;
    S.instance.capaciteMax = capaciteMax;
    S.instance.objets = malloc(nbrObjet * (sizeof(Objet)));
    S.solution.poidsTotalSolution = 0;

    //Initialiser objets et solution a vide et tous les objets non pr�sent
    S.solution.objetsSolution = malloc(nbrObjet * (sizeof(ObjetSolution)));
    for (i = 0; i < nbrObjet; i++)
    {
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
    nom : nom du fichier � lire
*/
InstanceSacADosSolution chargementSacADos (char *nom)
{
    int nbObjets, capaciteMax;

    FILE *f = fopen(nom, "r");
    //Lire le nbr d'objets et la capacite maximale du sac
    fscanf(f,"%d %d\n",&nbObjets, &capaciteMax);

    //On va stocker les profits et poids dans un tableau .
    Objet *tabObjet = malloc(nbObjets*sizeof(Objet));

    int n, curProfit, curPoids;
    //On lit la ligne des profits
    for (n = 0; n < nbObjets; n++)
    {
        fscanf(f, "%d", &curProfit);
        tabObjet[n].profit = curProfit;
    }
    //On lit la ligne des poids
    for (n = 0; n < nbObjets; n++)
    {
        fscanf(f, "%d", &curPoids);
        tabObjet[n].poids = curPoids;
    }

    fclose(f);
    return initialisation(nbObjets, capaciteMax, tabObjet);
};

void afficherSacADosComplet(InstanceSacADosSolution S)
{
    InstanceSacADos sac = S.instance;

    int i;
    for(i=0; i < sac.nbrObjet; i++)
    {
        Objet curObjet = sac.objets[i];
        printf("L'objet %d a pour poids %d et pour profit %d \n", i,curObjet.poids,curObjet.profit );
    }
    printf("\n");
};

void ajouterTrie (int *tab, Objet objet, int index, Objet *objets, int taille)
{
    int i = 0;
    double quotient1 = 0;
    double quotient2 = 0;
    int j;
    int tmp;
    int ajoute = 0;
    int ajouteVide = 0;
    while (!ajoute && !ajouteVide && i < taille)
    {
        if (tab[i] == -1)
        {
            tab[i] = index;
            ajouteVide = 1;
        }
        else
        {
            quotient1 = ((double)objets[tab[i]].profit)/((double)objets[tab[i]].poids);
            quotient2 = ((double)objet.profit)/((double)objet.poids);
            if (quotient1 < quotient2)
            {
                ajoute = 1;
                tmp = tab[i];
                tab[i] = index;
            }
        }
        i++;
    }
    if (!ajouteVide)
    {
        for (j = taille - 1; j > i; j--)
        {
            if (tab[j - 1] != -1)
            {
                tab[j] = tab[j - 1];
            }
        }
        tab[i] = tmp;
    }
};

void afficherSolutionGlouton (InstanceSacADosSolution S)
{
    int i;
    float solutionOpti = 0;
    for (i = 0; i < S.instance.nbrObjet; i++)
    {
        if (S.solution.objetsSolution[i].present == 0)
        {
            printf("L'objet numero %d n'est pas ajoute\n", i);
        }
        else
        {
            printf("L'objet numero %d est ajoute ", i);
            if (S.solution.objetsSolution[i].present == 1)
            {
                printf("entierement\n");
            }
            else
            {
                printf("partiellement (a %.2f%%)\n", S.solution.objetsSolution[i].present * 100);
            }
        }
        solutionOpti += (float)(S.solution.objetsSolution[i].present) * S.solution.objetsSolution[i].objet.profit;
    }
    printf("La valeur de la solution optimale avec l'heuristique gloutonne est : %.2f\n", solutionOpti);

};

/*
    solutionUsingAlgoGlouton permet de donner une solution optimale du probl�me de sac � dos en utilisant l'heuristique gloutonne
    S : l'instance du sac � dos
    isSolutionContinue : si 1 alors on donne la solution continu, si 0 on donne la solution r�alisable
*/
void solutionUsingAlgoGlouton (InstanceSacADosSolution *S, int isSolutionContinue)
{
    int i;
    //Cr�ation du tableau d'index dans le bon ordre pour l'algorithme
    int tab[S->instance.nbrObjet];
    //init
    for (i = 0; i < S->instance.nbrObjet; i++)
    {
        tab[i] = -1;
    }
    for (i = 0; i < S->instance.nbrObjet; i++)
    {
        ajouterTrie(&tab, S->instance.objets[i], i, S->instance.objets, S->instance.nbrObjet);
    }
    for (i = 0; i < S->instance.nbrObjet; i++)
    {
        if (S->solution.poidsTotalSolution + S->instance.objets[tab[i]].poids <= S->instance.capaciteMax)
        {
            S->solution.poidsTotalSolution += S->instance.objets[tab[i]].poids;
            S->solution.objetsSolution[tab[i]].present = 1;
        }
        else if (isSolutionContinue)
        {
            S->solution.objetsSolution[tab[i]].present = (float)(S->instance.capaciteMax - S->solution.poidsTotalSolution) / S->instance.objets[tab[i]].poids;
            S->solution.poidsTotalSolution = S->instance.capaciteMax;
        }
    }
    afficherSolutionGlouton(*S);
};

void resetSolution(InstanceSacADosSolution *S)
{
    int i;
    S->solution.poidsTotalSolution = 0;
    for(i=0; i < S->instance.nbrObjet; i++)
    {
        S->solution.objetsSolution[i].present = 0;
    }
};

int main()
{
    char nomFichier[50];
    printf("Quel est le nom du fichier a charger ? (n'oubliez pas le .txt)\n");
    scanf("%s", nomFichier);
    printf("\n");
    InstanceSacADosSolution S;
    S = chargementSacADos(nomFichier);
    int choix;
    //Menu :
    do
    {
        printf("\n\n------------------------------------------\n");
        printf("Que souhaitez-vous faire ? \n");
        printf("1 - Resoudre le probleme du sac a dos en utilisant l'heuristique gloutonne (relaxation continue)\n");
        printf("2 - Resoudre le probleme du sav a dos en utilisant l'heuristique gloutonne (solution realisable)\n");
        printf("0 - Sortir du programme ? \n");
        scanf("%d", &choix);
        printf("\n\n");
        switch (choix)
        {
        case 1:
            printf("Resultat du probeme de sac a dos avec l'heuristique gloutonne en relaxation continue : \n");
            resetSolution(&S);
            solutionUsingAlgoGlouton(&S, 1);
            break;
        case 2:
            printf("Resultat du probleme de sac a dos avec l'istique gloutonne avec une solution realisable :\n");
            resetSolution(&S);
            solutionUsingAlgoGlouton(&S, 0);
            break;
        default:
            printf("Au revoir\n");
            break;
        }
    }
    while (choix != 0);
    return 0;
}
