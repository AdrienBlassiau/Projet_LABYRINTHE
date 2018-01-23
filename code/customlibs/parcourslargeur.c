#include <stdio.h>
#include <stdlib.h>
#include "parcourslargeur.h"



/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*---------------Fonctions et structure liées au parcours en largeur utilisées dans l'exercice 1 et 3--------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/


struct file  /*Structure de maillon de file*/
{
    int valeur;
    pt_file suite;

};


/*  
    @require: les paramètres sont les entrées standards de l'exercice 1 qui doivent vérifier les consignes.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui appelle la fonction de recherche du plus court chemin dans un graphe par parcours en largeur ; ne retourne rien.
*/
void resolution_parcours_largeur(int** m, int taille_graphe, int s, int t)
{
    int * tab_res;
    int temps_parcours = 0;

    tab_res = parcours_largeur(m,taille_graphe,s,t,&temps_parcours);
    if (tab_res != NULL) /*Si on a trouvé un chemin, on l'affiche*/
        affichage_chemin_exo1(tab_res,temps_parcours);

    free(tab_res);
}


/*  
    @require: tab non NULL et n >= 0 .
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui affiche dans la sortie standard le chemin de taille minimum 
    entre l'entrée et la sortie si celui ci existe (comme précisé dans la consigne) ; ne retourne rien.
*/
void affichage_chemin_exo1(int * tab, int n)
{
	int i;

	for ( i = 0 ; i <= n ; i ++)
	{
		fprintf(stdout,"%d\n",tab[i]+1);
	}
}


/* 
    @require: taille doit être positive ou nulle.
    @assign: modifie tab.
    @ensure: fonction d'allocation dynamique de tableau d'entier de taille donnée ; retourne un tableau dynamique de taille donnée (tab).
*/
int * allocation_dynamique_tableau(int* tab, int taille)
{
	tab = (int*)calloc(taille,sizeof(int));

	if ( tab == NULL)
	{
		fprintf(stderr, "Erreur allocation dynamique dans allocation_dynamique_tableau\n");
		exit(EXIT_FAILURE);
	}

	return tab;
}


/*  
    @require: les paramètres sont les entrées standards de l'exercice 1.
    qui doivent vérifier les consignes sauf temps_parcours qui pointe vers un variable valant 0 (pas necessaire).
    @assign: modifie temps_parcours (et des variables locales).
    @ensure: fonction qui effectue le parcours en largeur d'un graphe donné ; retourne un tableau valant NULL (tab_fin) si aucun chemin trouvé,
    sinon retourne un tableau avec les différents noeuds du chemin trouvé rangés du départ à l'arrivée (tab_fin).
*/
int *parcours_largeur(int** m, int taille_graphe, int s, int t, int *temps_parcours)
{
    pt_file tete = NULL,queue = NULL;
    int i;
    int * tab_fin = NULL;
    int * tab_pred = NULL;
    
    tab_pred = allocation_dynamique_tableau(tab_pred,taille_graphe);
    enfiler(s,&tete,&queue);/*On enfile le premier sommet*/
    tab_pred[s] = -1;/*Il n'a pas de predecesseur*/

    while (!(tete == NULL && queue == NULL) && tab_pred[t] == 0) 
    {
        s = defiler(&tete,&queue);/*On défile le sommet n tête de file*/
        for (i = 0; i < taille_graphe; ++i)
        {
        	if ( m[s][i] == 1 && tab_pred[i] == 0)
   			{	
   				enfiler(i,&tete,&queue);/*On enfile tout les successeurs*/
   			   	tab_pred[i] = s+1;/*On maj leur predecesseur*/
   			}           
        }
    }

    if (tete == NULL && queue == NULL)/*Si la file est vide*/
    {
        fprintf(stdout,"Not connected\n");
        tab_fin = NULL;   
    }

    else/*Sinon*/
    {
        tab_fin = construction_chemin(tab_pred,taille_graphe,t,temps_parcours);
    }

    nettoyer(&tete,&queue);
    free(tab_pred);

    return tab_fin;
}


/*  
    @require: tab non NULL.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui retrouve le plus court chemin en remontant de predecesseur en predecesseur les différents sommets qui 
    composent le plus court chemin ; retourne un tableau avec les différents sommets du chemin trouvé rangés du départ à l'arrivée (tab_fin).
*/
int * construction_chemin(int *tab,int n, int t,int * temps_parcours)
{
    int i = t,j=0,k;
    int *tabRes = NULL, *tab_fin = NULL;

    tabRes = allocation_dynamique_tableau(tabRes,n);
    while (i != -2) /*On remonte les prédécesseurs*/
    {
        tabRes[j]=i+1;
        i=tab[i]-1;
        j++;
    }

    tab_fin = allocation_dynamique_tableau(tabRes,j);
    for (k = j-1 ; k >= 0 ; k--)/*On inverse le tableau*/
    {
        tab_fin[j-k-1] = tabRes[k]-1;
    }

    *temps_parcours = j-1;

    free(tabRes);

    return tab_fin;
}


/*  
    @require: rien.
    @assign: modifie la tete et/ou la queue de la file.
    @ensure: fonction qui enfile un élément file_element en queue de file par effet ; ne retourne rien.
*/
void enfiler(int v,pt_file *tete,pt_file *queue)
{
    pt_file file_element = (pt_file)malloc(sizeof(pt_file*));

    if ( file_element == NULL)
    {
        fprintf(stderr, "Erreur allocation dynamique dans enfiler\n");
        exit(EXIT_FAILURE);
    }

    file_element->suite = NULL;
    file_element->valeur = v;

    
    if (*tete == NULL && *queue == NULL)
    {
        *tete = file_element;
        *queue = file_element;

    }
    
    else
    {
        (*queue)->suite = file_element;
        (*queue) = file_element;
    }
}


/*  
    @require: *tete non NULL.
    @assign: modifie la tete et/ou la queue de la file.
    @ensure: fonction qui defile un élément file_element en tete de file par effet ; retourne la valeur de l'élément 
    de file défilé, c'est à dire le sommet du graphe défilé (sortie).
*/
int defiler(pt_file *tete,pt_file *queue)
{

    pt_file file_element = *tete;

    int sortie = file_element->valeur;

    if (*tete == *queue)
    {
        *tete = NULL;
        *queue = NULL;
    }
    else
    {
        *tete = (*tete)->suite;
    }

    file_element->suite = NULL;
    free(file_element);

    return sortie;
}


/* 
    @require: aucun.
    @assign: modifie la tete et la queue de la file. 
    @ensure: fonction qui libère la mémoire allouée dans la constrution de la file ; ne retourne rien.
*/
void nettoyer(pt_file *tete, pt_file *queue)
{   
    while (!(*tete == NULL && *queue == NULL))
    {
        pt_file file_element = *tete;

        if (*tete == *queue)
        {
            *tete = NULL;
            *queue = NULL;
        }
        else
        {
            *tete = (*tete)->suite;
        }

        file_element->suite = NULL;
        free(file_element);      
    }
}

