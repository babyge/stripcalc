#$Header$
#
#$Log$
#Revision 1.5  2000/06/21 18:05:48  mikef
#*** empty log message ***
#
#Revision 1.10  2000/02/24 21:53:40  mikef
#Fix numerical root finder causing blow-up
#
#Revision 1.9  1999/12/13 22:51:13  mikef
#Add header and log keywords to all files
#
#

#HMSHOST=rtfm.sr.hp.com
#HMSDIR=/MFeetools/stripcalc

#CFLAGS = -Ae -O -I/usr/local/include

CC = gcc

CFLAGS = -O

#LDFLAGS =-L /usr/local/lib/p2c 

MAKEFILE = Makefile

SHELL = /bin/sh

TARGETS	= stripcalc msctl msctl2 linestub cheby bpf helical_c helical_s helical_filter interdig slbrctl

all:		$(TARGETS)

stripcalc :     stripcalc.c stripsubs.o mathlib.o myprintf.o
		$(CC) $(CFLAGS) -o stripcalc stripcalc.c stripsubs.o mathlib.o $(LDFLAGS) -lp2c myprintf.o -lc -lm 

msctl2:	msctl2.c mathlib.o myprintf.o
	$(CC) $(CFLAGS) -o msctl2 msctl2.c mathlib.o myprintf.o -lm

msctl:	msctl.c mathlib.o myprintf.o
	$(CC) $(CFLAGS) -o msctl msctl.c mathlib.o myprintf.o -lm

slbrctl: slbrctl.c mathlib.o myprintf.o
	$(CC) $(CFLAGS) -o slbrctl slbrctl.c mathlib.o myprintf.o -lm

interdig:	interdig.c mathlib.o myprintf.o
	$(CC) $(CFLAGS) -o interdig interdig.c mathlib.o myprintf.o -lm

linestub: linestub.c mathlib.o myprintf.o
	$(CC) $(CFLAGS) -o linestub linestub.c mathlib.o myprintf.o -lm

cheby: cheby.c mathlib.o myprintf.o
	$(CC) $(CFLAGS) -o cheby cheby.c mathlib.o myprintf.o -lm

bpf: bpf.c mathlib.o myprintf.o
	$(CC) $(CFLAGS) -o bpf bpf.c mathlib.o myprintf.o -lm

srbpf: srbpf.c mathlib.o myprintf.o
	$(CC) $(CFLAGS) -o srbpf srbpf.c mathlib.o myprintf.o -lm

helical_c: helical_c.c  myprintf.o 
	$(CC)  $(CFLAGS) -o helical_c helical_c.c myprintf.o -lm

helical_s: helical_s.c myprintf.o
	$(CC)  $(CFLAGS) -o helical_s helical_s.c myprintf.o -lm

helical_filter: helical_filter.c myprintf.o
	$(CC)  $(CFLAGS) -o helical_filter helical_filter.c myprintf.o -lm

update:	
	cvs update -A

clean:;		@rm -f *.o core *~ \#*

clobber:;	@rm -f *.o *~ \#* core tags $(TARGETS)

###
myprintf.o: myprintf.c
mathlib.o: mathlib.h
stripcalc.o: mathlib.h stripsubs.h
stripsubs.o: stripsubs.h mathlib.h
msctl.o: mathlib.h  msctl.c
msctl:  mathlib.o
msctl2: mathlib.o
msctl2.o: mathlib.o
linestub.o: mathlib.o
linestub: mathlib.o
