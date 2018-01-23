#include <stdio.h>
#include <stdlib.h>

#include "customlibs/aetoile.h"
#include "customlibs/parcourslargeur.h"
#include "customlibs/labyrinthe.h"



/*17 : 8537*/
/*6 13 16 18*/
/* Prototypes et structures pour le plus court chemin*/


int main() {

    int n;
	char matrix[1000][1002] = { "" };
	int i;
	int timer;
	
    scanf("%d",&n);
	fgetc(stdin);
    
    for(i = 0; i < n; i++) {
        fgets(matrix[i], 1002, stdin);
    }
	
    scanf("%d", &timer);
    fgetc(stdin);

    /*Appel de l'algorithme de résolution de labyrinthe */
    resolution_labyrinthe(matrix,n,timer);

    return EXIT_SUCCESS;
}

