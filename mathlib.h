/*
$Header$

*/
#ifndef MATHLIB_H
#define MATHLIB_H


#include <math.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_sf_ellint.h>
#include <gsl/gsl_const_mks.h>

#ifdef MATHLIB_G
# define extern
#else
# define extern extern
#endif

/*elliptic integral of the first kind*/
#define ELIP1(k) gsl_sf_ellint_Kcomp((k),GSL_PREC_DOUBLE)

/*elliptic integral of the second kind*/
#define ELIP2(k) gsl_sf_ellint_Ecomp((k),GSL_PREC_DOUBLE)

extern double coth(double x);
extern double sech(double x);
extern double csch(double x);

extern double power(double x, double y);

#define min(a,b) ((a)<=(b))?(a):(b)
#define max(a,b) ((a)>=(b))?(a):(b)




#define v_light         GSL_CONST_MKS_SPEED_OF_LIGHT   /*m/s*/
#define Eo              GSL_CONST_MKS_VACUUM_PERMITTIVITY   /*F/m*/
#define MAXERR          0.0001
#define Pi              M_PI
#define No              (120*Pi) /* ohms */
#define E               M_E

#undef extern

#endif /*MATHLIB_H*/

/* End. */
