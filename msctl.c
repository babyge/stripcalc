/*   Microstrip coupled line calculator
     Copyright (c) 1986,1991 M. Ferrara
*/

#include <p2c/p2c.h>
#include <stdio.h>
#ifndef MATHLIB_H
#include "mathlib.h"
#endif


#define v_light         2.99792458e+8   /*m/s*/
#define Eo              8.854187818e-12   /*F/m*/

double Zoo,Zoe,Z0,c,whso,whse,P,er,r,H,F,G;
double wh,sh,eeff,ht;

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
}

#undef JMAX

double numer(gparam)
   double(gparam);
{
   return((M_2_PI*acosh(((gparam+1)*F-2)/(gparam-1))
    +r*acosh(acosh(((gparam+1)*F+(gparam-1))/2)/acosh(gparam)))-whso);
 }



main(argc,argv)
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









