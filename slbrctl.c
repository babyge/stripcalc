/*   Stripline broadside coupled line calculator
     Copyright (c) 1999 M. Ferrara

$Header$

$Log$
Revision 1.1  2000/06/21 18:07:37  mikef
*** empty log message ***

Revision 1.5  2000/02/24 22:31:44  mikef
Proper k range for elip1

Revision 1.4  2000/02/24 21:53:40  mikef
Fix numerical root finder causing blow-up

Revision 1.3  1999/12/13 22:51:13  mikef
Add header and log keywords to all files


*/

#include <p2c/p2c.h>
#include <stdio.h>
#ifndef MATHLIB_H
#include "mathlib.h"
#endif


#define v_light         2.99792458e+8   /*m/s*/
#define Eo              8.854187818e-12   /*F/m*/
#define Pi              3.14159265358979232646e+00
#define No              (120*Pi) /* ohms */
   
double Zoo,Zoe,Z0,c,whso,whse,P,er,r,H,F,G;
double w,b,s,r,k;

/***********************************************************************/
Static Void get_str(Prompt, ret)
Char *Prompt, *ret;
{
  Char *TEMP;

  printf("%*s", strlen(Prompt), Prompt);
  fgets(ret, 133, stdin);
  TEMP = strchr(ret, '\n');
  if (TEMP != NULL)
    *TEMP = 0;
}  /*get_str*/


/***********************************************************************/
Static Void get_double(Prompt, ret)
Char *Prompt;
double *ret;
{
  printf("%*s", strlen(Prompt), Prompt);
  scanf("%lg%*[^\n]", ret);
  if (getchar()==EOF) exit(0);
}  /*get_longreal*/


#define JMAX 9000

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
}

#undef JMAX

double findk(k)
   double k;
{
   double retval;

   r=sqrt(((k*b/s)-1.0)/((b/(k*s))-1.0));
   /*printf("r: %9.9f, k: %9.9f, ",r,k);*/
   retval=(1.0/Pi)*(log((1.0+r)/(1.0-r))-((s/b)*log((1.0+(r/k))/(1.0-(r/k)))));
   /*printf("gak: %9.9f, ",retval);*/
   retval= retval-(w/b);
   /*printf("w/b: %9.9f\n",w/b);*/

   return(retval);
}



main(argc,argv)
int argc;
char *argv[];
{
   double coupling,Zo,kprime,R3,k;
   int another;
   
   printf("Stripline broadside coupled line calculator.\n");
   printf("Copyright (C) 1999 M. Ferrara\n");
   do {
      get_double("Input Er: ",&er);
      get_double("Input gnd-gnd distance b: ",&b);
      get_double("Input strip width w: ", &w);
      get_double("Input strip spacing s: ", &s);
      
      k=rtbis(findk,0.6,1.0,1e-12);
      /*      printf("k: %9.9f\n",k);*/
      kprime=sqrt(1 - k*k);
      Zoo=296.1/(sqrt(er)*b*atanh(k)/s);
      Zoe=(No/sqrt(er))*ELIP1(kprime)/ELIP1(k);
      
      Zo=sqrt(Zoo*Zoe);
      coupling=10*log((Zoe-Zoo)/(Zoe+Zoo));
      
      fprintf(stdout, "Zoo = %g ohms, Zoe = %g ohms\n",Zoo,Zoe);
      fprintf(stdout, "Zo = %g ohms, Coupling = %g dB\n",Zo,coupling);
      
      R3=(2*Zoe*Zoo)/(Zoe-Zoo);
      fprintf(stdout, "Terminate with 2 %g ohm r's to gnd, and 1 %g ohm resistor from line to line\n",Zoe,R3);
      fprintf(stdout, "\n");
   } while(1);
 }









