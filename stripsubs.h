/*
$Header$

*/
#ifndef STRIPSUBS_H
#define STRIPSUBS_H


#ifndef MATHLIB_H
#include "mathlib.h"
#endif

#include <math.h>

#ifdef STRIPSUBS_G
# define GLOBAL
#else
# define GLOBAL extern
#endif

typedef enum {
  Microstrip, BuriedMicrostrip, GCPW, ACPW, CPW, Slotline, Stripline, OffsetStripline
} linetype;


GLOBAL void Strip_Z(linetype Strip_type, double *dimension, unsigned char finite, double *result);

GLOBAL void CPW_Z(double h, double Er, double w_strip,
		      double d_to_gplanes, double gplane_width, unsigned char finite,
		      double *Zo, double *Eeff);
GLOBAL void ACPW_Z(double h, double Er, double b, double d1, double d2,
		       double *Zo, double *Eeff);
GLOBAL void GCPW_Z(double h, double Er, double w, double d, double *Zo,
		       double *Eeff);
GLOBAL void MSTP_Z(double h, double Er, double w, double t, double *Zo,
		       double *Eeff);

GLOBAL void BMSTP_Z(double h, double Er, double w, double b, double t, double *Zo,
		       double *Eeff);


GLOBAL void SLT_Z(double h, double Er, double w, double f, double *Zo,
		      double *Eeff, int *Error_ret);

GLOBAL void STPLN_Z(double b, double Er, double w, double *Zo, double *Eeff);

GLOBAL void FTSTPLN_Z(double b, double Er, double w, double t, double *Zo, double *Eeff);

GLOBAL void OSTPLN_Z(double b, double Er, double w, double h, double t, double *Zo, double *Eeff);


typedef enum {
  backward, forward
} calctype;

typedef double input_vector[20];
typedef double output_vector[5];

#ifndef true
# define true    1
# define false   0
#endif

#ifndef TRUE
# define TRUE    1
# define FALSE   0
#endif


GLOBAL linetype current_line_type;
GLOBAL double w, d1, d2, d, Zo, height, Er, gw, Eeff, f, b, t;
GLOBAL char Ans[133];
GLOBAL unsigned char Finite_calc, OK;
GLOBAL char Letter_selection;
GLOBAL calctype calculation;
GLOBAL int target_parameter;
GLOBAL double target_value;
GLOBAL int variable_parameter;
GLOBAL input_vector dim_vector;
GLOBAL output_vector result_vector;
GLOBAL unsigned char loop, debug_mode;
GLOBAL int Calc_error;


#undef GLOBAL

#endif /*STRIPSUBS_H*/

/* End. */
