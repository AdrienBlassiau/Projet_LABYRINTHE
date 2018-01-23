#ifndef __AETOILE__
#define __AETOILE__


typedef struct info info, *pt_info;
typedef struct file_p *file_priorite;

int *a_etoile(int choix, int** m, int taille_graphe, int s, int t, int *temps_parcours,int n);
void recherche_tas(int *tab_pred,int pred_pot,file_priorite *fp,int *dist_arrivee,int *dist_depart,int sommet, int *tab_pos_noeud);
int extraction_tas(file_priorite *fp,int *cout_mini, int *tabPosnoeud);
void insertion_tas(int v,file_priorite *fp,int *dist_arrivee,int*dist_depart, int *tabPosnoeud);
void modification_tas(int pos_fils,file_priorite *fp,int *tab_pos_noeud);
file_priorite creation_fp(int taille_graphe);

void afficher_tas(file_priorite fp);


#endif