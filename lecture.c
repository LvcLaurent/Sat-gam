#include <stdio.h>
#include <stdlib.h>
#include "lecture.h"
#define MAX 1000

int calcul(int nombre){
    int j=0;
    do{
        nombre=nombre/10;
        j=j+1;
    }while(nombre>0);
    return(j);
}

Liste_var* init_list_var(int nb){
	int i=1;
	Liste_var* l = (Liste_var*) malloc((nb+1)*sizeof(Liste_var));
    l[0].nb_var = nb ;
	for(; i<=nb; l[i].val=i, l[i].p_lit=NULL, l[i].nb_var = nb, i++);
	return l;
}

Liste_cl* init_list_cl(int nb){
	int i=0;
	Liste_cl* l = (Liste_cl*) malloc(nb*sizeof(Liste_cl));	
	l[0].nbCoup=0;
	for(; i<nb; l[i].unitaire=0,l[i].valide=0, l[i].p_lit=NULL, l[i].nb_clause = nb, i++);
	return l;
}	

int init_cnf(char* c, Liste_cl** clause, Liste_var** var){
	static int lecture = 0;
	static int p = 0;
    int tmp=0;
    int tailInt=0;
	if (!lecture) {
		for(;*c;c++){
			if(*c== 'p') p = 1;
			if (p  && *c == 'c' && c[1] == 'n' && c[2] == 'f') {c+=3; lecture = 1;}
						
			//malloc var & clause
			if (lecture && *c != ' ' && !(*clause)){
				if(!(*var)){
                    tmp = atoi(c);
                    tailInt = calcul(tmp);
                    (*var)=init_list_var(tmp);			
                    c+=tailInt;
				}else{
                    tmp = atoi(c);
                    tailInt = calcul(tmp);
                    (*clause)=init_list_cl(tmp);				
                    c+=tailInt;
				}
			}
		}
	}
	else{
		return 0;
	}
	return 1;
}
void init_litteral(int nb_lu, int nb_cl,Liste_cl* clause, Liste_var* var, Litteral** l_cl){
	
	Litteral* new_l;	
	new_l= (Litteral*) malloc(sizeof(Litteral));
	new_l->state=CLEAN;
	
	if(nb_lu>0){
		new_l->signe=1;
	}else{
		new_l->signe=-1;
		nb_lu=abs(nb_lu);
	}	
												
	new_l->lien_P=var+nb_lu;
	
	new_l->next_va=var[nb_lu].p_lit;					
	new_l->next_cl=NULL;
	var[nb_lu].p_lit = new_l;
	if (!(*l_cl)){
		(clause+nb_cl)->p_lit = new_l;
		(*l_cl) = new_l;
	}
	else{
		(*l_cl)->next_cl = new_l;
		(*l_cl) = new_l;
	}	
}
void parcours (char *nomF, Liste_cl** clause, Liste_var** var) {
	
	char ligne_lu[MAX];
	char* c;
    int tailInt=0;
	int nb_cl=0;
	int nb_lu;
	
	Litteral* l_cl=NULL;
		
	FILE* fichier=NULL;
	fichier= fopen(nomF, "r");	
	//Parcours du fichier ligne/ligne
	if (fichier){	
		while (fgets(ligne_lu, MAX, fichier)){ 
			c=ligne_lu;
			//Suppression des commentaires
			if( *c=='c');
			else if(init_cnf(c, clause, var));						
			else {				
				for(;*c;c++){
					//Lecture du nombre
					if ((*c != ' ') && (*c != '0') && (*c != '\n') && (*c != '\t')){
                        nb_lu = atoi(&(c[0]));
                        tailInt=calcul(abs(nb_lu));
                        if (nb_lu<0) c++;
						c+=tailInt;
						init_litteral(nb_lu, nb_cl, *clause, *var, &l_cl);	
					}
					if (*c == '0' ){
						nb_cl++;
						l_cl = NULL;
					}
				}	
			}	
		}	
	}
	else{		
		printf("Erreur de lecture du fichier");
		exit(1);
	}
    fclose(fichier);
}
