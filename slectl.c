/*   Stripline edge coupled line calculator
     Copyright (c) 1999 M. Ferrara

$Header$

$Log$
Revision 1.2  2003/04/18 21:31:57  mikef
*** empty log message ***

Revision 1.6  2002/12/05 23:41:13  mikef
*** empty log message ***

Revision 1.5  2002/12/05 22:49:00  mikef
*** empty log message ***

Revision 1.4  2002/12/05 22:17:58  mikef
Remove p2c cruft

Revision 1.3  2002/09/12 03:19:27  mikef
*** empty log message ***

Revision 1.4  2002/07/08 20:17:06  mikef
Fix for typo in Waddell's book

Revision 1.3  2002/07/08 01:06:49  mikef
Add slectl with finite strip thickness

Revision 1.2  2002/07/07 23:16:37  mikef
Incremental checkin

Revision 1.1  2002/07/07 21:35:22  mikef
Initial import


*/
#include <stdio.h>
#ifndef MATHLIB_H
#include "mathlib.h"
#endif

#include "stripsubs.h"


double Cf0(double er);
double Cf(double er, double t, double b);
void zeroTZ(double er, double b, double w, double s, double *Zoo, double *Zoe);
void calcZ(double er, double b, double w, double s, double t, double *Zoo, double *Zoe);

#include "get_input.h"

int main(argc,argv)
int argc;
char *argv[];
{
   double coupling, er, b, w, s, t;
   double Zoo,Zoe, R3, Zo;

   printf("Stripline edge coupled line calculator.\n");
   printf("Copyright (C) 2001 M. Ferrara\n");

   do {
      get_double("Input Er: ", &er);
      get_double("Input gnd-gnd distance b: ",&b);
      get_double("Input strip width w: ", &w);
      get_double("Input strip spacing s: ", &s);
      get_double("Input strip thickness t: ", &t);

      if (t == 0) {
	 zeroTZ(er,b,w,s,&Zoo,&Zoe);
      } else {
	 calcZ(er,b,w,s,t,&Zoo,&Zoe);
      }

      Zo=sqrt(Zoo*Zoe);
      coupling=10*log((Zoe-Zoo)/(Zoe+Zoo));
      
      fprintf(stdout, "Zoo = %g ohms, Zoe = %g ohms\n",Zoo,Zoe);
      fprintf(stdout, "Zdiff = %.5g, Zcm = %.5g\n",2*Zoo,Zoe/2);
      fprintf(stdout, "Zo = %g ohms, Coupling = %g dB\n",Zo,coupling);
      
      R3=(2*Zoe*Zoo)/(Zoe-Zoo);
      fprintf(stdout, "Terminate with 2 %g ohm r's to gnd, and 1 %g ohm resistor from line to line\n",Zoe,R3);
      fprintf(stdout, "\n");


   } while(1);
}

/*Calculate Zoe and Zoo for the zero thickness case*/
void zeroTZ(double er, double b, double w, double s, double *Zoo, double *Zoe) 
{
   double ke,keprime, ko,koprime;

   ke=tanh(Pi * w/(2*b)) * tanh(Pi*(w+s)/(2*b));
   keprime=sqrt(1-(ke*ke));

   ko=tanh(Pi * w/(2*b)) * coth(Pi*(w+s)/(2*b));
   koprime=sqrt(1-(ko*ko));

   *Zoe=(30*Pi/sqrt(er)) * ELIP1(keprime)/ELIP1(ke);
   *Zoo=(30*Pi/sqrt(er)) * ELIP1(koprime)/ELIP1(ko);
}
		       


void calcZ(double er, double b, double w, double s, double t, double *Zoo, double *Zoe) 
{
   double Zowt, Zow, Zooz, Zoez, Eeff;
   FTSTPLN_Z(b,er,w,t,&Zowt,&Eeff);
   STPLN_Z(b,er,w,&Zow,&Eeff);
   zeroTZ(er,b,w,s,&Zooz,&Zoez);

   *Zoe = 1.0/((1.0/Zowt) - ((Cf(er,t,b)/Cf0(er))*((1.0/Zow)-(1.0/Zoez))));
   if ( (s/t) >= 5.0 ) {
      *Zoo=1.0/((1.0/Zowt) - ((Cf(er,t,b)/Cf0(er)) * ((1.0/Zow) - (1.0/Zooz))));
   } else {
      *Zoo=(1.0/Zooz) + ((1.0/Zooz)-(1.0/Zow)) -
	 ((2.0/No)*((Cf(er,t,b)/(er*Eo))-(Cf0(er)/(er*Eo)))) + 2.0/(No * s);
      *Zoo = 1.0/(*Zoo);
   }
}

double Cf0(double er)
{
   return((0.0885*er/Pi)*2.0*log(2.0));
}

double Cf(double er, double t, double b)
{
   double tb, tmp, tmp2;

   tb=t/b;
   tmp = log((1.0/pow(1.0-tb,2)) - 1.0) * ((1.0/(1.0-tb)) - 1.0);
   tmp2 = log((1.0/(1.0-tb))+1.0) * (2.0) / (1.0-tb);
   return((0.0885*er/Pi) * (tmp2 - tmp));
}
