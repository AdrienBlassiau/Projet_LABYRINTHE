#include <stdio.h>
#include <stdlib.h>
#include "customlibs/dijkstra.h"


int main() {
    int n, s, t;
	scanf("%d",&n);
    int ** matrix = (int**) malloc(n*sizeof(int*));
    int i,j;
	
    

    for(i = 0; i < n; i++) {
        int* line = (int*) malloc(n * sizeof(int));
        matrix[i] = line;
        for(j = 0; j < n; j++) {
            scanf("%d", &line[j]);
		}
    }
	
    scanf("%d", &s);
    scanf("%d", &t);

    /*Appel de l'algorithme de dijkstra */
    resolution_dijkstra(n,s-1,t-1,matrix);
	
    for(i = 0; i < n; i++) {
        free(matrix[i]);
	}
	
    free(matrix);
    return EXIT_SUCCESS;
}


