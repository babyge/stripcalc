/*   Chebychev filter parameters

$Header$

$Log$
Revision 1.3  2003/04/18 21:31:56  mikef
*** empty log message ***

Revision 1.5  2002/12/05 23:41:13  mikef
*** empty log message ***

Revision 1.4  2002/12/05 22:49:00  mikef
*** empty log message ***

Revision 1.3  2002/12/05 22:17:57  mikef
Remove p2c cruft

Revision 1.2  2000/06/21 18:05:48  mikef
*** empty log message ***

Revision 1.3  2000/02/24 00:00:09  mikef
compilation cleanup

Revision 1.2  1999/12/13 22:51:13  mikef
Add header and log keywords to all files



*/

#include <stdio.h>
#ifndef MATHLIB_H
#include "mathlib.h"
#endif


#define isodd(x) (((int)((n)/2))*2 != n)

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

/***********************************************************************/

#include "get_input.h"

int main(argc,argv)
int argc;
char *argv[];

{
   int R;
   double rd, at, rl, ap, gamma, beta, delta, a[50], b[50], g[50], n;
   double eps, am, ah, wh;    
   unsigned char cheby = FALSE;
   
   printf("Chebyshev Filter calculator.\n");
   printf("Copyright (C) 1986,1991 M. Ferrara\n");
   get_double("Input passband ripple (dB): ",&am);
   get_double("Input filter order: ",&n);
   get_double("The input impedance is 1 ohm, input the output impedance: ",&rl); 

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
      ah=isodd(n) ? 2.0 : 2.0*ap;
      wh=cosh((1/n)*acosh(sqrt(ah-1)/eps)); /*3dB bandwidth*/
    }
   if (cheby) {
      at = isodd(n) ? 4.0*rl/((rl+1)*(rl+1)) : 4.0*rl*ap/((rl+1)*(rl+1));
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
 }
   








