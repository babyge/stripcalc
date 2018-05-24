/* Calculate necessary params for a helical filter. Most eqns are
   from Zverev p 500. 

   Mike Ferrara 8/12/97
   */

/*

$Header$

$Log$
Revision 1.4  2003/04/18 21:31:56  mikef
*** empty log message ***

Revision 1.5  2002/12/05 23:41:13  mikef
*** empty log message ***

Revision 1.4  2002/12/05 22:17:58  mikef
Remove p2c cruft

Revision 1.3  2000/06/21 18:05:48  mikef
*** empty log message ***

Revision 1.3  1999/12/13 22:51:13  mikef
Add header and log keywords to all files

*/


#define NULL ((char *)0)
#include <stdio.h>
#include <math.h>

double wire_guage[40] = {
8.252, 6.950, 6.185, 5.508, 4.905, 4.358, 3.890, 3.465, 3.085, 2.747,
2.446, 2.177, 1.940, 1.730, 1.540, 1.370, 1.220, 1.085, .9660, .8625,
.7685, .6835, .6085, .5425, .4830, .4295, .3825, .3405, .3035, .2705,
.2400, .2145, .1915, .1700, .1510, .1345, .1205, .1080, .09545, .08380  
};

#include "get_input.h"

int guage(double g)
{
   int j;
   g=10*g;

   for (j=40; j>=0; j--) {
      if ( g < wire_guage[j] ) return(j+1);
   }
   return(-1);
}

int main(int argc, char **argv)
{
   double d,D,S,b,H,N,n,Zo,Qfilt,Qu,f0,bw;
   double q0,q1,qn,k[50],hd[50],thick;
   double Qd,RbZo,theta,tapin,tapout;
   int square=0,sect=1,j;
   char prompt[255];


   printf("Helical resonator filter design.\n");
   printf("copyright Mike Ferrara (c) 1997.\n\n");
   printf("Dimensions are in inches and MHz.\n");
 
   square=get_yn("Design square cross section resonators?(Y/N)");
   if ( !square ) {
      printf("OK, designing circular cross section resonators.\n");
   }

   get_double("Filter CF (MHz):",&f0);
   get_double("Filter 3dB BW (MHz):",&bw);
   get_int("Number of sections:", &sect);

   /* Calculate Q of filter */
   Qfilt=f0/bw;

   if (sect > 1) {
      get_double("Input q0:",&q0);
      get_double("Input q1:",&q1);
      get_double("Input qn:",&qn);
      for (j=1; j<sect; j++) {
	  sprintf(prompt,"Input k[%d][%d]:",j,j+1);
	  get_double(prompt,&k[j]);
	  /* Scale the kn,n+1 values */
	  k[j] = k[j]/Qfilt;
      }
   }
   
   get_double("Mean helix diameter (inches):",&d);
   if (square) {
      get_double("Resonator (inside) side length (inches):",&S);
      D=1.20*S;
      get_double("Wall material thickness (inches):",&thick);
   } else {
      get_double("Resonator inside diameter (inches):",&D);
   }
   b=1.5*d;
   H=(D/2)+b;
   N=1720/(f0*d) * sqrt( log10(D/d)/ (1 - (d*d/(D*D))));

   n=N/b;
   Zo=183*n*d*sqrt( (1-(d/D))*(1-(d/D))*log10(D/d));
   Qu=220*D*sqrt(f0)*( ((d/D) - pow((d/D),3.0))/( 1.5 + pow((d/D),3.0)));

   /*Input tap calc*/
   Qd = (q1/2) * Qfilt;
   RbZo= M_PI_4 * ( 1/Qd - 1/Qu );
   theta=asin(sqrt(RbZo * 50/(2*Zo)));
   tapin=N*theta/M_PI_2;

   /*Output tap calc*/
   Qd = (qn/2) * Qfilt;
   RbZo= M_PI_4 * ( 1/Qd - 1/Qu );
   theta=asin(sqrt(RbZo * 50/(2*Zo)));
   tapout=N*theta/M_PI_2;

   /* Coupling h/d calculation */
   for (j=1; j<sect; j++) {
      hd[j]=pow((k[j]/0.071),(1/1.91));
   }
   
   printf("\nFilter center freq: %g MHz\n",f0);
   printf("Filter 3dB BW: %g MHz\n",bw);
   printf("Helix mean diameter (d): %g in\n",d);
   if (square) {
      printf("Square cavity side dimension (S): %g in\n",S);
   }
   else {
      printf("Circular cavity diameter (D): %g in\n",D);
   }
   printf("Cavity height (H): %g in\n",H);
   printf("Helix axial length (b): %g in\n",b);
   printf("Helix has %g turns, for %g turns/inch\n",N,n);
   printf("Helix short placed %g in. from bottom\n",(D/4));
   printf("Resonator Zo %g ohms\n",Zo);
   printf("Unloaded Q (Qu): %g ... be sure to sanity check your k, and q values!\n",Qu);
   printf("50ohm input tap: %g turns from shorted end\n",tapin);
   printf("50ohm output tap: %g turns from shorted end\n",tapout);
   printf("Coupling apertures (h is axial length of exposed helix):\n");
   for (j=1; j<sect; j++) {
      printf("   H:d%d%d = %g, h%d%d = %g in\n",j,j+1,hd[j],j,j+1,hd[j]*d);
   }

   if (square) {
      printf("\nCut list: \n");
      printf("2 end walls: %g X %g in.\n", S+(2*thick),H);
      printf("2 (front and back) walls %g X %g in.\n",(S*sect)+(thick*(sect-1)),H);
      printf("2 (bottom and top) walls %g X %g in.\n",(S*sect)+(thick*(sect+1)),H);
      printf("Coupling walls:\n");
      for (j=1; j<sect; j++) {
	 printf("   K%d%d wall: %g X %g in.\n",j,j+1,b-(hd[j]*d)+(D/4),S);
      }
   }
}
