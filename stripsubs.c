/*
$Header$

*/

#define STRIPSUBS_G
#include "stripsubs.h"


/***********************************************************************/
void Strip_Z(linetype Strip_type, double *dimension, unsigned char finite, double *result)
{
  double Er;   /*dimension[1]*/
  double h;   /*dimension[2]*/
  double w;   /*dimension[3]*/
  double d1;   /*dimension[4]*/
  double d2;   /*dimension[5]*/
  double d;   /*dimension[6]*/
  double sw;   /*dimension[7]*/
  double f;   /*dimension[8]*/
  double b;
  int Calc_err;

  Er = dimension[0];
  h = dimension[1];
  w = dimension[2];
  switch (Strip_type) {

  case Microstrip:
    t = dimension[9];
    MSTP_Z(h, Er, w, t, &Zo, &Eeff);
    break;

  case BuriedMicrostrip:
    b=dimension[8];
    t = dimension[9];
    BMSTP_Z(h, Er, w, b, t, &Zo, &Eeff);
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
       /*      _Escape(1);*/
    break;

  case Stripline:
     b = dimension[8];
     t = dimension[9];
     if (t == 0) {
	STPLN_Z(b, Er, w, &Zo, &Eeff);
     } else {
	FTSTPLN_Z(b, Er, w, t, &Zo, &Eeff);
     }
     break;

  case OffsetStripline:
     b = dimension[8];
     t = dimension[9];
     OSTPLN_Z (b, Er, w, h, t, &Zo, &Eeff);
     break;
  }/*case*/
  result[0] = Zo;
  result[1] = Eeff;
}  /*procedure*/


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
void CPW_Z(double h, double Er, double w_strip, double d_to_gplanes, 
	   double gplane_width, unsigned char finite, double *Zo, double *Eeff)
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



/*********************************************************************/
/** Asymmetric Coplanar Waveguide Z0, and Eeff calculation          **/
/** Ref. MTT-32, No.12 December 1984 pp. 1649-1651                  **/
/**   h           substrate height                                  **/
/**   Er          Relative dielectric const. of substrate           **/
/**   b           Center conductor width                            **/
/**   d1          Distance to ground strip                          **/
/**   d2          Distance to ground strip                          **/
/*********************************************************************/
void ACPW_Z(double h, double Er, double b, double d1, double d2, double *Zo, double *Eeff)
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
void GCPW_Z(double h, double Er, double w, double d, double *Zo, double *Eeff)
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

#ifdef OLD_MSTP
/***********************************************************************/
/**  Procedure MSTP_Z Calculates microstrip impedance and            **/
/**  Effective dielectric constant from physical dimensions           **/
/**                                                                   **/
/***********************************************************************/
void MSTP_Z(double h, double Er, double w, double *Zo, double *Eeff)
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
#else
void MSTP_Z(double h, double Er, double w, double t, double *Zo, double *Eeff)
{
   double tmp1,tmp2;
   double dwt,dwp;
   if (t) {
     /*Width correction for finite thickness*/
     dwt = (1.0/Pi)*log( (4*E) / sqrt((t*t/(h*h)) + pow((1/Pi)/((w/t)+1.1),2)));
     dwp = (dwt * t) * ( (1.0 + 1.0/Er)/2.0);
     w = w + dwp;
   }

   tmp1=sqrt(power(((14.0 + (8.0/Er))/11.0),2)*power((4.0*h/w),2)+
	     (((1.0 + (1.0/Er))/2.0)*Pi*Pi));

   tmp2=log(1.0 + (4.0*h/w)*(((14.0+(8.0/Er))/11.0)*(4.0*h/w)+tmp1));
   *Zo=No*tmp2/(2.0*sqrt(2.0)*Pi*sqrt(Er+1.0));

   if (w/h <= 1.0) {
      *Eeff=((Er+1)/2) + ((Er-1)/2*(pow((1+(12*h/w)),-0.5) + 
				    0.04*pow((1.0 - (w/h)),2)));
   } else {
      *Eeff=((Er+1)/2) + ((Er-1)/2*(pow((1+(12*h/w)),-0.5)));
   }
}
#endif

void BMSTP_Z(double h, double Er, double w, double b, double t, double *Zo, double *Eeff)
{
double Eburied, Zomicrostrip, Emicrostrip;
MSTP_Z(h, Er, w, t, &Zomicrostrip, &Emicrostrip);
Eburied=Emicrostrip*exp(-2.0*b/h) + Er*(1.0 - exp(-2.0*b/h));
*Eeff=Eburied;
*Zo=Zomicrostrip*sqrt(Emicrostrip)/sqrt(Eburied);
}


#define c   v_light


/* ********************************************************************* */
/* *                                                                   * */
/* * Procedure SLT_Z calculates Zo and Eeff of Slotline                * */
/* * Ref. Gupta K.C. "Computer Aided Design of Microwave Circuits"     * */
/* *      Artech House 1981, pp.65-67.                                 * */
/* *  DIMENSIONS MUST BE IN MICRONS !!!!!                              * */
/* ********************************************************************* */
void SLT_Z(double h, double Er, double w, double f, double *Zo, double *Eeff, int *Error_ret)
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

/*Zero strip thickness stripline*/
/*Transmission Line Design Handbook, Wadell, pp 125-126 */
void STPLN_Z(b, Er, w, Zo, Eeff)
     double b, Er, w, *Zo, *Eeff;
{
   double k,kprime;

   k=sech((Pi*w)/(2.0*b));
   kprime=tanh((Pi*w)/(2.0*b));
   
   *Zo=(No/(4.0*sqrt(Er)))*ELIP1(k)/ELIP1(kprime);
   *Eeff=Er;
}

/*Finite strip thickness stripline*/
/*Transmission Line Design Handbook, Wadell, pp 126,127*/
void FTSTPLN_Z(double b,double Er,double w,double t,double *Zo,double *Eeff)
{
   double m,dwt,wprime;
   double tmp;

   *Eeff=Er;
   m = 6.0/(3.0 + (2.0*t/(b-t)));
   tmp = pow(1.0/((2.0*(b-t)/t) + 1.0),2.0) + pow((1.0/(4*Pi))/((w/t)+1.1),m);
   dwt = (1.0/Pi) * (1.0 - 0.5 * log(tmp));
   wprime = w+(dwt*t);
   tmp = sqrt(pow(8.0*(b-t)/(Pi*wprime),2)+6.27) + (8.0*(b-t)/(Pi*wprime));
   tmp *= 4.0*(b-t)/(Pi*wprime);
   *Zo = No/(4.0*Pi*sqrt(Er)) * log(1.0 + tmp);
}

#ifdef OLD_OSTPLN

void OSTPLN_Z (double b, double Er, double w, double h, double *Zo, double *Eeff)
{
   double Zoair,dZoair,h1,h2;
   double Zoh1, Zoh2, Zoctr;
   double t = 0;

   h1=min(b-h-t,h);
   h2=max(b-h-t,h);
   if (h1 == h2) { /* Symmetrical */
      STPLN_Z(h1, Er, w, Zo, Eeff);
      return;
   }

   STPLN_Z(h1, 1.0, w, &Zoh1, Eeff);
   STPLN_Z(h2, 1.0, w, &Zoh2, Eeff);
   STPLN_Z(b, 1.0, w, &Zoctr, Eeff);
   

   Zoair=2.0*((Zoh1*Zoh2)/(Zoh1 + Zoh2));

   dZoair=(0.26*Pi/8.0)*Zoair*Zoair*
          power((0.5-((h1 + (t/2))/(h1+h2+t))),2.2) *
          power(((t+w)/(h1+h2+t)),2.9);

   *Zo=(1.0/sqrt(Er))*(Zoctr - dZoair);
   *Eeff=Er;
}

#else

double func(double x) 
{
   double ret;
   if (x <= 0) x=1.0e-6; /*If we don't do this, the log is ill-conditioned */
   ret = (1.0 - 2.0*x)*(((1.0-x)*log(1.0-x))-x*log(x));
   return(ret);
}

void OSTPLN_Z (double b, double Er, double w, double h, double t, double *Zo, double *Eeff)
{
   double h1,h2;
   double s = 0;
   double x,A,d0,cl;
   double gam,beta,k,kp,wbeff,Cfp;

   *Eeff=Er;
   h1=min(b-h-t,h);
   h2=max(b-h-t,h);
   if (h1 == h2) { /* Symmetrical */
      STPLN_Z(h1, Er, w, Zo, Eeff);
      return;
   }

   s=(2*h2)+t-b;
   if (w>=t) {
      x=t/w;
   } else {
      x=w/t;
   }
   d0=w*(0.5008 + 1.0235*x - 1.0230*x*x + 1.1564*x*x*x - 0.4749*x*x*x*x);
   cl=((b-s)/2.0);
   A=sin(Pi*cl/b)*coth(Pi*d0/(2*b));

   if ((w/(b-t)) < 0.35) {
      *Zo = No*acosh(A) /(2.0*Pi*sqrt(Er));
   } else {
      if ((w/(b-t)) < (t/b)) {
	 k=sech(Pi*w/(2*b));
	 kp=tanh(Pi*w/(2*b));
	 wbeff= (w/b) + power((1.0-(t/b)),8.0)*((ELIP1(kp)/ELIP1(k)) - 2.0*log(2.0)/Pi - w/b);
      } else {
	 wbeff = w/b;
      }
      beta=1.0-(t/b);
      gam=(cl/b)-(t/(2*b));
      Cfp=Er*Eo/Pi*(2.0*log(1.0/(gam*(beta-gam))) + (func(t/(2*b))-func(cl/b))/(gam*(beta-gam)));
      *Zo=(No/sqrt(Er))*(1/(wbeff/gam + wbeff/(beta-gam) + 2*Cfp/(Eo*Er)));
   }
}

#endif
