#ifndef _COULEUR_H_
#define _COULEUR_H_

#define JAUNE "\033[33m"
#define ROUGE "\033[31m"
#define VERT  "\033[32m"
#define NOIR  "\033[00m"

#define cooleur(msg,couleur)\
		printf("%s%s%s",couleur,msg,NOIR);

#define cooleurSigne(msg,couleur,signe)\
		printf("%s%c%s%s ",couleur,signe,msg,NOIR);
		
		
#endif
