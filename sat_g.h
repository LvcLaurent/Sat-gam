#ifndef _SAT_G_H_
#define _SAT_G_H_

//Rajoute \0 à la fin de la chaine
int ajustString(char * c);

//Lance le jeu avec le parser (menu)						
void menu(int* solution, Liste_cl* clause, Liste_var* var);

//Affecte la varriable (nb)
void change_var(int nb,Liste_var** var, Liste_cl** clause);

//Affiche la matrice
void affiche(Liste_var* var, Liste_cl* clause);

//Affiche les commandes
void afficheHelp();

//Parcours les clauses et test s'il y a une variable unitaire
void testUnitaire(Liste_cl** clause);

//Test si le joueur a gagné
int testWin(Liste_cl* clause);

//Supprime les litteraux d'un var
void deleteLit(Litteral* l);

//Appel deletLit et free les tableaux
void extinction(Liste_var* var, Liste_cl* clause);

//Parcours les clauses et enregistre si elle est satisfaite
void modifClause(Liste_cl** clause);

//Reinitialise le jeu mais pas le nb coup
void resetClause(Liste_cl** clause);

//Return la taille de la plus grand clause
int plusGrandeClause(Liste_cl* clause);

//Return la place que va prendre une clause
int tailleClause(int max,Liste_var* var);

//Return le nombre de litteraux d'une clause
int tailleClauseAct(Litteral* l);

//Initialise les variables non joué
int * initTabVarNonAffect(int nb);

//Résoud le sat game
int solver(int* solution, int* tabVar,Liste_var* var,Liste_cl* clause);

//Return le nombres de coups restant pour gagner
int resteCoup(int * solUser,int * solution, int n);

#endif
