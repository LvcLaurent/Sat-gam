#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lecture.h"
#include "sat_g.h"




int main(int argc, char *argv[]) {
	
	Liste_cl* clause=NULL;
	Liste_var* var=NULL;
	int* solution;
	int* tab;
    parcours(argv[1],&clause, &var);
	solution=(int*) malloc(var->nb_var*sizeof(int));
	tab=initTabVarNonAffect(var->nb_var);
	if (!solver(solution, tab, var, clause)){
		printf("\nCe fichier n'a aucune solution\n");
		extinction(var, clause);
	}
	else{
		resetClause(&clause);
		clause->nbCoup = 0;
		testUnitaire(&clause);
		menu(solution,clause,var);
	}
    free(solution);
    free(tab);
    return 0;
}
