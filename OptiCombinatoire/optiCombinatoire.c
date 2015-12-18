/*
    TP Optimisation Combinatoire Jordane QUINCY et Jean-Baptiste DURIEZ
    Etat : Exercice 1 et 2  et 3finis, en cours : Exercice 4 (l'arbre est Construit)
    Version : 18/12/2015 (Deuxieme séance de TP)
*/

#include <stdio.h>
#include <stdlib.h>

//Structure de données pour une instance du sac à dos

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

typedef struct n
{
    int *objets;//indice de l'objet
    float borneSup;
    float borneMoins;//en théorie la borne moins sera toujours un int
    struct n *fg;
    struct n *fd;
}Noeud;

typedef Noeud *Arbre;

int maxBorneMoinsDansArbre; //pour la gestion du branch and bound
int *tabSolution; //pour la gestion du branch and bound

InstanceSacADosSolution initialisation(int nbrObjet, int capaciteMax, Objet *objets)
{
    InstanceSacADosSolution S;
    int i;
    S.instance.nbrObjet = nbrObjet;
    S.instance.capaciteMax = capaciteMax;
    S.instance.objets = malloc(nbrObjet * (sizeof(Objet)));
    S.solution.poidsTotalSolution = 0;

    //Initialiser objets et solution a vide et tous les objets non présent
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
    nom : nom du fichier à lire
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

float afficherSolutionGlouton (InstanceSacADosSolution S)
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
    return solutionOpti;

};

/*
    solutionUsingAlgoGlouton permet de donner une solution optimale du problème de sac à dos en utilisant l'heuristique gloutonne
    S : l'instance du sac à dos
    isSolutionContinue : si 1 alors on donne la solution continu, si 0 on donne la solution réalisable
*/
float solutionUsingAlgoGlouton (InstanceSacADosSolution *S, int isSolutionContinue)
{
    int i;
    //Création du tableau d'index dans le bon ordre pour l'algorithme
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
    return afficherSolutionGlouton(*S);
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

float max(float a, float b) {
    if (a >= b) {
        return a;
    }
    return b;
};

void solutionUsingDynamicProgrammation (InstanceSacADosSolution *S) {
    //Initialisation de la matrice
    //les colonnes correspondent aux nombres d'objets à mettre
    //les lignes la capacité du sac
    float tabDynamicProgra[S->instance.capaciteMax + 1][S->instance.nbrObjet + 1];
    int i, j;
    for (i = 0; i <= S->instance.capaciteMax; i++) {
        tabDynamicProgra[i][0] = 0;
    }
    //Construction de la matrice
    for (i = 0; i <= S->instance.capaciteMax; i++) {
        for (j = 1; j <= S->instance.nbrObjet; j++) {
            if (S->instance.objets[j - 1].poids > i) {
                tabDynamicProgra[i][j] = tabDynamicProgra[i][j-1];
            }
            else {
                tabDynamicProgra[i][j] = max(tabDynamicProgra[i][j-1], (float)S->instance.objets[j - 1].profit + tabDynamicProgra[i - S->instance.objets[j - 1].poids][j - 1]);
            }
        }
    }
    //Récupération des résultats
    i = S->instance.capaciteMax;
    j = S->instance.nbrObjet;
    while (tabDynamicProgra[i][j] == tabDynamicProgra[i - 1][j]) {
        i--;
    }
    while (i > 0) {
        while (j > 0 && tabDynamicProgra[i][j] == tabDynamicProgra[i][j - 1]) {
            j--;
        }
        if (j != 0) {
            i -= S->instance.objets[j - 1].poids;
            if (j > 0) {
                S->solution.objetsSolution[j - 1].present = 1;
            }
            j--;
        }
    }
    //Affichage des résultats
    for (i = 0; i < S->instance.nbrObjet; i++) {
        if (S->solution.objetsSolution[i].present == 0)
        {
            printf("L'objet numero %d n'est pas ajoute\n", i);
        }
        else
        {
            printf("L'objet numero %d est ajoute ", i);
            if (S->solution.objetsSolution[i].present == 1)
            {
                printf("entierement\n");
            }
            else
            {
                printf("partiellement (a %.2f%%)\n", S->solution.objetsSolution[i].present * 100);
            }
        }
    }
    printf("La valeur de la solution optimale avec l'algo de programmation dynamique est : %.2f\n", tabDynamicProgra[S->instance.capaciteMax][S->instance.nbrObjet]);

};




void ajouterObjet(int *objets, int idOfObjet) {
    int i = 0;
    printf("Dans le while\n");
    while (objets[i] >= 0) {

        printf("valeur objets[%d] : %d\n", i, objets[i]);
        i++;
    }
    objets[i] = idOfObjet;
    printf("Apres le while\n");
    for (i = 0; i < 3; i++) {
        printf("valeur objets[%d] : %d\n", i, objets[i]);
    }
};


InstanceSacADosSolution modifierInstance(InstanceSacADosSolution S, int idObjet, int present) {
    //Creation du nouveau Pb
    //Modification de la capacite
    int capaciteMax = S.instance.capaciteMax;
    if (present) {
        capaciteMax = capaciteMax - S.instance.objets[idObjet].poids;
    }
    //Modification de la liste d'objt
    Objet *newTab;
    newTab = malloc((S.instance.nbrObjet - 1) * sizeof(Objet));
    int i;
    for (i = 0; i < S.instance.nbrObjet - 1; i++) {
        newTab[i].poids = S.instance.objets[i+1].poids;
        newTab[i].profit = S.instance.objets[i+1].profit;
    }
    printf("Changement d'instance !\n");
    for (i = 0 ; i < S.instance.nbrObjet -1 ; i++) {
        printf("objet %d, poids: %d, profit : %d\n", i, newTab[i].poids, newTab[i].profit);
    }
    return initialisation(S.instance.nbrObjet - 1, capaciteMax, newTab);
};

Arbre creerArbre(int present, int itteration , int *ancienObjets, InstanceSacADosSolution S, InstanceSacADosSolution SdeBase) {
    int i;
    Arbre A;
    A = malloc(sizeof(Noeud));
    A->objets = malloc(SdeBase.instance.nbrObjet * sizeof(int));
    InstanceSacADosSolution newS;
    for (i = 0; i <= SdeBase.instance.nbrObjet; i++) {
        A->objets[i] = ancienObjets[i];
    }
    if (present == 1) {
        ajouterObjet(A->objets, itteration);
        newS = modifierInstance(S, itteration, 1);
    }
    else {
       newS = modifierInstance(S, itteration, 0);
    }

    float profitPrecedent = 0;
    for (i = 0; i < SdeBase.instance.nbrObjet; i++) {
        if (A->objets[i] >= 0) {
            profitPrecedent = profitPrecedent + SdeBase.instance.objets[A->objets[i]].profit;
        }
    }
    printf("ProfitPrecedent : %f\n", profitPrecedent);

    A->borneMoins = profitPrecedent + solutionUsingAlgoGlouton(&newS, 0);
    resetSolution(&newS);
    A->borneSup = profitPrecedent + solutionUsingAlgoGlouton(&newS, 1);
    resetSolution(&newS);
    /*if (A->borneMoins == A->borneSup) {
        A->fg = NULL;
        A->fd = NULL;
    }*/
    if (A->borneMoins - profitPrecedent == 0 ) {
        A->fg = NULL;
        A->fd = NULL;
        if (maxBorneMoinsDansArbre <= A->borneMoins) {
            maxBorneMoinsDansArbre = A->borneMoins;
            tabSolution = A->objets;
        }
    }
    else if (maxBorneMoinsDansArbre > A->borneSup) {
        A->fg = NULL;
        A->fd = NULL;
    }
    else{
        if (maxBorneMoinsDansArbre <= A->borneMoins) {
            maxBorneMoinsDansArbre = A->borneMoins;
            tabSolution = A->objets;
        }
        A->fg = creerArbre(0, itteration + 1, A->objets, newS, SdeBase);
        A->fd = creerArbre(1, itteration + 1, A->objets, newS, SdeBase);
    }
    return A;
}

Arbre construireArbre(InstanceSacADosSolution S) {
    Arbre A;
    int i;
    A = malloc(sizeof(Noeud));
    A->objets = malloc(S.instance.nbrObjet * sizeof(int));
    for (i = 0; i < S.instance.nbrObjet; i++) {
        A->objets[i] = -1;
    }
    A->borneMoins = solutionUsingAlgoGlouton(&S, 0);
    resetSolution(&S);
    A->borneSup = solutionUsingAlgoGlouton(&S, 1);
    resetSolution(&S);
    printf("Borne moins : %f\nBorne plus : %f\n", A->borneMoins, A->borneSup);
    maxBorneMoinsDansArbre = A->borneMoins;
    tabSolution = malloc(S.instance.nbrObjet * sizeof(int));
    tabSolution = A->objets;
    A->fg = creerArbre(0, 0, A->objets, S, S);
    A->fd = creerArbre(1, 0, A->objets, S, S);
    return A;
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
        printf("2 - Resoudre le probleme du sac a dos en utilisant l'heuristique gloutonne (solution realisable)\n");
        printf("3 - Resoudre le probleme du sac a dos en utilisant un algorithme de programmation dynamique\n");
        printf("4 - Resoudre le probleme du sac a dos en utilisant un algorithme Branch and Bound\n");
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
            printf("Resultat du probleme de sac a dos avec l'heuristique gloutonne avec une solution realisable :\n");
            resetSolution(&S);
            solutionUsingAlgoGlouton(&S, 0);
            break;
        case 3:
            printf("Resultat du probleme de sac a dos avec un algorithme de programmation dynamique :\n");
            resetSolution(&S);
            solutionUsingDynamicProgrammation(&S);
            break;
        case 4:
            resetSolution(&S);
            Arbre A = construireArbre(S);
            //On arrive à construire le bon arbre mais il faut recup la liste des objets
            break;
        default:
            printf("Au revoir\n");
            break;
        }
    }
    while (choix != 0);
    return 0;
}
