CFLAGS	      = -O -I/usr/local/include

DEST	      = .

EXTHDRS	      =

HDRS	      = mathlib.h \
		stripsubs.h

INSTALL	      = install

LD	      = cc

LDFLAGS	      =-L /usr/local/lib/p2c 

LIBS	      = 

MAKEFILE      = Makefile

OBJS	      = mathlib.o \
		stripcalc.o \
		stripsubs.o

PRINT	      = more

PROGRAM       = stripcalc 

SHELL	      = /bin/sh

SRCS	      = mathlib.c \
		stripcalc.c \
		stripsubs.c

all:		$(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS)
		@echo "Linking $(PROGRAM) ..."
		$(LD) -o $(PROGRAM) $(LDFLAGS) $(OBJS) $(LIBS) -lp2c -lc -lm 
		@echo "done"

msctl:		msctl.o mathlib.o
		cc -o msctl msctl.o mathlib.o $(LDFLAGS) -lp2c -lc -lm

clean:;		@rm -f $(OBJS) core

clobber:;	@rm -f $(OBJS) $(PROGRAM) core tags

depend:;	@mkmf -f $(MAKEFILE) ROOT=$(ROOT)

echo:;		@echo $(HDRS) $(SRCS)

index:;		@ctags -wx $(HDRS) $(SRCS)

install:	$(PROGRAM)
		@echo Installing $(PROGRAM) in $(DEST)
		@-strip $(PROGRAM)
		@if [ $(DEST) != . ]; then \
		(rm -f $(DEST)/$(PROGRAM); $(INSTALL) -f $(DEST) $(PROGRAM)); fi

print:;		@$(PRINT) $(HDRS) $(SRCS)

tags:           $(HDRS) $(SRCS); @ctags $(HDRS) $(SRCS)

update:		$(DEST)/$(PROGRAM)

$(PROGRAM): $(SRCS) $(LIBS) $(HDRS) $(EXTHDRS)
		@$(MAKE) -f $(MAKEFILE) ROOT=$(ROOT) DEST=$(DEST) install


msctl2:	msctl2.c
	cc -O -I/usr/local/include -o msctl2 msctl2.c mathlib.o -lm

msctl:	msctl.c
	cc -O -I/usr/local/include -o msctl msctl.c mathlib.o -lm


linestub: linestub.c
	cc -O -I/usr/local/include -o linestub linestub.c mathlib.o -lm

cheby: cheby.c mathlib.o
	cc -O -I/usr/local/include -o cheby cheby.c mathlib.o -lm

bpf: bpf.c mathlib.o
	cc -g -I/usr/local/include -o bpf bpf.c mathlib.o -lm

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
