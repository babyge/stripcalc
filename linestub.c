/*   Distributed line and stub bandpass filter calculator
     Copyright (c) 1986,1991 M. Ferrara
     Ref. -  MTT-29 No.3 March 1981 pp 215-222
             MYJ Table 10.03-1 p 597
*/

#include <p2c/p2c.h>
#include <stdio.h>
#ifndef MATHLIB_H
#include "mathlib.h"
#endif


#define v_light         2.99792458e+8   /*m/s*/
#define Eo              8.854187818e-12   /*F/m*/

#define isodd(x) (((int)((n)/2))*2 != n)
#define TRUE (1)
#define FALSE (0)


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

/***********************************************************************/
Static Void get_int(Prompt, ret)
Char *Prompt;
double *ret;
{
  printf("%*s", strlen(Prompt), Prompt);
  scanf("%ld%*[^\n]", ret);
  if (getchar()==EOF) exit(0);
}  /*get_longreal*/



main(argc,argv)
int argc;
char *argv[];

{
   int k,m,n;
   double h, w, fl, fh, Zs, g[50], f0, Z[50], ZprimeS[50];
   double Zo, theta, Zstub[50];    
   double Yo, YstbN, Ystb, YlinN, Ylin;
   
   printf("Distributed Line&Stub Bandpass Filter calculator.\n");
   printf("Copyright (C) 1986,1991 M. Ferrara\n");
   get_int("Input filter order: ",&n);
   g[0]=1.0;
   for (k=1;k<=n;k++) {
       printf("   g[%d]: ",k);
       get_double("",&g[k]);
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
   








