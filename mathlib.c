/*
  Math library for microstrip and coplanar waveguide calculations

  Changes:
         03/11/86    MRF    Author

$Header$

*/


#define MATHLIB_G
#include "mathlib.h"


/* ******************************************************************** */
/* * Hyperbolic Functions                                             * */
/* ******************************************************************** */

inline double coth(double x)
{
  return (1/tanh(x));
}


/* ******************************************************************** */
inline double sech(double x)
{
  return (2 / (exp(x) + exp(-x)));
}


/* ******************************************************************** */
inline double csch(double x)
{
  return (2 / (exp(x) - exp(-x)));
}
/* ******************************************************************** */

inline double power(double x, double y)
{
  if (x == 0) return (0);
  return pow(x,y);
}
