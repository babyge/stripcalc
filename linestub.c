/*   Distributed line and stub bandpass filter calculator
     Copyright (c) 1986,1991 M. Ferrara
     Ref. -  MTT-29 No.3 March 1981 pp 215-222
             MYJ Table 10.03-1 p 597

$Header$

$Log$
Revision 1.3  2003/04/18 21:31:56  mikef
*** empty log message ***

Revision 1.4  2002/12/05 23:41:13  mikef
*** empty log message ***

Revision 1.3  2002/12/05 22:17:58  mikef
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

#include "get_input.h"


int main(argc,argv)
int argc;
char *argv[];

{
   int k,n;
   double h, w, fl, fh, g[50], f0, Z[50], ZprimeS[50];
   double Zo, theta, Zstub[50];    
   double Yo, YstbN, Ystb, YlinN, Ylin;
   char prompt[80];
   
   printf("Distributed Line&Stub Bandpass Filter calculator.\n");
   printf("Copyright (C) 1986,1991 M. Ferrara\n");
   get_int("Input filter order: ",&n);
   g[0]=1.0;
   for (k=1;k<=n;k++) {
       sprintf(prompt,"   g[%d]: ",k);
       get_double(prompt,&g[k]);
     }

   while (1) {
      get_double("Input lower edge of passband: ",&fl);
      if (fl == 0) exit(0);
      get_double("Input upper edge of passband: ",&fh);
      if (fh == 0) exit(0);
      get_double("Input de-normalized impedance level (Zo): ",&Zo);
      if (Zo == 0) exit(0);

      Yo=1/Zo;
      f0=(fl+fh)/2;
      w = 2*(fh-fl)/(fh+fl);
      theta = M_PI_2*(1-(w/2));
      h = g[1]*g[1]*tan(theta)*tan(theta);
   
      Z[1]=sqrt(2*g[1]/g[2]);
      Z[n-1]=sqrt(2*g[1]/g[n-1]);
      for (k=2;k<n-1;k++) {
          Z[k]=2*g[1]/sqrt(g[k]*g[k+1]);
          }      
      for (k=1;k<n;k++) {
          ZprimeS[k]=sqrt(h+(Z[k]*Z[k]))-Z[k];
	}
      Zstub[1]=ZprimeS[1];
      Zstub[n]=ZprimeS[n-1];
      for (k=2;k<n;k++) {
          Zstub[k]=ZprimeS[k-1]+ZprimeS[k];
	}


      printf("Fractional BW: %.5g \n",w);
      for (k=1;k<n;k++) {
         printf ("Series open stub  Z = %.5g\n",Zstub[k]*Zo );
         printf ("Unit Element      Z = %.5g\n",Z[k]*Zo );
       }
      printf ("Series open stub  Z = %.5g\n",Zstub[n]*Zo );
      printf ("\nDual Circuit:\n");
      for (k=1;k<n;k++) {
         /*I could have said that Zstb=Zo*Zo/Zstub[k]
           but that wouldn't have been nearly as clear now, would it? */
         YstbN=1/Zstub[k];
         Ystb=Yo/YstbN;
         YlinN=1/Z[k];
         Ylin=Yo/YlinN;         
         printf ("Shunt shorted stub  Z = %.5g\n",1/Ystb );
         printf ("Unit Element      Z = %.5g\n",1/Ylin );
       }
       YstbN=1/Zstub[k];
       Ystb=Yo/YstbN;
       printf ("Shunt shorted stub  Z = %.5g\n",1/Ystb );
   }  
 }
   








