/*   Series resonant Coupled-resonator BPF ... see Dec 1992 Microwaves&RF
     Copyright (c) 1997 M. Ferrara

     Canonical form:
     
|----|----Lr--C1--|----Lr--C2--|---Lr--CN-|--------|
Y1  C01         C12          C2N        CN,N+1    YN
|----|------------|------------|----------|--------|


     Pi Transform:


$Header$

$Log$
Revision 1.3  2003/04/18 21:31:57  mikef
*** empty log message ***

Revision 1.6  2002/12/06 01:17:47  mikef
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

/*#define is_odd(x) (((int)x % 2)==1)*/
#define is_odd(x) (GSL_IS_ODD((int) x))

#define TRUE (1)
#define FALSE (0)


#include "get_input.h"

int main(argc,argv)
int argc;
char *argv[];

{
   int R;
   double rd, at, rl, ap, gamma, beta, delta, a[50], b[50], g[50], n;
   double ga, gb, f1, f2, f0, bf, l;
   double w0, cr, c[50][50], cein, ceout;
   double zin, zout;
   double eps, am, ah, wh, wf;    
   unsigned char cheby = FALSE;
   
   printf("Coupled (Series) Resonator BPF calculator.\n");
   printf("Copyright (C) 1997 M. Ferrara\n");
   get_double("Input passband ripple (dB): ",&am);
   get_double("Input filter order: ",&n);
   get_double("The input impedance:",&zin);
   get_double("The output impedance: ",&zout);
   get_double("Input Lower edge freq: ",&f1);
   get_double("Input Upper edge freq: ",&f2);
   get_double("Input resonator inductance: ",&l);

   ga=1/zin;
   gb=1/zout;
   rl=zout/zin;


   /*  **** calculate the MYJ g parameters */
   g[0]=1.0;
   g[(int)n+1]=1/rl;

   if (am==0) {
      cheby=FALSE;
    }
   else {
      cheby=TRUE;
      ap=power(10.0,am/10.0);
      eps=sqrt(ap-1.0);
      beta=2.0*asinh(1/eps);
      ah=(is_odd(n)) ? 2.0 : 2.0*ap;
      wh=cosh((1/n)*acosh(sqrt(ah-1)/eps)); /*3dB bandwidth*/
    }
   if (cheby) {
      at = (is_odd(n)) ? 4.0*rl/((rl+1)*(rl+1)) : 4.0*rl*ap/((rl+1)*(rl+1));
    }
   else at = 4.0*rl/((rl+1)*(rl+1));

   if (cheby) delta = sinh((1.0/n)*asinh(sqrt(1.0-at)/eps));
   else delta = power((1.0-at),(1.0/(2.0*n)));

   gamma = (cheby) ? sinh(beta/(2*n)) : 1.0;

   for (R=1;R<=n;R++) {
     rd=(double) R;
     b[R] = cheby ? gamma*gamma + delta*delta - 2*gamma*delta*cos(rd*M_PI/n) +
                    sin(rd*M_PI/n)*sin(rd*M_PI/n) :
                    gamma*gamma + delta*delta - 2*gamma*delta*cos(rd*M_PI/n);

     a[R] = sin((2*rd-1)*M_PI/(2*n));
   }
   g[1]= 2*a[1]/(gamma-delta);
   for (R=2;R<=n;R++) {
      g[R] = 4.0*a[R]*a[R-1]/(b[R-1]*g[R-1]);
      }

   if (cheby) {
      printf("3dB bandwidth = %.5g\n",wh);
      printf("Insertion loss = %.3g dB\n",10.0*log10(at));
    }
   for (R=0;R<=(n+1);R++) {
      printf("g[%d]=%.7g\n",R,g[R]);
    }
   
   /* *** Now the bandpass transform, and de-norm **** */
   f0=sqrt(f2*f1);
   bf=(f2-f1)/f0;
   if (bf > 0.05) { /* wideband transformation */
      f0=f1+f2-sqrt(((f2-f1)*(f2-f1)) + (f1*f2));
      bf=f0/f1 - f0/f2;
   }
   w0= 2*M_PI*f0;
   wf= 2*M_PI*bf;
   cr= 1/(l*w0*w0);

   /* Now cap values */
   c[0][1]=(1/w0)*sqrt((g[1]*ga/(wf*l))-(ga*ga));
   for (R=1; R<n; R++) {
      c[R][R+1]= (1/w0)*(sqrt(g[R]*g[R+1])/(wf*l)-(gb*gb));
   }
   R=(int)n;
   c[R][R+1]= (1/w0)*(sqrt(g[R]*gb)/(wf*l)-(gb*gb));

   /* Now effective end caps */
   cein=c[0][1]/(1 + ((w0*c[0][1]/ga)*(w0*c[0][1]/ga)) );
   c[1][1]=cr-cein-c[1][2];
   for (R=2; R<n; R++) {
      c[R][R]=cr-c[R-1][R]-c[R][R+1];
   }
   R=(int)n;
   ceout=c[R][R+1]/(1 + ((w0*c[R][R+1]/gb)*(w0*c[R][R+1]/gb)) );
   c[R][R]=cr-c[R-1][R]-ceout;

   /* Done ... print the result */
   /*C[j][j] is the tank cap, C[j-1][j] is the coupling cap */
   printf("%dth order BPF, with edges: %g and %g\n",R,f1,f2);
   printf("Resonator inductor: %g \n",l);
   for (R=0; R<=(n); R++) {
      if (R>0) {
	 printf("C[%d][%d] = %g\n",R,R,c[R][R]);
      }
      printf("C[%d][%d] = %g\n",R,R+1,c[R][R+1]);
   }
 }
   








