CC=gcc
DEBUG=-g

ppmcvt: pbm.o pbm_aux.o ppmcvt.o
	$(CC) $(DEBUG) -o ppmcvt pbm.o pbm_aux.o ppmcvt.o

ppmcvt.o: pbm.h pbm_aux.c ppmcvt.c
	$(CC) $(DEBUG) -c ppmcvt.c

pbm.o: pbm.h pbm.c
	$(CC) $(DEBUG) -c pbm.c

pbm_aux.o: pbm.h pbm_aux.c
	$(CC) $(DEBUG) -c pbm_aux.c

clean: 
	rm ppmcvt ppmcvt.o pbm.o