/* Output from p2c, the Pascal-to-C translator */
/* From input file "stripsubs.p" */


/*generate symbol tables*/

#include <p2c/p2c.h>

#define STRIPSUBS_G
#include "stripsubs.h"


#define Pi              3.14159265358979232646e+00
#define v_light         3e+8   /*m/s*/
#define Eo              8.85e-12   /*F/m*/


typedef enum {
  Microstrip, GCPW, ACPW, CPW, Slotline
} linetype;
typedef enum {
  backward, forward
} calctype;
typedef double input_vector[10];
typedef double output_vector[5];


 linetype current_line_type;
 double w, d1, d2, d, Zo, height, Er, gw, Eeff, f;
 Char Ans[133];
 boolean Finite_calc, OK;
 Char Letter_selection;
 calctype calculation;
 int target_parameter;
 double target_value;
 int variable_parameter;
 input_vector dim_vector;
 output_vector result_vector;
 boolean loop, debug_mode;
 int Calc_error;


/***********************************************************************/
Void get_str(Prompt, ret)
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
Void get_longreal(Prompt, ret)
Char *Prompt;
double *ret;
{
  printf("%*s", strlen(Prompt), Prompt);
  scanf("%lg%*[^\n]", ret);
  getchar();
}  /*get_longreal*/


/***********************************************************************/
Void Strip_Z(Strip_type, dimension, finite, result)
linetype Strip_type;
double *dimension;
boolean finite;
double *result;
{
  double Er;   /*dimension[1]*/
  double h;   /*dimension[2]*/
  double w;   /*dimension[3]*/
  double d1;   /*dimension[4]*/
  double d2;   /*dimension[5]*/
  double d;   /*dimension[6]*/
  double sw;   /*dimension[7]*/
  double f;   /*dimension[8]*/
  int Calc_err;

  Er = dimension[0];
  h = dimension[1];
  w = dimension[2];
  switch (Strip_type) {

  case Microstrip:
    MSTP_Z(h, Er, w, &Zo, &Eeff);
    break;

  case GCPW:
    d = dimension[5];
    GCPW_Z(h, Er, w, d, &Zo, &Eeff);
    break;

  case ACPW:
    d1 = dimension[3];
    d2 = dimension[4];
    ACPW_Z(h, Er, w, d1, d2, &Zo, &Eeff);
    break;

  case CPW:
    d = dimension[5];
    sw = dimension[6];
    CPW_Z(h, Er, w, d, sw, finite, &Zo, &Eeff);
    break;

  case Slotline:
    f = dimension[7];
    SLT_Z(h, Er, w, f, &Zo, &Eeff, &Calc_err);
    if (Calc_err != 0)
      _Escape(1);
    break;
  }/*case*/
  result[0] = Zo;
  result[1] = Eeff;
}  /*procedure*/


#define MAXERR          0.0001


/***************************************************************************/
Void Newton(Strip_type, dimension, var_parm, finite, result,
		   target_parm, target_val)
linetype Strip_type;
double *dimension;
int var_parm;
boolean finite;
double *result;
int target_parm;
double target_val;
{
  double current_val, increment, slope, error;
  int iteration;

  iteration = 0;
  Strip_Z(Strip_type, dimension, finite, result);
  current_val = result[target_parm - 1];
  error = fabs(target_val - result[target_parm - 1]);
  while (error > MAXERR) {
    iteration++;
    /*calculate f'(x)*/
    increment = dimension[var_parm - 1] / 100;
    dimension[var_parm - 1] += increment;
    Strip_Z(Strip_type, dimension, finite, result);
    slope = (result[target_parm - 1] - current_val) / increment;
    /*x(n+1)=x(n)-f(x(n))/f'(x(n))*/
    dimension[var_parm - 1] += (target_val - current_val) / slope - increment;
    /*find new error*/
    Strip_Z(Strip_type, dimension, finite, result);
    current_val = result[target_parm - 1];
    error = fabs(target_val - current_val);
    if (debug_mode) {
      printf("Iteration=%3d\n", iteration);
      printf("Dimension=%6.3f Current val=%3.4f\n",
	     dimension[var_parm - 1], current_val);
      printf("Error=%3.5f\n\n", error);
    }
  }  /*loop*/
}  /*procedure*/

#undef MAXERR


/**********************************************************************/
Void load_dimension()
{
  dim_vector[0] = Er;
  dim_vector[1] = height;
  dim_vector[2] = w;
  dim_vector[3] = d1;
  dim_vector[4] = d2;
  dim_vector[5] = d;
  dim_vector[6] = gw;
  dim_vector[7] = f;
}



/************************************************************************/
/** procedure CPW_Z ... Symmetric coplanar waveguide impedance         **/
/**                     anf Eeff calculation.                          **/
/** Ref: Int. J. Electronics, 1980 Vol. 48, No.1, pp 47-56             **/
/**             h = Substrate height                                   **/
/**            Er = Rel dielectric const of substrate                  **/
/**       w_strip = Width of center conductor                          **/
/**  d_to_gplanes = Distance to ground planes                          **/
/**  gplane_width = Ground plane width                                 **/
/**        finite = If FALSE, ignore w_gplane, assume infinity         **/
/************************************************************************/
Void CPW_Z(h, Er, w_strip, d_to_gplanes, gplane_width, finite, Zo, Eeff)
double h, Er, w_strip, d_to_gplanes, gplane_width;
boolean finite;
double *Zo, *Eeff;
{
  double a, b, d, elip_ratio1, elip_ratio2, k1, k1prime, k2, k2prime, temp1,
	 temp2, temp3, temp4, TEMP;

  b = w_strip;
  a = gplane_width;
  d = d_to_gplanes;
  if (!finite)
    a = 100 * d;
  temp1 = 2 * a / (2 * d + b) + 1;
  temp2 = (1 + b / d) / ((1 + a / d + b / d) * (1 + a / d));
  k1 = temp1 * sqrt(temp2);
  k1prime = sqrt(1 - k1 * k1);
  temp1 = sinh(Pi * b / (4 * h)) / sinh((b / 2 + d) * Pi / (2 * h));
  TEMP = sinh((b / 2 + d + a) * Pi / (2 * h));
  temp2 = TEMP * TEMP;
  TEMP = sinh((b / 2 + d) * Pi / (2 * h));
  temp3 = temp2 - TEMP * TEMP;
  TEMP = sinh(Pi * b / (4 * h));
  temp4 = temp2 - TEMP * TEMP;
  k2prime = temp1 * sqrt(temp3 / temp4);
  k2 = sqrt(1 - k2prime * k2prime);
  elip_ratio1 = ELIP1(k1) / ELIP1(k1prime);
  elip_ratio2 = ELIP1(k2prime) / ELIP1(k2);
  *Eeff = (Er - 1) / 2 * elip_ratio1 * elip_ratio2 + 1;
  *Zo = 30 * Pi / sqrt(*Eeff) * elip_ratio1;
}  /*procedure*/

#undef Pi


#define Pi              3.1415926535897932364e+00


/*********************************************************************/
/** Asymmetric Coplanar Waveguide Z0, and Eeff calculation          **/
/** Ref. MTT-32, No.12 December 1984 pp. 1649-1651                  **/
/**   h           substrate height                                  **/
/**   Er          Relative dielectric const. of substrate           **/
/**   b           Center conductor width                            **/
/**   d1          Distance to ground strip                          **/
/**   d2          Distance to ground strip                          **/
/*********************************************************************/
Void ACPW_Z(h, Er, b, d1, d2, Zo, Eeff)
double h, Er, b, d1, d2, *Zo, *Eeff;
{
  double k1, k1p;   /*k1 calculated with alphap*/
  double k1m;   /*k1 calculated with alpham*/
  double root1, root2, bottomp, bottomm, alphap;
      /*alpha calculated with pos root*/
  double alpham;   /*alpha calculated with neg root*/
  double alpha1p;   /*alpha2 calculated with pos root*/
  double alpha1n;   /*alpha2 calculated with neg root*/
  double k2, k2p;   /*k2 using alpha2p*/
  double k2n;   /*k2 using alpha2n*/
  double Wa, Wb, We, TEMP;

  if (d1 == d2) {
    /*call Symmetric Coplanar routine with NOT finite ground strip
      widths*/
    CPW_Z(h, Er, b, d1, 0.0, false, Zo, Eeff);
    return;
  }
  root1 = sqrt(d1 * d2 * (b + d1) * (b + d2));
  TEMP = 0.5 * b;
  alphap = (d1 * d2 + 0.5 * b * (d1 + d2) + root1) / (TEMP * TEMP * (d1 - d2));
  TEMP = 0.5 * b;
  alpham = (d1 * d2 + 0.5 * b * (d1 + d2) - root1) / (TEMP * TEMP * (d1 - d2));
  TEMP = 0.5 * b;
  bottomp = 0.5 * b + d1 + alphap * (TEMP * TEMP);
  TEMP = 0.5 * b;
  bottomm = 0.5 * b + d1 + alpham * (TEMP * TEMP);
  k1p = 0.5 * b * (1 + alphap * (0.5 * b + d1)) / bottomp;
  k1m = 0.5 * b * (1 + alpham * (0.5 * b + d1)) / bottomm;
  if (k1p <= 1)
    k1 = k1p;
  else
    k1 = k1m;
  Wa = sinh(Pi * b / (4 * h));
  Wb = sinh(Pi / (2 * h) * (b / 2 + d1));
  We = -sinh(Pi / (2 * h) * (b / 2 + d2));
  root2 = sqrt((Wb * Wb / (Wa * Wa) - 1) * (We * We / (Wa * Wa)) - 1);
  alpha1p = 1 / (Wb + We) * (root2 - Wb * We / (Wa * Wa) - 1);
  alpha1n = 1 / (Wb + We) * (-1 - Wb * We / (Wa * Wa) - root2);
  k2p = Wa * (1 + alpha1p * Wb) / (Wb + alpha1p * Wa * Wa);
  k2n = Wa * (1 + alpha1n * Wb) / (Wb + alpha1n * Wa * Wa);
  if (k2p <= 1)
    k2 = k2p;
  else
    k2 = k2n;
  *Eeff = 1 + 0.5 * (Er - 1) * (ELIP1(k2) / ELIP1(sqrt(1 - k2 * k2))) *
	      (ELIP1(sqrt(1 - k1 * k1)) / ELIP1(k1));
  *Zo = 30 * Pi / sqrt(*Eeff) * (ELIP1(sqrt(1 - k1 * k1)) / ELIP1(k1));

  /*if statement*/
}  /*ACPW_Z*/

#undef Pi


#define Pi              3.1415926535897932364e+00


/* ************************************************************************* */
/* * Procedure GCPW_Z  Grounded Coplanar Waveguide Zo and Eeff             * */
/* *                                                                       * */
/* * Passed parameters:  w  Width of center strip                  (input) * */
/* *                     d  Distance to ground conductors          (input) * */
/* *                     h  Height of Substrate                    (input) * */
/* *                     Er Relative dielectric const of Substrate (input) * */
/* *                     Zo Impedance of the line                 (output) * */
/* *                     Eeff Effective epsilon of the line       (output) * */
/* ************************************************************************* */
Void GCPW_Z(h, Er, w, d, Zo, Eeff)
double h, Er, w, d, *Zo, *Eeff;
{
  double k, k1, a, b, elip_ratio, elip_ratio1, temp1;

  a = w / 2;
  b = a + d;
  k1 = tanh(Pi * a / (2 * h)) / tanh(Pi * b / (2 * h));
  k = a / b;
  elip_ratio = ELIP1(sqrt(1 - k * k)) / ELIP1(k);
  elip_ratio1 = ELIP1(k1) / ELIP1(sqrt(1 - k1 * k1));
  *Eeff = (1 + Er * elip_ratio * elip_ratio1) / (1 + elip_ratio * elip_ratio1);
  temp1 = 1 / (1 / elip_ratio + elip_ratio1);
  *Zo = 60 * Pi * temp1 / sqrt(*Eeff);
}  /*GCPW_Z*/

#undef Pi


#define Pi              3.1415926535897932364e+00


/***********************************************************************/
/**  Procedure MSTP_Z Calculates microstrip impedance and            **/
/**  Effective dielectric constant from physical dimensions           **/
/**                                                                   **/
/***********************************************************************/
Void MSTP_Z(h, Er, w, Zo, Eeff)
double h, Er, w, *Zo, *Eeff;
{
  double u, a, b, c, d1, func_u, Zo_air, TEMP;


  u = w / h;
  d1 = Er - 1;
  TEMP = u / 18.1;
  c = u / 18.1 * (TEMP * TEMP);   /*(u/18.1)^3*/
  a = 1 + log((u * u * u * u + u * u / 2704) / (u * u * u * u + 0.432)) / 49 +
      log(1 + c) / 18.7;
  b = 0.564 * power((Er - 0.9) / (Er + 3), 0.053);
  *Eeff = (Er + 1) * 0.5 + d1 * 0.5 * power(1 + 10 / u, -a * b);
  func_u = 6 + (Pi + Pi - 6) * exp(-power(30.666 / u, 0.7528));
  Zo_air = 59.96 * log(func_u / u + sqrt(1 + 4 / (u * u)));
  *Zo = Zo_air / sqrt(*Eeff);
}  /*procedure*/

#undef Pi


#define Pi              3.1415926535897932364e+00
#define c               3.0e+08   /*m/s*/


/* ********************************************************************* */
/* *                                                                   * */
/* * Procedure SLT_Z calculates Zo and Eeff of Slotline                * */
/* * Ref. Gupta K.C. "Computer Aided Design of Microwave Circuits"     * */
/* *      Artech House 1981, pp.65-67.                                 * */
/* *  DIMENSIONS MUST BE IN MICRONS !!!!!                              * */
/* ********************************************************************* */
Void SLT_Z(h, Er, w, f, Zo, Eeff, Error_ret)
double h, Er, w, f, *Zo, *Eeff;
int *Error_ret;
{
  double temp1, temp2, wh, lo, lamr, TEMP;

  *Error_ret = 0;
  wh = w / h;
  lo = c / (f * 1.0e+03);   /*Lambda,free air,in microns*/
  if (wh >= 0.02) {
    if (wh <= 0.2) {
      temp1 = (0.126 * wh + 0.02) * log(h / lo * 100);
      lamr = 0.923 - 0.195 * log(Er) + 0.2 * wh - temp1;
      temp1 = 0.139 * log(Er) - 0.11 + wh * (0.465 * log(Er) + 1.44);
      TEMP = 11.4 - 2.636 * log(Er) - h / lo * 100;
      temp2 = TEMP * TEMP;
      temp1 *= temp2;
      temp2 = log(wh * 100) * (19.23 - 3.693 * log(Er));
      temp2 -= temp1;
      temp1 = 50 * ((wh - 0.02) * (wh - 0.1) / wh);
      *Zo = 72.62 - 15.283 * log(Er) + temp1 + temp2;
    } else if (wh <= 1.0) {
      temp1 = (0.053 + 0.041 * wh - 0.0014 * Er) * log(h / lo * 100);
      lamr = 0.987 - 0.21 * log(Er) + wh * (0.111 - 0.0022 * Er) - temp1;
      TEMP = 10.25 - 2.171 * log(Er) + wh * (2.1 - 0.617 * log(Er)) - h * 100 / lo;
      temp1 = TEMP * TEMP;
      temp2 = (0.15 + 0.1 * log(Er) + wh * (0.899 * log(Er) - 0.79)) * temp1;
      temp1 = 1.25 * wh * (114.59 - 22.531 * log(Er));
      *Zo = 113.19 - 23.257 * log(Er) + temp1 + 20 * (wh - 0.2) * (1 - wh) - temp2;
    } else
      *Error_ret = 1;
  } else
    *Error_ret = 1;
  *Eeff = 1 / (lamr * lamr);
}  /*procedure*/

#undef Pi
#undef c


/*module*/




/* End. */
