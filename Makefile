#$Header$
#
#$Log$
#Revision 1.7  2003/04/18 21:31:56  mikef
#*** empty log message ***
#
#Revision 1.13  2002/12/06 00:17:00  mikef
#Convert to gsl
#
#Revision 1.12  2002/12/05 23:03:03  mikef
#*** empty log message ***
#
#Revision 1.11  2002/12/05 22:17:57  mikef
#Remove p2c cruft
#
#Revision 1.10  2002/12/05 01:21:12  mikef
#*** empty log message ***
#
#Revision 1.9  2002/12/04 23:41:30  mikef
#*** empty log message ***
#
#Revision 1.8  2002/09/12 03:19:27  mikef
#*** empty log message ***
#
#Revision 1.9  2002/07/08 20:17:06  mikef
#Fix for typo in Waddell's book
#
#Revision 1.8  2002/07/08 01:06:49  mikef
#Add slectl with finite strip thickness
#
#Revision 1.7  2002/07/07 23:16:37  mikef
#Incremental checkin
#
#Revision 1.6  2002/07/07 21:35:22  mikef
#Initial import
#
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

CFLAGS = -g -Wall

#LDFLAGS =-L /usr/local/lib/p2c 

LIBS = stripsubs.o mathlib.o -lgsl -lgslcblas -lm -lreadline

MAKEFILE = Makefile

SHELL = /bin/sh

TARGETS	= stripcalc msctl msctl2 linestub cheby bpf helical_c helical_s helical_filter interdig slbrctl slectl srbpf

all:		$(TARGETS)

%:	%.c stripsubs.o mathlib.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) ${LIBS}

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
