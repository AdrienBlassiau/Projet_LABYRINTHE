#include <stdio.h>
#include <stdlib.h>
#include "customlibs/parcourslargeur.h"

int main() 
{
    int i,j;
    int n, s, t;
    
    scanf("%d",&n);

    int** matrix = (int**) malloc(n*sizeof(int*));

    for(i = 0; i < n; i++) {
        int* line = (int*) malloc(n * sizeof(int));
        matrix[i] = line;
        for(j = 0; j < n; j++) {
            scanf("%d", &line[j]);
		}
    }
	
    scanf("%d", &s);
    scanf("%d", &t);
   
    /*Appel de l'algorithme de parcours en largeur */
    resolution_parcours_largeur(matrix,n,s-1,t-1);

    for(i = 0; i < n; i++) {
		free(matrix[i]);
	}

    free(matrix);
    

    return EXIT_SUCCESS;
}


