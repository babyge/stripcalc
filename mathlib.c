/* Output from p2c, the Pascal-to-C translator */
/* From input file "mathlib.p" */


/*
  Math library for microstrip and coplanar waveguide calculations

  Changes:
         03/11/86    MRF    Author
*/

#include <p2c/p2c.h>


#define MATHLIB_G
#include "mathlib.h"


#define a0              1.3862944e+0
#define a1              0.1119723e+0
#define a2              0.0725296e+0
#define b0              0.5e+0
#define b1              0.1213478e+0
#define b2              0.0288729e+0
#define MaxArg          0.999999e+0


/* ******************************************************************** */
/* Elliptic integral of the first kind:                                 */
/* from: Handbook of Mathematical Functions pp591 eqn 17.3.34           */
/* ******************************************************************** */
double ELIP1(Ax)
double Ax;
{
  double K, Kp;

  K = Ax;
  Kp = 1 - K * K;
  if (K < MaxArg)
    return (((b2 * Kp + b1) * Kp + b0) * log(1 / Kp) + (a2 * Kp + a1) * Kp + a0);
  else
    return (0.5 * log(16 / Kp));
}

#undef a0
#undef a1
#undef a2
#undef b0
#undef b1
#undef b2
#undef MaxArg


#define a1              0.44479204e+0
#define a2              0.085099193e+0
#define a3              0.040905094e+0
#define b1              0.24969795e+0
#define b2              0.08150224e+0
#define b3              0.01382999e+0


/* ******************************************************************** */
/* Elliptic Integral of the Second kind                                 */
/* from: Handbook of Mathematical Functions                             */
/* ******************************************************************** */
double ELIP2(k)
double k;
{
  double t;

  t = 1 - k * k;
  return (((a3 * t + a2) * t + a1) * t + 1 - ((b3 * t + b2) * t + b1) * t * log(t));
}

#undef a1
#undef a2
#undef a3
#undef b1
#undef b2
#undef b3


/* ******************************************************************** */
/* * Hyperbolic Functions                                             * */
/* ******************************************************************** */

double coth(x)
double x;
{
  return (1/tanh(x));
}


/* ******************************************************************** */
double sech(x)
double x;
{
  return (2 / (exp(x) + exp(-x)));
}


/* ******************************************************************** */
double csch(x)
double x;
{
  return (2 / (exp(x) - exp(-x)));
}
/* ******************************************************************** */
double asinh(x)
double x;
{
  return (log(x+sqrt((x*x)+1)));
}
/* ******************************************************************** */
double acosh(x)
double x;
{
  if (x >= 1) {
     return (log(x+sqrt((x*x)-1)));
   }
  else {
     fprintf(stderr,"acosh was given an argument less than 1\n");
     return(HUGE_VAL);
   }
}
/* ******************************************************************** */
double atanh(x)
double x;
{
  if ((x*x) < 1) {
     return (0.5*log((1+x)/(1-x)));
   }
  else {
     fprintf(stderr,"atanh was given an argument whose square is greater than 1\n");
     return(HUGE_VAL);
   }
}
/* ******************************************************************** */
double x_to_int(x, y)
double x;
int y;
{
  int i;
  double product;

  if (y >= 0) {
    product = 1.0;
    for (i = 1; i <= y; i++)
      product = x * product;
    return product;
  } else
    return (1 / x_to_int(x, -y));
}  /*function*/


/* ******************************************************************** */
double power(x, y)
double x, y;
{
  if (x == 0) return (0);
  if (y - (int)y == 0)
    return (x_to_int(x, (int)y));
  else
    return exp(y * log(x));
}  /*function*/


/*module*/




/* End. */
