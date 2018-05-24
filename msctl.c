/*   Microstrip coupled line calculator
     Copyright (c) 1986,1991 M. Ferrara

$Header$

$Log$
Revision 1.3  2003/04/18 21:31:56  mikef
*** empty log message ***

Revision 1.5  2002/12/05 23:41:13  mikef
*** empty log message ***

Revision 1.4  2002/12/05 22:49:00  mikef
*** empty log message ***

Revision 1.3  2002/12/05 22:17:58  mikef
Remove p2c cruft

Revision 1.2  2000/06/21 18:05:48  mikef
*** empty log message ***

Revision 1.2  1999/12/13 22:51:13  mikef
Add header and log keywords to all files


*/

#include <stdio.h>
#ifndef MATHLIB_H
#include "mathlib.h"
#endif

double Zoo,Zoe,Z0,c,whso,whse,P,er,r,H,F,G;
double wh,sh,eeff,ht;

#include "get_input.h"

#define JMAX 4000

double rtbis(func,x1,x2,xacc)
double x1,x2,xacc;
double (*func)();	/* ANSI: double (*func)(double); */
{
	int j;
	double dx,f,fmid,xmid,rtb;

	f=(*func)(x1);
	fmid=(*func)(x2);
	if (f*fmid >= 0.0) fprintf(stderr,"Root must be bracketed for bisection in RTBIS\n");
	rtb = f < 0.0 ? (dx=x2-x1,x1) : (dx=x1-x2,x2);
	for (j=1;j<=JMAX;j++) {
		fmid=(*func)(xmid=rtb+(dx *= 0.5));
		if (fmid <= 0.0) rtb=xmid;
		if (fabs(dx) < xacc || fmid == 0.0) return rtb;
	}
	fprintf(stderr,"Too many bisections in RTBIS\n");
        return(NAN);
}

#undef JMAX

double numer(gparam)
   double(gparam);
{
   return((M_2_PI*acosh(((gparam+1)*F-2)/(gparam-1))
    +r*acosh(acosh(((gparam+1)*F+(gparam-1))/2)/acosh(gparam)))-whso);
 }



int main(argc,argv)
int argc;
char *argv[];

{
   
   printf("Microstrip coupled line calculator.\n");
   printf("Copyright (C) 1986,1991 M. Ferrara\n");
   get_double("Input Er: ",&er);
   get_double("Input substrate height: ",&ht);

   while(1) {   
      get_double("\nInput Zoe: ",&Zoe);
      if (Zoe == 0) exit(0);
      get_double("Input Zoo: ",&Zoo);
      if (Zoo == 0) exit(0);

      Z0=sqrt(Zoe*Zoo);
      c=((Zoe/Z0)*(Zoe/Z0)-1)/((Zoe/Z0)*(Zoe/Z0)+1);
      printf("Coupling factor = %.5g, which is %.5g dB\n",c,20*log10(c));
      printf("Z0 = %.3g\n",Z0);
      P=exp((Zoo/84.8)*sqrt(er+1))-1;
      whso=(8/P)*sqrt(((7+(4/er))*P/11.0)+(1+1/er)/0.81);
      P=exp((Zoe/84.8)*sqrt(er+1))-1;
      whse=(8/P)*sqrt(((7+(4/er))*P/11.0)+(1+1/er)/0.81);
      if (er >= 6) {
         r=M_1_PI;
       }
      else {
         r=8/(M_PI*(er+2));
       }
      F=cosh(M_PI_2*whse);
      G=rtbis(numer,1.0000001,50000.0,1e-8);
      H=((G+1)*F/2)+(G-1)/2;   
      wh=M_1_PI*(acosh(H)-acosh(G));
      sh=M_2_PI*acosh(G);
      printf("Strip width: %.5g\n",wh*ht);   
      printf("Strip spacing: %.5g\n\n",sh*ht);
    }
 }









