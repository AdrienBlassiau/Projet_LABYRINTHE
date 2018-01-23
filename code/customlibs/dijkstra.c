#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "dijkstra.h"




/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*---------------Fonctions et structure liées à l'algorithme de Dijkstra utilisées dans l'exercice 2 --------------*/
/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

struct noeuds 
{
    int cout;
    int predecesseur;
    int marque;
    
};

/*  
    @require: les paramètres sont les entrées standards de l'exercice 2 qui doivent vérifier les consignes.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui appelle la fonction de recherche du plus court chemin dans un graphe par dijkstra ; ne retourne rien.
*/

void resolution_dijkstra(int n,int s,int t,int ** matrix)
{
    int * tab_res;
    int temps_parcours = 0;

    tab_res = dijkstra(n,s,t,matrix,&temps_parcours);
    if (tab_res != NULL) /*Si on a trouvé un chemin, on l'affiche*/
        affichage_chemin_exo2(tab_res,temps_parcours);

    free(tab_res);
}

/*  
    @require: les paramètres sont les entrées standards de l'exercice 2 qui doivent vérifier les consignes.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui recherche le plus court chemin dans un graphe avec l'algorithe de dijkstra ; retourne un tableau valant NULL (tab_fin) si aucun chemin trouvé,
    sinon retourne un tableau avec les différents noeuds du chemin trouvé rangés du départ à l'arrivée (tab_fin).
*/
int * dijkstra(int n,int s,int t,int ** matrix,int *temps_parcours)
{
    int u,v;
    int * tab_fin = NULL;

    noeuds * tab_noeuds=(noeuds *)malloc(n*sizeof(noeuds));

    if (tab_noeuds == NULL)
    {
        fprintf(stderr, "Erreur allocation dynamique dans resolution_dijkstra\n");
        exit(EXIT_FAILURE);
    }


    tab_noeuds = initialisation(tab_noeuds,n,s);/*On initilise le tableau des noeuds visités*/

    while(1)
    {
        u=noeuds_cout(tab_noeuds,n);/*On sort le noeud de cout le plus faible*/

        if (u == n+1)
        {
            fprintf(stdout,"Not connected\n");
            free(tab_noeuds);
            return NULL;
        }
        
        tab_noeuds[u].marque=1; 

        if ( u == t)/*Si on a sorti le noeud d'arrivée*/
        {
            tab_fin = construction_chemin_dijkstra(s,t,tab_noeuds,n,temps_parcours);
            return tab_fin;
        }

        for (v = 0; v < n; ++v)/*On cherche tout les successeurs du noeud sorti*/
        {
            if (est_successeur(u,v,matrix))
            {
                tab_noeuds = maj(tab_noeuds,u,v,matrix);/*On met à jour les données*/
            }
        }
    }  
}

/*  
    @require: tab_noeuds non NULL et consignes.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure:  fonction qui affiche le plus court chemin en remontant de predecesseur en predecesseur les différents sommets qui 
    composent le plus court chemin ; retourne un tableau avec les différents sommets du chemin trouvé rangés du départ à l'arrivée (tab_fin).

*/
int * construction_chemin_dijkstra(int s, int t,noeuds * tab_noeuds, int n,int * temps_parcours)
{
    if (tab_noeuds[t].predecesseur >= n)
    {
        fprintf(stdout,"Not connected\n");
        free(tab_noeuds);
        exit(EXIT_SUCCESS);
    }


    int pred=t , j=1 ,k;
    int *tab_res = (int *)calloc(n,sizeof(int));
    int *tab_fin = NULL;

    if (tab_res == NULL)
    {
        fprintf(stderr, "Erreur allocation dynamique dans affichage_chemin_exo2\n");
        free(tab_noeuds);
        exit(EXIT_FAILURE);
    }

    /*On "remonte" de prédecesseur en prédécesseur*/
    tab_res[0]=t+1;
    while(pred !=s)
    {
        pred = tab_noeuds[pred].predecesseur;
        tab_res[j]=pred+1;
        j++;
    }

    tab_fin = (int *)calloc(j,sizeof(int));

    for (k = j-1 ; k >= 0 ; k--)
    {
        tab_fin[j-k-1] = tab_res[k]-1;
    }

    *temps_parcours = j-1;

    free(tab_noeuds);
    free(tab_res);

    return tab_fin;   
}

void affichage_chemin_exo2(int *tab, int n)
{
    int k;

    for (k = 0 ; k <= n ; k++)
    {
        fprintf(stdout,"%d\n",tab[k]+1);
    }
}


/*  
    @require: tab_noeuds non NULL, 0<= u,v <n.
    @assign: modifie tab_noeuds (et des variables locales).
    @ensure: fonction qui modifie ou non le cout au départ pour un successeur v en prenant
    en compte son predecesseur u et qui modifie le predecesseur en mémoire du noeud v si le cout
    est plus courte ; retourne le tableau remplit de ces données modifiées ou non (tab_noeuds).
*/
noeuds * maj(noeuds * tab_noeuds,int u, int v, int ** matrix)
{
    int nouveau = tab_noeuds[u].cout + matrix[u][v];

    if (tab_noeuds[v].cout ==-1 || tab_noeuds[v].cout >= nouveau)/*Si le cout est plus petit que le cout précédent, on maj*/
    {
        tab_noeuds[v].cout = nouveau;
        tab_noeuds[v].predecesseur = u;
    }

    return tab_noeuds;
}

/*  
    @require: tab_noeuds non NULL, 0<= u,v <n.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui indique si un sommet v est successeur d'un sommet u;
    retourne 1 si c'est le cas, 0 sinon.
*/
int est_successeur(int u,int v,int ** matrix)
{
    if (matrix[u][v] != -1)
        return 1;
    return 0;
}

/*  
    @require: tab_noeuds non NULL.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui recherche le noeud minimisant le cout au départ parmi les noeuds
    non visités; retourne le numéro de ce noeud (choix).
*/
int noeuds_cout(noeuds *tab_noeuds, int n)
{
    int min = INT_MAX, i,choix = n+1;
    /*On recherche le mininimum*/
    for (i = 0; i < n; ++i)
    {
        if ((tab_noeuds[i].cout <=min) && (tab_noeuds[i].cout >=0) && (tab_noeuds[i].marque == 0))
        {
            min=tab_noeuds[i].cout;
            choix=i;
        }
    }
    return choix;
}

/*  
    @require: tab_noeuds non NULL et s<n.
    @assign: modifie tab_noeuds (et des variables locales).
    @ensure: fonction qui initialise les couts par rapport au départ des différents sommets du graphe, ainsi que leur prédecesseur 
    permettant d'obtenir ces couts ; retourne un tableau remplit de ces données (tab_noeuds). 
*/
noeuds * initialisation(noeuds *tab_noeuds, int n, int s)
{
    int i;

    for (i = 0; i < n; ++i)
    {
        if ( i == s)
        {
            tab_noeuds[i].cout = 0;
            tab_noeuds[i].predecesseur = 0;
        }

        else
        {
            tab_noeuds[i].cout = INT_MAX;/*On met INT_MAX à défaut de pouvoir de mettre l'infini*/
            tab_noeuds[i].predecesseur = 51;/*Car 50 noeuds max*/
        }

        tab_noeuds[i].marque = 0;
        
    }

    return tab_noeuds;
}

/*--------------------------------Fonction pour le débogage--------------------------------*/

/*  
    @require: tab_noeuds non NULL.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui affiche certaines informations utiles ; ne retourne rien. 
*/
void affichage_tab_noeuds(noeuds *tab_noeuds, int n)
{
    int i;

    for (i = 0; i < n; ++i)
    {
        fprintf(stdout,"Noeuds %d : [cout = %d / predecesseur = %d / marque = %d]\n",i,tab_noeuds[i].cout,tab_noeuds[i].predecesseur,tab_noeuds[i].marque);
    }

}