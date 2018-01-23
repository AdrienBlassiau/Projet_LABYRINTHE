#ifndef __LABYRINTHE__
#define __LABYRINTHE__


typedef struct noeud_labyrinthe noeud_labyrinthe;

noeud_labyrinthe ** creation_labyrinthe(int n, char matrix[1000][1002],int *compteur,int tab_cle_porte[2]);
void nettoyer_matrice(int ** matrice_adjacence, noeud_labyrinthe ** labyrinthe, int taille_graphe, int n);
int description_noeud(char matrix[1000][1002],int i, int j);
int chemin_valable(int choix,int type,int ** matrice_adjacence,int n, int timer,int taille_graphe,int tab_cle_porte[2]);
void resolution_labyrinthe(char matrix[1000][1002] ,int n, int timer);
int conversion_chemin(int k, int *tabRes_fin,int temps_parcours, int ** matrice_adjacence,char **tab_direction);
int ** creation_matrice_adjacence(int n, noeud_labyrinthe ** labyrinthe, int taille_graphe);
int** recherche_voisin(int j, int k, int** mat, noeud_labyrinthe ** labyrinthe, int n,int numero);
char** allocation_dynamique_matrice(char** tab, int taille);
void affichage_chemin_exo3(char **tab, int taille_graphe);
void calcul_distance(int x1,int y1, int taille_graphe, int* dist_arrivee, int* dist_depart, int*cout_mini,int choix_heuristique);

void tracage_chemin(char matrix[1000][1002],char matrix_modif[1000][1002],int n, int *tab_res_fin,int temps_parcours, int ** matrice_adjacence);
void copie_chemin(char matrix_modif[1000][1002] , int n, char * nom);
void affichage_labyrinthe(noeud_labyrinthe** labyrinthe, int n);
void affichage_matrice_ajacence(int **mat, int taille_graphe);



#endif