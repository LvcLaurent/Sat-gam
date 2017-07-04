CC=gcc
CFLAGS= -Wall -g

OBJS= lecture.o sat_g.o sat-game.o

sat-game : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o sat-game

lecture.o: lecture.c lecture.h
	$(CC) $(CFLAGS) -c lecture.c

sat_g.o: sat_g.c lecture.h sat_g.h couleur.h lecture.c
	$(CC) $(CFLAGS) -c sat_g.c

sat-game.o: sat-game.c lecture.h sat_g.h
	$(CC) $(CFLAGS) -c sat-game.c

clean :
	rm *.o sat-game &> /dev/null
