#ifndef _LECTURE_H_
#define _LECTURE_H_

typedef enum {CLEAN=0,GREEN=1,RED=2,YELLOW=3} etat;

// 0 clause fausse 1 clause valide

typedef struct Litteral{		
	int signe;
	etat state;
	struct Litteral* next_cl;	
	struct Litteral* next_va;	
	void* lien_P;
}Litteral;

typedef struct Liste_var{
		int val;
		Litteral* p_lit;	
		int nb_var;
}Liste_var;

typedef struct Liste_cl{
		int unitaire;
        int valide;
		int nbCoup;
		Litteral* p_lit;
        int nb_clause;
}Liste_cl;

Liste_var* init_list_var(int nb);

Liste_cl* init_list_cl(int nb);

//lecture du fichier
void parcours (char *nomF, Liste_cl** clause, Liste_var** var);

int init_cnf(char* c, Liste_cl** clause, Liste_var** var);

void init_litteral(int nb_lu, int nb_cl,Liste_cl* clause, Liste_var* var, Litteral** l_cl);
#endif
