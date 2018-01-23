#ifndef __DIJKSTRA__
#define __DIJKSTRA__

typedef struct noeuds noeuds;

void resolution_dijkstra(int n,int s,int t,int ** matrix);
noeuds * initialisation(noeuds *tab_noeuds, int n, int s);
void affichage_tab_noeuds(noeuds *tab_noeuds, int n);
int * dijkstra(int n,int s,int t,int ** matrix,int *temps_parcours);
int noeuds_cout(noeuds *tab_noeuds, int n);
int est_successeur(int u,int v,int ** matrix);
noeuds * maj(noeuds * tab_noeuds,int u, int v, int ** matrix);
int * construction_chemin_dijkstra(int s, int t,noeuds * tab_noeuds, int n,int * temps_parcours);
void affichage_chemin_exo2(int *tab, int n);


#endif





