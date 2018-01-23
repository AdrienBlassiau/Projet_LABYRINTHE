#include <stdio.h>
#include <stdlib.h>

#include "parcourslargeur.h"
#include "aetoile.h"
#include "labyrinthe.h"

/*--------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------*/
/*---------------Fonctions et structures liées à l'algorithme A* utilisées dans l'exercice 3--------------*/
/*--------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------*/


struct info 
{
    int valeur;
    int cout;
    int heuri;
    int cout_heuri;

};

struct file_p 
{
    int taille;
    pt_info tas;
    
};



/*  
    @require: 0 <= s<t < taille_graphe et m non NULL et consignes.
    @assign: modifie temps_parcours.
    @ensure: fonction qui effectue l'algorithme de recherche de chemin A* dans un graphedonné ;
    retourne un tableau valant NULL (tab_fin) si aucun chemin trouvé, sinon retourne un tableau
    avec les différents noeuds du chemin trouvé rangés du départ à l'arrivée (tab_fin). 
*/
int *a_etoile(int choix, int** m, int taille_graphe, int s, int t, int *temps_parcours,int n)
{
    file_priorite fp = creation_fp(taille_graphe);
    
    int * tab_pred = NULL, *tab_visite = NULL, * tab_fin = NULL, * tab_pos_noeud = NULL;;

    tab_visite = allocation_dynamique_tableau(tab_visite,taille_graphe);
    tab_pred = allocation_dynamique_tableau(tab_pred,taille_graphe);
    tab_pos_noeud = allocation_dynamique_tableau(tab_pos_noeud,taille_graphe);
    

    int i;

    int suc_pot = 0;

    int sommet = 0;
    int entree = s;
    int sortie = t;
    int cout_mini = 0;

    int heuri=0;
    int cout=0;

    insertion_tas(entree,&fp,&heuri,&cout,tab_pos_noeud); /*On insère le premier sommet dans le tas*/
    tab_pred[s] = -1;

    while ((fp->taille > 0) && tab_pred[sortie] == 0)  /*Tant que le tas n'est pas vide et que le sommet de sortie n'a pas de predecesseur*/
    {
        sommet = extraction_tas(&fp,&cout_mini,tab_pos_noeud); /*On extrait le sommet de plus petit cout heuristique*/
        tab_visite[sommet] = 1;/*On le marque*/


        for (i = 0; i < 8; ++i)/*On fait défiler ses voisins*/
        {
            suc_pot = m[sommet][i];
            if ( (suc_pot != -1) && tab_visite[suc_pot] == 0) /*Si on a trouvé un voisin pas encore marqué*/
            {
                calcul_distance(m[suc_pot][8],m[suc_pot][9],n,&heuri,&cout,&cout_mini,choix);/*On calcule les différents coûts ...*/
                recherche_tas(tab_pred, suc_pot,&fp,&heuri,&cout,sommet,tab_pos_noeud);/*... et on met à jour le tas*/
            }
        }
    }


    if (fp->taille <= 0) /*On a pas trouvé de chemin */
    {
        tab_fin = NULL; 
    }

    else      /*On a trouvé un chemin*/
    {
        tab_fin = construction_chemin(tab_pred,taille_graphe,t,temps_parcours); /*On remonte les prédécesseurs*/
    }

    free(fp->tas);
    free(fp);

    free(tab_visite);
    free(tab_pred);
    free(tab_pos_noeud);

    return tab_fin;

}

/*  
    @require: fp non NULL, tab_pred non NULL, sommmet est le prédecesseur de suc_pot et consignes.
    @assign: modifie fp, tab_pos_noeud et tab_pred.
    @ensure: fonction qui insére le successeur et/ou modifie ses caractéristiques ; ne retourne rien.
*/
void recherche_tas(int *tab_pred,int suc_pot,file_priorite *fp,int *heuri,int *cout,int sommet, int *tab_pos_noeud)
{
    int present = 0;

    if (tab_pos_noeud[suc_pot] != 0) /*Si le noeud est déjà présent dans le tas ...*/
    {
        if ( ((*fp)->tas[tab_pos_noeud[suc_pot]]).cout >= *cout)/*... et si il a un cout plus petit*/
        {
    		/*On met à jour*/
            ((*fp)->tas[tab_pos_noeud[suc_pot]]).cout = *cout;
            ((*fp)->tas[tab_pos_noeud[suc_pot]]).heuri = *heuri;
            ((*fp)->tas[tab_pos_noeud[suc_pot]]).cout_heuri = *heuri + *cout;
            tab_pred[suc_pot] = sommet+1;
            modification_tas(tab_pos_noeud[suc_pot],fp,tab_pos_noeud);

            return;
        }   

        present +=1;
    }

    else if (((*fp)->tas[0]).valeur == suc_pot) /*Même chose si le noeud est la racine ...*/
    {
        if ( ((*fp)->tas[0]).cout >= *cout)/*... et a un cout plus petit*/
        {
    		/*On met à jour*/
            ((*fp)->tas[0]).cout = *cout;
            ((*fp)->tas[0]).heuri = *heuri;
            ((*fp)->tas[0]).cout_heuri = *heuri + *cout;
            tab_pred[suc_pot] = sommet+1;
            modification_tas(0,fp,tab_pos_noeud);

            return;
        }   

        present +=1;
    }

    if (present)/*Si le noeud est présent dans le tas avec un cout plus petit, on ne fait rien*/
    {
        return;
    }

    else/*Si il n'est pas déjà présent, on l'ajoute au tas*/
    {
        tab_pred[suc_pot] = sommet+1;
        insertion_tas(suc_pot,fp,heuri,cout,tab_pos_noeud);
        return;
    }
}

/*  
    @require: fp non NULL.
    @assign: modifie fp et tab_pos_noeud.
    @ensure: fonction qui extrait le sommet de cout heuristique le plus faible dans le tas et qui fait remonter
    le nouveau sommet de cout heuristique le plus faible dans le tas suivant son cout heuristique de manière à conserver
    une file de priorité ; retourne le numero du sommet extrait (min).
*/
int extraction_tas(file_priorite *fp,int *cout_mini, int *tab_pos_noeud)
{

    int min = ((*fp)->tas[0]).valeur; /*On enlève le dernier noeud du tas et on le met à la place du premier que l'on retire du tas*/
    *cout_mini = ((*fp)->tas[0]).cout; 

    int v = (*fp)->tas[(*fp)->taille-1].cout_heuri;
    int ancien = (*fp)->taille-1;

    tab_pos_noeud[((*fp)->tas[0]).valeur] = 0;
    (*fp)->tas[0] = (*fp)->tas[(*fp)->taille-1];
    tab_pos_noeud[((*fp)->tas[(*fp)->taille-1]).valeur] = 0;
    (*fp)->taille --;

    int i = 0;

    while (2*i+1 < (*fp)->taille)/*Tant que le père est plus petit que l'un de ses deux fils */
    {
        int j = 2*i+1;
        if ( (j +1 < (*fp)->taille) && ( (*fp)->tas[j+1].cout_heuri < (*fp)->tas[j].cout_heuri) )/*Si l'un des deux fils est plus petit*/
        {
            j++;
        } 
        if ( v < (*fp)->tas[j].cout_heuri)/*Si le père est plus petit que ses deux fils*/
            break;

        (*fp)->tas[i] = (*fp)->tas[j];
        tab_pos_noeud[((*fp)->tas[i]).valeur] = i;
        i = j;           
    }

    (*fp)->tas[i] = (*fp)->tas[ancien];/*On place l'ancienne racine à sa nouvelle place*/
    tab_pos_noeud[((*fp)->tas[i]).valeur] = i;
    return min;
} 


/*  
    @require: fp non NULL.
    @assign: modifie fp et tab_pos_noeud.
    @ensure: fonction qui insère dans le tas un nouvel élément qui va donc automatiquement être trié dans
    le tas suivant son cout heuristique ; ne retourne rien.
*/
void insertion_tas(int v,file_priorite *fp,int *heuri,int*cout, int *tab_pos_noeud)
{
    int largeur = (*fp)->taille;
    int poids = *heuri + *cout;

    (*fp)->taille +=1;

    while (largeur > 0 && (*fp)->tas[(largeur-1)/2].cout_heuri >= poids)/*Tant que le noeud à remonter est plus petit que son père on le remonte*/
    {
        (*fp)->tas[largeur] = (*fp)->tas[(largeur-1)/2];
        tab_pos_noeud[((*fp)->tas[largeur]).valeur] = largeur;
        largeur = (largeur-1)/2;
    }

    ((*fp)->tas[largeur]).valeur = v;/*On met à jour les infos du noeud ajoutées au tas dans sa nouvelle place*/
    ((*fp)->tas[largeur]).cout = *cout;
    ((*fp)->tas[largeur]).heuri = *heuri;
    ((*fp)->tas[largeur]).cout_heuri = *heuri + *cout;
    tab_pos_noeud[((*fp)->tas[largeur]).valeur] = largeur;
}


/*  
    @require: fp non NULL, tab_pos_noeud non NULL, pos_fils<taille.
    @assign: modifie fp et tab_pos_noeud.
    @ensure: fonction qui modifie le cout heuristique d'un sommet qui va donc automatiquement être trié dans
    le tas suivant son nouveau cout heuristique; ne retourne rien.
*/
void modification_tas(int pos_fils,file_priorite *fp,int *tab_pos_noeud)
{
    int largeur = pos_fils;
    info ret = (*fp)->tas[largeur];
    int poids = (*fp)->tas[largeur].cout_heuri;

    while (largeur > 0 && (*fp)->tas[(largeur-1)/2].cout_heuri >= poids)/*Tant que le noeud à remonter est plus petit que son père on le remonte*/
    
    {
        (*fp)->tas[largeur] = (*fp)->tas[(largeur-1)/2];
        tab_pos_noeud[((*fp)->tas[largeur]).valeur] = largeur;
        largeur = (largeur-1)/2;
    }

    ((*fp)->tas[largeur]) = ret;/*On met à jour les infos du noeud ajoutées au tas dans sa nouvelle place*/
    tab_pos_noeud[((*fp)->tas[largeur]).valeur] = largeur;
}


/*  
    @require: taille_graphe >=0.
    @assign: ne mofifie rien.
    @ensure: fonction qui crée une strucure qui contient une file de priorité sous la forme d'un tableau ordonnée en tas 
    ainsi que la longueur du tas ; retourne la structure (fp); 
*/
file_priorite creation_fp(int taille_graphe)
{
    file_priorite fp = malloc(sizeof(fp));

    if (fp == NULL)
    {
        fprintf(stderr, "Erreur allocation dynamique dans creation_fp\n");
        exit(EXIT_FAILURE);
    }

    info * tab = (info *)calloc(taille_graphe,sizeof(info));

    if (tab == NULL)
    {
        fprintf(stderr, "Erreur allocation dynamique dans creation_fp\n");
        exit(EXIT_FAILURE);
    }

    fp->tas = tab;
    fp->taille = 0;

    if (fp->tas == NULL)
       exit(EXIT_FAILURE);

    return fp;
}

/*--------------------------------Fonction pour le débogage--------------------------------*/

/*  
    @require: fp non NULL.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui affiche le tas ainsi que certaines informations utiles ; ne retourne rien. 
*/
void afficher_tas(file_priorite fp)
{
    int compteur = fp->taille,i;
    for (i = 0; i < compteur; ++i)
    {
        fprintf(stdout,"%d (%d/%d/%d)-",(fp->tas[i]).valeur,(fp->tas[i]).cout_heuri,(fp->tas[i]).cout,(fp->tas[i]).heuri);
    }
    fprintf(stdout,"\n");
}

