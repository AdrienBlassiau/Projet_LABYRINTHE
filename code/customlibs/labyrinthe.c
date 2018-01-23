#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "aetoile.h"
#include "labyrinthe.h"



/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
/*---------------Fonctions générales et structure utilisées dans l'exercice 3--------------*/
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

struct noeud_labyrinthe
{
    int type; /* 0 = mur , 1 = point , 2 = porte , 3 = cle , 4 et plus = TP */
    int numero_noeud; /* -1 si mur ordre sinon on numérote dans l'ordre de parcours de la carte de haut en bas et de gauche à droite*/
    int indic;/* 1 si c'est le premier 2 si c'est le deuxieme*/
    
};


/*  
    @require: les paramètres sont les entrées standards de l'exercice 3 qui doivent vérifier les consignes.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction générale qui recherche un chemin dans un labyrinthe ne dépassant pas un temps donné (timer),
    utilise l'algorithme A* dans un premier temps (i=0) et si aucun chemin satisfaisant trouvé utilise le A* avec une heuristique nulle,
    donc Dijkstra mais comme les coêts sont de 1 c'est en fait un parcours en largeur ! Distingue deux cas, celui d'un labyrinthe avec porte et celui d'un labyrinthe sans porte.
    Ne retourne rien.
*/
void resolution_labyrinthe(char matrix[1000][1002] ,int n, int timer)
{
    int taille_graphe=0,i;
    int tab_cle_porte[2]={0,0};

    noeud_labyrinthe** labyrinthe = creation_labyrinthe(n,matrix,&taille_graphe,tab_cle_porte);
    int ** matrice_adjacence = creation_matrice_adjacence(n,labyrinthe,taille_graphe);

    for (i = 0; i < 2; ++i)
    {
        /*Si le graphe a une porte*/
        if (tab_cle_porte[0] != 0)
        {
            /*Essaye en premier de prendre la porte (3)*/
            if(chemin_valable(i,3,matrice_adjacence,n,timer,taille_graphe,tab_cle_porte))
            {
                nettoyer_matrice(matrice_adjacence,labyrinthe,taille_graphe,n);
                return ;
            }
                

            /*Essaye en second de ne pas prendre la porte (1)*/
            if(chemin_valable(i,1,matrice_adjacence,n,timer,taille_graphe,tab_cle_porte))
            {
                nettoyer_matrice(matrice_adjacence,labyrinthe,taille_graphe,n);
                return ;
            }
                
        }

        /*Si le graphe n'a pas de porte (2)*/
        {
            if(chemin_valable(i,2,matrice_adjacence,n,timer,taille_graphe,tab_cle_porte))  
            {
                nettoyer_matrice(matrice_adjacence,labyrinthe,taille_graphe,n);
                return ;
            }
                
        }
    }  
    
}



/*  
    @require: taille_graphe >=0 et consignes.
    @assign: modifie matrice_adjacence et labyrinthe.
    @ensure: fonction qui désalloue de la memoire occupée par des tableaux dynamiques ; ne retourne rien.
*/

void nettoyer_matrice(int ** matrice_adjacence, noeud_labyrinthe ** labyrinthe, int taille_graphe, int n)
{
    int i;
    for (i = 0; i < taille_graphe; ++i)
    {
        free(matrice_adjacence[i]);
    }

    for (i = 0; i < n; ++i)
    {
        free(labyrinthe[i]);

    }

    free(matrice_adjacence);
    free(labyrinthe);
}


/*  
    @require: rien (sauf consignes).
    @assign: modifie compteur (=nombre de sommet du graphe), tab_cle_porte.
    @ensure: fonction qui convertit le labyrinthe d'entrée (matrice de caractères) en une matrice de structures noeud_labyrinthe
    qui contient des informations sur chaque case du labyrinthe ; retourne cette matrice.
*/
noeud_labyrinthe ** creation_labyrinthe(int n, char matrix[1000][1002],int *compteur,int tab_cle_porte[2])
{
    int i,j;
    int tab_tp[11]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    noeud_labyrinthe** labyrinthe = (noeud_labyrinthe **)calloc(n,sizeof(noeud_labyrinthe*));

    if (labyrinthe == NULL)
    {
        fprintf(stderr, "Erreur allocation dynamique dans creation_labyrinthe\n");
        exit(EXIT_FAILURE);
    }

    /*On fait défiler toutes les lignes du labyrinthe*/
    for (i = 0; i < n; ++i)
    {
        labyrinthe[i]=(noeud_labyrinthe *)calloc(n,sizeof(noeud_labyrinthe));

        if (labyrinthe[i]== NULL)
        {
            fprintf(stderr, "Erreur allocation dynamique dans creation_labyrinthe\n");
            exit(EXIT_FAILURE);
        }

        /*On fait défiler toutes les cases du labyrinthe*/
        for(j = 0; j < n; j++) {

            labyrinthe[i][j].type=description_noeud(matrix,i,j);

            /*Si la case concernée est un téléporteur*/
            if (labyrinthe[i][j].type -1 >=0 && labyrinthe[i][j].type -1 < 10)
            {
                /*Si c'est le deuxième téléporteur (ordre d'arrivée) d'un type déjà rencontré*/
                if( tab_tp[labyrinthe[i][j].type -1] != -1)
                {
                    labyrinthe[i][j].numero_noeud = tab_tp[labyrinthe[i][j].type -1];
                    labyrinthe[i][j].indic = 2;                   
                }
                /*Sinon*/
                else
                {
                    labyrinthe[i][j].numero_noeud = *compteur;
                    labyrinthe[i][j].indic = 1;
                    tab_tp[labyrinthe[i][j].type -1] = *compteur;
                    *compteur = *compteur +1;    
                }
            } 

            /*Si la case concernée n'est pas un mur ni un téléporteur*/
            else if (labyrinthe[i][j].type !=-1)
            {

                labyrinthe[i][j].numero_noeud = *compteur;
                labyrinthe[i][j].indic = 1; 

                /*Si la case concernée est une clé*/
                if (labyrinthe[i][j].type == 11)
                    tab_cle_porte[0]=*compteur;

                /*Si la case concernée est une porte*/
                if (labyrinthe[i][j].type == 12)
                    tab_cle_porte[1]=*compteur;

                *compteur = *compteur +1;
            }
            else
                labyrinthe[i][j].numero_noeud = -1;
        }
    }

    return labyrinthe;
}


/*  
    @require: 0 <= i,j < n et consignes.
    @assign: ne modifie rien.
    @ensure: fonction qui assigne à chaque type de case du labyrinthe un nombre ; retourne ce nombre.
*/
int description_noeud(char matrix[1000][1002],int i, int j)
{
	/*On utilise un switch car il y a beaucoup de cas, cela est plus optimal qu'avec des if*/
    switch (matrix[i][j])
    {
        case 'S':   return 0;
        case 'E':   return 0;
        case 'X':   return -1;
        case '.':   return 0;
        case 'a':   return 11;
        case 'A':   return 12;
        case '*':   return 1;
        case '%':   return 2;
        case '$':   return 3;
        case '#':   return 4;
        case '&':   return 5;
        case '+':   return 6;
        case '-':   return 7;
        case '@':   return 8;
        case '^':   return 9;
        case '!':   return 10;
        default :   return 100; 
    }
}


/*  
    @require: choix_heuristique vaut 0 (algorithme A*) ou 1 (parcours en largeur); type vaut 1 (pas de porte), 2 (porte ignorée) ou 3 (porte non ignorée)
    matrice_adjacence non NULL, taille_graphe >=0 et consignes.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui teste si un chemin particulier respecte le timer ; retourne 0 si le chemin trouvé n'est pas bon, 1 sinon.
*/
int chemin_valable(int choix_heuristique,int type,int ** matrice_adjacence,int n, int timer,int taille_graphe,int tab_cle_porte[2])
{

    int porte=tab_cle_porte[1],  cle=tab_cle_porte[0], i, j, k=0, succes=0, temps_parcours, temps_total;

    int tab_etapes[4] = {0,cle,porte,taille_graphe-1};
    int *tab_fin = NULL;
    char **tab_direction = NULL; 
    int *voisin_porte=(int*)calloc(8,sizeof(int));

    if (voisin_porte== NULL)
    {
        fprintf(stderr, "Erreur allocation dynamique dans chemin_valable\n");
        exit(EXIT_FAILURE);
    }

    /*Si le labyrinthe est sans porte ...*/
    if (type == 1 || type == 2)
    {
        /*... la résolution se fera en 1 étape*/
        tab_etapes[1] = taille_graphe-1;
    }
    
    /*Si le labyrinthe est avec porte ...*/
    if (type == 1 || type == 3)
    {
        /*... on "élimine" toutes les arètes entre la porte et ses sommets adjacents dans un premier temps*/
        for (i = 0; i < 8; ++i)
        {
            voisin_porte[i]=matrice_adjacence[porte][i];
            matrice_adjacence[porte][i]=-1;
        }
    }
    

    temps_total=0;

    tab_direction = allocation_dynamique_matrice(tab_direction,n*n);

    /*On effectue la recherche du chemin dans le labyrinthe soit du début jusqu'a la fin soit du début jusqu'à la clé*/
    tab_fin = a_etoile(choix_heuristique,matrice_adjacence,taille_graphe,tab_etapes[0],tab_etapes[1],&temps_parcours,n);

    if ( tab_fin == NULL)
    {
        free(voisin_porte);
        free(tab_fin);
        free(tab_direction);

        return succes;
    }

    k=conversion_chemin(k,tab_fin,temps_parcours,matrice_adjacence,tab_direction);
    temps_total+=temps_parcours;      

    /*Si le labyrinthe est avec porte, on recontruit les arètes entre la porte et ses sommets adjacents...*/
    if (type == 1 || type == 3)
    {
        for (i = 0; i < 8; ++i)
        {
            matrice_adjacence[porte][i]=voisin_porte[i];
        } 
    }

    /* Si le labyrinthe est avec porte et que l'on passe par la porte, on effectue les étapes suivantes : clé->porte et porte->arrivée.*/
    if (type == 3)
    {
        for (j = 1; j< type; ++j)
        {
            tab_fin = a_etoile(choix_heuristique,matrice_adjacence,taille_graphe,tab_etapes[j],tab_etapes[j+1],&temps_parcours,n);
            if ( tab_fin == NULL)
            {
                free(voisin_porte);
                free(tab_fin);
                free(tab_direction);

                return succes;
            }

            k=conversion_chemin(k,tab_fin,temps_parcours,matrice_adjacence,tab_direction);
            temps_total+=temps_parcours;               
        }
    }

    free(voisin_porte);
    free(tab_fin);

    /*Si le temps du chemin trouvé est inférieur au timer, c'est un succès !*/
    if (temps_total <= timer)
    {
        affichage_chemin_exo3(tab_direction,k);
        succes ++;
    }

    free(tab_direction);

    return succes;
}

/*  
    @require: les différents tableaux et matrices non NULL sauf si temps_parcours est nulle et consignes.
    @assign: modifie tab_direction (et des variables locales).
    @ensure: fonction qui construit un tableau avec les différentes direction du chemin trouvé,
    à partir du tableau des différents sommets du chemin trouvé rangés du départ à l'arrivée (tab_fin) ;
    retourne le nombre d'éléments (k) du tableau de direction (tab_direction) construit.
*/
int conversion_chemin(int k, int *tab_fin,int temps_parcours, int ** matrice_adjacence,char **tab_direction)
{

    int i,j;
    char direction[256][256] = {"DROITE","GAUCHE","HAUT","BAS","DROITE","GAUCHE","HAUT","BAS"};
    int s1 = 0, s2 = 0;

    for (i = 0; i < temps_parcours; ++i) /*On fait défiler tout les noeuds du chemin.*/
    {
        if ( (matrice_adjacence[tab_fin[i]][10] != 0) || (matrice_adjacence[tab_fin[i]][11] != 0) )
        {
        	/*On va tester si on se téléporte, c'est à dire si on passe par deux téléporteurs du même type*/
            for (j = 0; j < 4; ++j)
            {
                if ( matrice_adjacence[tab_fin[i]][j] == tab_fin[i-1])
                {      
                    s1 += 1;
                }  

                if ( matrice_adjacence[tab_fin[i]][j] == tab_fin[i+1])
                {      
                    s1 += 1;
                } 
            }

            for (j = 4; j < 8; ++j)
            {
                if ( matrice_adjacence[tab_fin[i]][j] == tab_fin[i-1])
                {     
                    s2 += 1;
                }  

                if ( matrice_adjacence[tab_fin[i]][j] == tab_fin[i+1])
                {   
                    s2 += 1;
                }  
            }
            /*Si on sait TP ...*/
            if ( (s1 != 2) && (s2 !=2) )
            {
                tab_direction[k]="TP";
                k+=1;
            }
                
            s1 = 0;
            s2 = 0; 

        }

        for (j = 0; j < 8; ++j)/*On teste les voisins du noeuds pour trouver la direction de ce noeud relativement au noeud suivant dans le chemin*/
        {
            if ( matrice_adjacence[tab_fin[i]][j] == tab_fin[i+1])
            {
                tab_direction[k]=direction[j];
                k+=1;
            }        
        }

    }

    return k;
}  
   

/*  
    @require: taille_graphe >= 0 et labyrinthe non NULL et consignes. 
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui construit une matrice d'adjacence un peu particulière pour le graphe. En effet, chaque case (=sommet) possède au maximum
    8 voisines (en prenant en compte les téléporteurs) donc notre matrice d'adjacence ne fait ici figurer que les voisins de chaque sommet,
    de plus, elle conserve aussi les coordonnées de nos différents sommets :
    [droite,gauche,bas,haut,droite,gauche,bas,haut,x,y,x,y]. Tout est en double pour pouvoir traiter le cas des téléporteur.
    Retourne cette matrice d'adjacence.
*/
int ** creation_matrice_adjacence(int n, noeud_labyrinthe ** labyrinthe, int taille_graphe)
{

    int i,j,k,l;
    int** mat = (int**)calloc(taille_graphe,sizeof(int*)); /*Début de l'allocation mémoire*/

    if ( mat == NULL)
    {
        fprintf(stderr, "Erreur allocation dynamique dans creation_matrice_adjacence\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < taille_graphe; ++i)
    {
        mat[i] = (int*)calloc(12,sizeof(int));/*Suite de l'allocation mémoire*/

        if ( mat[i] == NULL)
        {
            fprintf(stderr, "Erreur allocation dynamique dans creation_matrice_adjacence\n");
            exit(EXIT_FAILURE);
        }

        for (l = 0; l < 8; ++l)
        {
            mat[i][l] = -1;/*On initialise toute les cases à -1*/
        }
    }

    for (j = 0; j < n; ++j)
    {
        for (k = 0; k < n; ++k)
        {
            /*Si la case n'est pas un mur, on conserve les coordonnées de la case*/
            if (labyrinthe[j][k].numero_noeud != -1)
            {
                /*Si c'est tout sauf le deuxième passage d'un téléporteur*/
                if (labyrinthe[j][k].indic == 1)
                {
                    mat[labyrinthe[j][k].numero_noeud][8] = j;
                    mat[labyrinthe[j][k].numero_noeud][9] = k;
                }

                else 
                {
                	/*La ligne de la matrice d'adjacence du deuxième TP est la même que le premier, on rajoute juste ses coordonées*/
                    mat[labyrinthe[j][k].numero_noeud][10] = j;
                    mat[labyrinthe[j][k].numero_noeud][11] = k;
                }

                /*On recherche les voisins de chaque sommet*/
                mat = recherche_voisin(j,k,mat,labyrinthe,n,labyrinthe[j][k].numero_noeud);
            }
        }
    }

    return mat;
}


/*  
    @require: mat non NULL, labyrinthe non NULL, numero < n*n, 0<=j,k<n et consignes.
    @assign: modifie mat (et des variables locales).
    @ensure: fonction qui remplit la matrice d'adjacence des voisins du sommet de coordonnés (j,k) ; retourne la matrice d'adjacence modifiée (mat).
*/
int** recherche_voisin(int j, int k, int** mat, noeud_labyrinthe ** labyrinthe, int n,int numero)
{

    if (labyrinthe[j][k].indic == 1)
    {
        if (k<n-1)
            if (labyrinthe[j][k+1].numero_noeud != -1)/*droite*/
                mat[numero][0] = labyrinthe[j][k+1].numero_noeud;
        if (k>0)
            if (labyrinthe[j][k-1].numero_noeud != -1)/*gauche*/
                mat[numero][1] = labyrinthe[j][k-1].numero_noeud;
        if (j>0)
            if (labyrinthe[j-1][k].numero_noeud != -1)/*bas*/
                mat[numero][2] = labyrinthe[j-1][k].numero_noeud;
        if (j<n-1)
            if (labyrinthe[j+1][k].numero_noeud != -1)/*haut*/
                mat[numero][3] = labyrinthe[j+1][k].numero_noeud;
    }

    else /*Si c'est le deuxième TP d'une paire*/
    {

        if (k<n-1)
            if (labyrinthe[j][k+1].numero_noeud != -1)/*droite*/
                mat[numero][4] = labyrinthe[j][k+1].numero_noeud;
        if (k>0)
            if (labyrinthe[j][k-1].numero_noeud != -1)/*gauche*/
                mat[numero][5] = labyrinthe[j][k-1].numero_noeud;
        if (j>0)
            if (labyrinthe[j-1][k].numero_noeud != -1)/*bas*/
                mat[numero][6] = labyrinthe[j-1][k].numero_noeud;
        if (j<n-1)
            if (labyrinthe[j+1][k].numero_noeud != -1)/*haut*/
                mat[numero][7] = labyrinthe[j+1][k].numero_noeud;
    }
    return mat;

    

}

/* 
    @require: tab non NULL et taille >= 0.
    @assign: modifie tab.
    @ensure: fonction d'allocation dynamique de matrice de charactères de taille donnée ; retourne un matrice dynamique de taille donnée (tab).
*/
char** allocation_dynamique_matrice(char** tab, int taille)
{
    int i;
    tab = (char**)calloc(taille,sizeof(char*));

    if ( tab == NULL)
    {
        fprintf(stderr, "Erreur allocation dynamique dans allocation_dynamique_matrice\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < taille; ++i)
    {
        tab[i] = (char*)calloc(4,sizeof(char));

        if ( tab[i] == NULL)
        {
            fprintf(stderr, "Erreur allocation dynamique dans allocation_dynamique_matrice\n");
            exit(EXIT_FAILURE);
        }
    }

    return tab;
}


/*  
    @require: tab non NULL et taille_graphe >= 0 .
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui affiche dans la sortie standard le chemin de taille minimum 
    entre l'entrée et la sortie si celui ci existe (comme précisé dans la consigne) ; ne retourne rien.
*/
void affichage_chemin_exo3(char **tab, int taille_graphe)
{
    int compteur = taille_graphe,i;
    for (i = 0; i < compteur; ++i)
    {
        fprintf(stdout,"%s \n",tab[i]);
    }
    fprintf(stdout,"\n");
}


/* 
    @require: cout_mini doit être le cout du prédecesseur selectionné dans poids_min, 0 <= x1,y1 < taille_graphe et consignes.
    @assign: modifie cout et heuri.
    @ensure: fonction qui calcule la distance séparant une case du départ (avec la distance du prédéceseur) ainsi que la distance 
    séparant une case de l'arrivée (avec la distance de manhattan) ; retourne ces deux distances (heuri et cout).
*/
void calcul_distance(int x1,int y1, int taille_graphe, int* heuri, int* cout, int*cout_mini, int choix_heuristique)
{

    *cout = *cout_mini + 1 ;
    if (choix_heuristique == 1) /* Si parcours en largeur*/
        *heuri = 0;
    else /*Si A * */
        *heuri = abs(x1 - taille_graphe +1) + abs(y1 - taille_graphe +1);
}




/*--------------------------------Fonction pour le débogage--------------------------------*/

/*  
    @require: tab_fin non NULL, temps_parcours >=0, matrice_adjacence non NULL et consignes.
    @assign: modifie matrix_modif.
    @ensure: fonction qui trace le chemin trouvé dans une nouvelle matrice de caractère avec un H;
    retourne cette nouvelle matrice (matrix_modif).
*/
void tracage_chemin(char matrix[1000][1002],char matrix_modif[1000][1002],int n, int *tab_fin,int temps_parcours, int ** matrice_adjacence)
{

    int i,j,k,ok=0;

    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            ok = 0;
            for (k = 0; k <= temps_parcours; ++k)
            {
                if ( ((i == matrice_adjacence[tab_fin[k]][8]) && (j == matrice_adjacence[tab_fin[k]][9]) ))
                {
                    matrix_modif[i][j]='H';
                    
                    ok=1 ; 
                }   
            }   

            if (ok==0)
            {
                matrix_modif[i][j]=matrix[i][j];
                
            }   
              
        }
        matrix_modif[i][j]='\n';
         
    }
} 


/*  
    @require: matrix_modif non NULL.
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui trace le labyrinthe avec chemin dans un fichier texte ouvert en écriture.
*/
void copie_chemin(char matrix_modif[1000][1002] , int n, char * nom)
{
    int i;

    FILE *fd = fopen(nom,"w");

    if (fd ==  NULL)/*Teste si l'appel système a fonctionné*/
    {
        fprintf(stderr, "%s : Erreur ouverture fichier en ecriture : %s\n", nom , strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; ++i)
    {
        fputs(matrix_modif[i],fd);

    }

    fclose(fd);
}


/*  
    @require: mat non NULL et taille_graphe >= 0 .
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui affiche dans la sortie standard la matrice d'adjacence du graphe ; ne retourne rien.
*/
void affichage_matrice_ajacence(int **mat, int taille_graphe)
{
    int i,j;

    fprintf(stdout,"\n-------------------------\n");
    for(i = 0; i < taille_graphe; i++)
    {
        fprintf(stdout,"%d :",i);
        for (j = 0; j <12; ++j)
        {
            fprintf(stdout,"%5d ",mat[i][j]);
        }

        fprintf(stdout,"\n");    
    }
}

/*  
    @require: labyrinthe non NULL et n >= 0 .
    @assign: ne modifie rien (uniquement des variables locales).
    @ensure: fonction qui affiche dans la sortie standard le labyrinthe avec des cases codées (labyrinthe) ;  ne retourne rien.
*/
void affichage_labyrinthe(noeud_labyrinthe** labyrinthe, int n)
{
    int i,j;

    fprintf(stdout,"\n-------------------------\n");
    for(i = 0; i < n; i++)
    {
        for (j = 0; j < n; ++j)
        {
            fprintf(stdout,"%4d",labyrinthe[i][j].numero_noeud);
        }

        fprintf(stdout,"\n");
        
    }
}