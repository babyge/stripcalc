/*   Microstrip interdigital bandpass filter calculator
     Copyright (c) 1986,1991 M. Ferrara
     Ref. -  Microwave Journal March 1989 pp. 147-152
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
   int k,m;
   double theta1, h, w, fl, fh, Zs, g[50], f0, J[50], Y[50], n;
   double R, Zt, Cs, thetat, Zoo[50], Zoe[50], Zooprime, Zoeprime;    
   
   printf("Microstrip Interdigital Filter calculator.\n");
   printf("Copyright (C) 1986,1991 M. Ferrara\n");
   get_double("Input filter order: ",&n);
   g[0]=1.0;
   for (k=1;k<=n;k++) {
       printf("   g[%d]: ",k);
       get_double("",&g[k]);
     }

   while (1) {
      get_double("Input lower edge of passband (GHz): ",&fl);
      if (fl == 0) exit(0);
      get_double("Input upper edge of passband (GHz): ",&fh);
      if (fh == 0) exit(0);
      get_double("Input single strip impedance (Zs) : ",&Zs);
      if (Zs == 0) exit(0);
      get_double("Input source and load impedance   : ",&R);
      if (R == 0) exit(0);   

      f0=(fl+fh)/2;
      w = 2*(fh-fl)/(fh+fl);
      theta1 = M_PI_2*(1-(w/2));
      h = 1/(Zs*tan(theta1));
   
      for (k=1;k<n;k++) {
          J[k]=h/sqrt(g[k+1]*g[k]);
          Y[k]=J[k]*sin(theta1);
          }      
      thetat=(1/(1-(w/2)))*asin(sqrt(R*h*sin(theta1)*sin(theta1)/(g[0]*g[1])));
      Zoe[1]=Zs/(1-Y[1]*Zs);
      Zoo[1]=Zs/(1+Y[1]*Zs);
      m=(int)(n-1);
      Zoe[m]=Zs/(1-Y[m]*Zs);
      Zoo[m]=Zs/(1+Y[m]*Zs);
      for (k=2;k<(n-1);k++) {
         Zoe[k]=1/((2/Zs)-(1/Zoe[k-1])-Y[k]-Y[k-1]);
         Zoo[k]=1/(2*Y[k]+(1/Zoe[k]));
       }
      Zt=Zs/(1-(Y[1]*Y[1]*Zs*Zs));
      Cs=Zt*cos(thetat)*sin(thetat)*sin(thetat)*sin(thetat)/
         (2*M_PI*f0*1.0E9*(R*R+Zt*Zt*cos(thetat)*cos(thetat)*sin(thetat)*sin(thetat)));   
      printf("\nFl (GHz): %.5g, Fh (GHz): %.5g \n",fl,fh);
      printf("Fractional BW: %.5g \n",w);
      printf("Cs (pF): %.5g\n",Cs*1.0e12);
      printf("Tap : %.5g (rad) = %.5g (deg)\n",thetat,180*thetat/M_PI);
      for (k=1;k<n;k++) {
         printf ("Strips %d,%d   Zoe: %.5g   Zoo: %.5g \n",k,k+1,Zoe[k],Zoo[k]);
       }
   
      
      printf("To model, add a parallel strip to each end of impedance %.5g ohms.\n",2*Zs);
      printf("The loading capacitors still get connected to the coupled lines.\n");
      printf("Replace each coupled pair with separate coupled lines with the\n");
      printf("following even and odd mode impedances:\n");
      printf("You'll need to split each coupled line pair into two shorter\n");
      printf("simulate the tap.\n");
      for (k=1;k<n;k++) {
         Zoeprime=1/((1/Zoe[k])-(1/(2*Zs)));
         Zooprime=1/((1/Zoo[k])-(1/(2*Zs)));
         printf ("Ctl %d: Zoe: %.5g   Zoo: %.5g \n",k,Zoeprime,Zooprime);
       }
   }  
 }
   








