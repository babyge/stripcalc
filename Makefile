CFLAGS	      = -Ae -O -I/usr/local/include

LDFLAGS	      =-L /usr/local/lib/p2c 

MAKEFILE      = Makefile

SHELL	      = /bin/sh

TARGETS	   =	stripcalc msctl msctl2 linestub cheby bpf helical_c helical_s helical_filter interdig

all:		$(TARGETS)

stripcalc :     stripcalc.c stripsubs.o mathlib.o
		cc $(CFLAGS) -o stripcalc stripcalc.c stripsubs.o mathlib.o $(LDFLAGS) -lp2c -lc -lm 

msctl2:	msctl2.c mathlib.o
	cc $(CFLAGS) -o msctl2 msctl2.c mathlib.o -lm

msctl:	msctl.c mathlib.o
	cc $(CFLAGS) -o msctl msctl.c mathlib.o -lm

interdig:	interdig.c mathlib.o
	cc $(CFLAGS) -o interdig interdig.c mathlib.o -lm

linestub: linestub.c mathlib.o
	cc $(CFLAGS) -o linestub linestub.c mathlib.o -lm

cheby: cheby.c mathlib.o
	cc $(CFLAGS) -o cheby cheby.c mathlib.o -lm

bpf: bpf.c mathlib.o
	cc $(CFLAGS) -o bpf bpf.c mathlib.o -lm

srbpf: srbpf.c mathlib.o
	cc $(CFLAGS) -o srbpf srbpf.c mathlib.o -lm

helical_c: helical_c.c
	cc  $(CFLAGS) -o helical_c helical_c.c -lm

helical_s: helical_s.c
	cc  $(CFLAGS) -o helical_s helical_s.c -lm

helical_filter: helical_filter.c
	cc  $(CFLAGS) -o helical_filter helical_filter.c -lm

tags:;		@etags *.c *.h

clean:;		@rm -f *.o core *~ \#*

clobber:;	@rm -f *.o *~ \#* core tags $(TARGETS)

###
mathlib.o: mathlib.h
stripcalc.o: mathlib.h stripsubs.h
stripsubs.o: stripsubs.h mathlib.h
msctl.o: mathlib.h  msctl.c
msctl:  mathlib.o
msctl2: mathlib.o
msctl2.o: mathlib.o
linestub.o: mathlib.o
linestub: mathlib.o
