#ifndef __PLUSCOURTCHEMIN__
#define __PLUSCOURTCHEMIN__

typedef struct file file, *pt_file;


void resolution_parcours_largeur(int** m, int taille_graphe, int s, int t);
int *parcours_largeur(int** m, int taille_graphe, int s, int t, int *temps_parcours);
void enfiler(int v,pt_file *tete,pt_file *queue);
int defiler(pt_file *tete,pt_file *queue);
void nettoyer(pt_file *tete, pt_file *queue);
int * construction_chemin(int *tab,int n, int t,int * temps_parcours);
void affichage_chemin_exo1(int * tab, int n);
int * allocation_dynamique_tableau(int* tab, int taille);



#endif