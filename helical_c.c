/* Dimensions of optimal helical resonator for circular
   cross section. Dimensions are in cm. From "Filters with
   Helical and Folded Helical Resonators"  Peter Vizmuller,
   Artech House, 1987 ISBN: 0-89006-244-7 */

/***********************************************************************/

#define NULL ((char *)0)
#include <stdio.h>
#include <math.h>

double wire_guage[40] = {
8.252, 6.950, 6.185, 5.508, 4.905, 4.358, 3.890, 3.465, 3.085, 2.747,
2.446, 2.177, 1.940, 1.730, 1.540, 1.370, 1.220, 1.085, .9660, .8625,
.7685, .6835, .6085, .5425, .4830, .4295, .3825, .3405, .3035, .2705,
.2400, .2145, .1915, .1700, .1510, .1345, .1205, .1080, .09545, .08380  
};

static void get_str(char *Prompt, char *ret)
{
  char *TEMP;

  printf("%*s", strlen(Prompt), Prompt);
  fgets(ret, 133, stdin);
  TEMP = strchr(ret, "\n");
  if (TEMP != NULL)
    *TEMP = 0;
}  /*get_str*/


/***********************************************************************/
static void get_double(char *Prompt, double *ret)
{
  printf("%*s", strlen(Prompt), Prompt);
  scanf("%lg%*[^\n]", ret);
  if (getchar()==EOF) exit(0);
}  /*get_longreal*/

/***********************************************************************/
static void get_int(char *Prompt, long *ret)
{
  printf("%*s", strlen(Prompt), Prompt);
  scanf("%ld%*[^\n]", ret);
  if (getchar()==EOF) exit(0);
}  /*get_longreal*/


int guage(double g)
{
   int j;
   g=10*g;

   for (j=40; j>=0; j--) {
      if ( g < wire_guage[j] ) return(j+1);
   }
}

main(int argc, char **argv) 
{
   double S,Q,F,Turns,Pitch,Zed,D,D_,H,G,Er,Df;
   char yn[255];
   
   Er=1.0;
   Df=0.0;
   
   printf("Program calculates dimensions of optimal helical resonator\n");
   printf("of circular cross section.\n\n");

   get_double("Enter inner diameter of shield, D [cm]: ", &D);
   get_double("Enter frequency in MHz: ",&F);

   S=D/1.2;
   Q=23.6*S*sqrt(F);
   Turns=4064/(F*S);
   Pitch=4064/(S*S*F);
   Zed=207010/(F*S);
   D_=0.66*S;
   H=1.592*S;
   G=1.0/(2*Pitch);

 REPRINT:
   Zed=Zed/sqrt(Er);

   printf("Frequency = %g MHz\n",F);
   printf("Unloaded Q = %g\n",Q);
   printf("Resonator Height = %g cm\n", H);
   printf("Mean helix diameter d = %g cm\n", D_);
   printf("Pitch = %g turns/cm\n",Pitch);
   printf("Number of turns = %g\n",Turns);
   printf("Characteristic impedance = %g ohms\n",Zed);
   printf("Wire diameter = %g cm\n",G);
   printf("Nearest wire guage is %d AWG\n",guage(G));

   get_str("Calculate with dielectric?",yn);
   if ( *yn == 'y' || *yn == 'Y' ) {
         get_double("Enter Dielectric Er: ", &Er);
	 get_double("Enter Dielectric loss fact: ", &Df);
	 F=F/sqrt(Er);
	 Q=1/(1/Q + Df);
	 goto REPRINT;
   }
}
