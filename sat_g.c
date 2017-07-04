#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include "lecture.h"
#include "sat_g.h"
#include "couleur.h"

int * initTabVarNonAffect(int nb){
	int n=1;
	int * tab=NULL;
	tab=(int*) calloc(sizeof(int),(nb+1));
	for(;n<=nb;n++)tab[n-1]=n;
	return tab;
}

int solver(int* solution, int* tabVar,Liste_var* var,Liste_cl* clause){

	int v;
	int* V=tabVar;
	if (!tabVar) return 0;
	v=-tabVar[0];
	solution[abs(v)-1]=v;
	if (tabVar[1]) V=tabVar+1;
	change_var(v,&var,&clause);
	if (!testWin(clause)) return 1;
	if (tabVar[0] != V[0]) 
		solver(solution,V,var,clause);
	if (!testWin(clause)) return 1;
	
	v=-v;
	solution[abs(v)-1]=v;
	change_var(v,&var,&clause);
	if (!testWin(clause)) return 1;
	if (tabVar[0] != V[0])
		solver(solution,V,var,clause);
	if (!testWin(clause)) return 1;
	return 0;
}

int resteCoup(int * solUser,int * solution, int n){	
	int i;
	int nbWin=0;
	for(i=0; i<n;i++)
		if(solUser[i]!=solution[i])nbWin++;		
	return nbWin;		
}
	
void aideCoup(int* solucUser,Liste_cl* clause,Liste_var* var,int* solution){
    clause->nbCoup++;
    int* tab;
    int i,j,z;
    int memo=clause->nbCoup;
    if(solucUser[0]==0) printf("\nProchain coup : %d\n",solution[0]);
    else {
        tab=initTabVarNonAffect(var->nb_var);
        for(i=0;i<var->nb_var && tab[i]!= 0;i++)
            for(j=0;j<var->nb_var;j++){
                if (solucUser[j]!=0 && tab[i]==(abs(solucUser[j]))){
                    for(z=i;tab[z]!=0 && z<var->nb_var;z++) tab[z]=tab[1+z];
                    tab[z]=0;
				}
            }
        if (tab[0] && solver(solution,tab,var,clause)) printf("\nProchain coup : %d\n",solution[tab[0]-1]);
        else {
            free(tab);
            tab=initTabVarNonAffect(var->nb_var);
            solver(solution,tab,var,clause);
            for(i=0;i<var->nb_var;i++)
                if (solucUser[i]!=solution[i]) {printf("\nProchain coup : %d\n",solution[i]);break;}
            
        }
        resetClause(&clause);
	    clause->nbCoup = 0;
        for(i=0;i<var->nb_var;i++) if (solucUser[i] != 0) change_var(solucUser[i],&var,&clause);
        free(tab);
        clause->nbCoup=memo;
    }
}

void affiche(Liste_var* var, Liste_cl* clause){
    printf("=============Sat Game==============\n");
    int i=0;
    int j;
    int cpt2;
    char tab[5];
    char tab2[5];
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    Litteral *l=NULL;
	Liste_var* p;
    int taillePlusClause=plusGrandeClause(clause);
    int pourri=tailleClause(taillePlusClause,var)+2;
    int cbClause=(int) (w.ws_col/pourri);
    int cpt=0;
    int nb=clause[0].nb_clause;
    for (;i<nb;i++){
        if (clause[i].p_lit){
            l=clause[i].p_lit;
            printf("| ");
            for(cpt2=0;l!=NULL;l=l->next_cl,cpt2++){
                p = l->lien_P;
                sprintf(tab, "%i",p->val);
                sprintf(tab2, "%i",var->nb_var);
                while(strlen(tab) < strlen(tab2)){
                    strcat(tab," ");
                }
                if ((l->signe) > 0 ){                
					if(l->state==GREEN){
						cooleurSigne(tab,VERT,'+');
                    }else if(l->state==RED){
						cooleurSigne(tab,ROUGE,'+');
					}else{
						if (clause[i].unitaire==0)
                            printf("+%s ",tab);
                        else
                            cooleurSigne(tab,JAUNE,'+');
					}	
                }else{
					if(l->state==GREEN){
						cooleurSigne(tab,VERT,'-');
                    }else if(l->state==RED){
						cooleurSigne(tab,ROUGE,'-');
					}else{	
                        if (clause[i].unitaire==0)
                            printf("-%s ",tab);
                        else
                            cooleurSigne(tab,JAUNE,'-');
					}	
				}
            }
            if (cpt != cbClause){
                cpt++;
                while (cpt2<taillePlusClause){
                    for(j=0;j<strlen(tab2);j++) printf(" ");
                    printf("  ");
                    cpt2++;
                    if(i+1>=nb && cpt2>=taillePlusClause)printf("|\n");
                }
            }
            else{
                while (cpt2<taillePlusClause){
                    for(j=0;j<strlen(tab2);j++) printf(" ");
                    printf("  ");
                    cpt2++;
                    if(i+1>=nb && cpt2>=taillePlusClause)printf(" |\n");
                }
                if(i+1<nb )printf(" |\n");
                cpt=0;
            }
        }
    } 
}

int ajustString(char * c){
	
	for(;*c && *c!='\n' ; c++);
		*c = '\0';	
	return 0;	
}

int plusGrandeClause(Liste_cl* clause){
    int i=0;
    int cpt;
    Litteral* l;
    int max=0;
    for(;i<clause->nb_clause;i++){
        for(cpt=0,l=clause[i].p_lit;l!=NULL;l=l->next_cl,cpt++);
        if (cpt>max) max=cpt;
    }
    return max;
}

int tailleClauseAct(Litteral* l){
    int cpt = 0;
    for(;l!=NULL;cpt++,l=l->next_cl);
    return cpt;
}

int tailleClause(int max,Liste_var* var){
    char tab[5];
    sprintf(tab, "%i",var->nb_var);
    int compteur=max*(strlen(tab)+1);
    compteur=compteur+max+1+2;
    return compteur;
}

void resetClause(Liste_cl** clause){
	
	Litteral *l=(*clause)[0].p_lit;
	int nb=(*clause)[0].nb_clause;	
	int i=0;	
	for(; i<nb;i++){
		l=(*clause)[i].p_lit;
        (*clause)[i].valide=0;
        testUnitaire(clause);
		if(l) for(;l;l->state=CLEAN,l=l->next_cl);									
	}
	(*clause)[0].nbCoup++;
		
}

int gagner(Liste_cl* clause){
	if (!testWin(clause)){
		printf("Vous avez gagné en %d coups !\n",clause[0].nbCoup);
		return 0;
	}
	return 1;
}

int testWin(Liste_cl* clause){
	int i=0;
	int nb=clause[0].nb_clause;	
	int win=0;
	
	for(; i<nb;i++){
		if(clause[i].valide)
			win++;		
	}
	if(win==nb){	
		return 0;
	}
	return 1;		
}

void testUnitaire(Liste_cl** clause){
    
    Litteral* l;
	int nb=(*clause)[0].nb_clause;	
	int i=0;
    int j=0;
    int nbRed=0;
	
    for(; i<nb;i++){
        nbRed=0;
		l=(*clause)[i].p_lit;
        j=tailleClauseAct(l);
		if(l){
            for(;l;l=l->next_cl){
                if(l->state==RED)nbRed++;
            }    
        }
        if(j-nbRed == 1 )(*clause)[i].unitaire=1; 
        else (*clause)[i].unitaire=0;
    }   
}


void modifClause(Liste_cl** clause){
	Litteral *l;	
	int i=0;
	int nb=(*clause)[0].nb_clause;
	int valide;
	
	for(i=0; i<nb;i++){
        l=(*clause)[i].p_lit;
		valide=0;		
		if(l){			
			for(; l && !valide ;l=l->next_cl){
				if(l->state==GREEN){
					(*clause)[i].valide=1;
					valide=1;									
				}else{
					(*clause)[i].valide=0;
				}	
			}
		}
	}
}

void change_var(int nb,Liste_var** var, Liste_cl** clause){
	
	Litteral* l=NULL;
	int nb_var=(*var)[1].nb_var;
	int valeur;
	int cpt=nb;	
	if(nb<0)cpt=-nb;
								
	if( nb<=nb_var && nb>=-nb_var ){				
		for(l=(*var)[cpt].p_lit; l;l=l->next_va){			
			valeur=( ((*var)[cpt].val)*(l->signe) );			
			if( valeur==nb ){
				l->state=GREEN;					
			}else{
				l->state=RED;					
			}		
		}
	}else{
		cooleur("Parametre trop grand !\n",ROUGE);
	}	
	(*clause)[0].nbCoup++;		
	modifClause(clause);
    testUnitaire(clause);
}	

void deleteLit(Litteral* l){
    if(l->next_va!=NULL) deleteLit(l->next_va);
    free(l);
}

void extinction(Liste_var* var, Liste_cl* clause){
    int i = 1;
    for(; i <= var->nb_var;i++){
        if(var[i].p_lit) deleteLit(var[i].p_lit);
    }
    free(var);
    free(clause);
}


void afficheHelp(int n){
    printf("\nHelp: Ba vous etes dedans...\n");
    printf("0 p: Vous donne le prochain coup possible à joué \n");
    printf("0 s: Vous donne la solution mais vous avez perdu... \n");
    printf("0 r: Reset la matrice \n");
    printf("[valeur]: Saisir entre -%d et %d  \n",n,n);
    printf("Exit: Ferme le jeux\n");
}

void menu(int* solution, Liste_cl* clause, Liste_var* var){
    int exit=1;
    int perdu=0;
    int i = 0;
    char chaine[1000];
    int* solucUser = (int*) calloc(sizeof(int),var->nb_var);
    printf("Solution :");
    for(;i<var->nb_var;i++) printf("%d ",solution[i]);
    printf("\n");
    affiche(var,clause);
    while(exit){					
        printf("\nPrompt>");
        fgets(chaine,1000,stdin);
        ajustString(chaine);
        printf("\n"); 
        if(!strcmp(chaine, "exit\0")) exit=0;
		else if (!strcmp(chaine, "0 p\0")) {affiche(var,clause);aideCoup(solucUser,clause,var,solution);}
		else if (!strcmp(chaine, "0 s\0")) {perdu=1;printf("Solution :");for(i=0;i<var->nb_var;i++) printf("%d ",solution[i]);printf("\n");exit=0;}
		else if (!strcmp(chaine, "help\0")) {affiche(var,clause);afficheHelp(var->nb_var);}
		else if (!strcmp(chaine, "0 r\0")){
			resetClause(&clause);		
			affiche(var,clause);
		}
		else if (abs(atoi(chaine))<=var->nb_var && atoi(chaine)!=0){solucUser[abs(atoi(chaine))-1]=atoi(chaine); change_var(atoi(chaine),&var,&clause);affiche(var,clause);}
        else cooleur("Ecrire 'help' pour connaitre les commandes\n",ROUGE);
        if (!gagner(clause)) exit=0;
    }
    if (perdu){
        printf("\n================================ PERDU ====================================\n");
        printf("Il vous restez %d coups à jouer ! \n",resteCoup(solucUser,solution,var->nb_var));
    }
    extinction(var,clause);
    free(solucUser);
}
