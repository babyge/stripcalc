/* Output from p2c, the Pascal-to-C translator */
/* From input file "stripcalc.p" */


/*generate symbol tables*/

#include <p2c/p2c.h>


#ifndef MATHLIB_H
#include "mathlib.h"
#endif

/*Hyperbolic functions*/
/* p2c: ./mathlib.p, line 8: 
 * Warning: Symbol 'MATHLIB' was already defined [220] */

#ifndef STRIPSUBS_H
#include "stripsubs.h"
#endif


/*Impedance calculation routines*/
/**import IODECLARATIONS,general_1,general_2; I/O routines**/
/** ***************************************************************** **/
/** This program performs Zo, Eeff, and Vp calculations for various   **/
/** planar geometries, including Microstrip, Coplanar, Grounded       **/
/** Coplanar, and Slotline. Where necessary it uses an iterative      **/
/** technique to solve for the required parameters.                   **/
/** Copyright (C) 1986  M. Ferrara                                    **/
/** ***************************************************************** **/

#define v_light         2.99792458e+8   /*m/s*/
#define Eo              8.854187818e-12   /*F/m*/


typedef enum {
  Microstrip, GCPW, ACPW, CPW, Slotline
} linetype;
typedef enum {
  backward, forward
} calctype;
typedef double input_vector[10];
typedef double output_vector[5];


Static linetype current_line_type;
Static double w, d1, d2, d, Zo, height, Er, gw, Eeff, f;
Static Char Ans[133];
Static boolean Finite_calc, OK;
Static Char Letter_selection;
Static calctype calculation;
Static int target_parameter;
Static double target_value;
Static int variable_parameter;
Static input_vector dim_vector;
Static output_vector result_vector;
Static boolean loop, debug_mode;
Static int Calc_error;


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
Static Void get_longreal(Prompt, ret)
Char *Prompt;
double *ret;
{
  printf("%*s", strlen(Prompt), Prompt);
  scanf("%lg%*[^\n]", ret);
  getchar();
}  /*get_longreal*/


/***********************************************************************/
Static Void Strip_Z(Strip_type, dimension, finite, result)
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
Static Void Newton(Strip_type, dimension, var_parm, finite, result,
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
Static Void load_dimension()
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


/**********************************************************************/
main(argc, argv)
int argc;
Char *argv[];
{  /*program*/
  PASCAL_MAIN(argc, argv);
  printf("Stripcalc -- (C) 1986 M.Ferrara\n");
  loop = true;
  debug_mode = false;
  while (loop) {
    TRY(try1);
      OK = false;
      while (!OK) {
	printf("Do you want:\n");
	printf("   (A) Asymmetric Coplanar Waveguide\n");
	printf("   (G) Grounded Coplanar Waveguide\n");
	printf("   (C) Symmetric Coplanar Waveguide\n");
	printf("   (M) Microstrip Transmission Line\n");
	printf("   (S) Slotline\n");
	printf("   (Q) Quit\n");
	printf("Enter letter of selection:");
	scanf("%c%*[^\n]", &Letter_selection);
	getchar();
	if (Letter_selection == '\n')
	  Letter_selection = ' ';
	OK = true;
	/*writestring(1,#12);clear screen*/
	switch (Letter_selection) {

	case 'A':
	case 'a':
	  current_line_type = ACPW;
	  printf("Asymmetric Coplanar Waveguide\n");
	  break;

	case 'G':
	case 'g':
	  current_line_type = GCPW;
	  printf("Grounded Coplanar Waveguide\n");
	  break;

	case 'C':
	case 'c':
	  current_line_type = CPW;
	  printf("Symmetric Coplanar Waveguide\n");
	  break;

	case 'M':
	case 'm':
	  current_line_type = Microstrip;
	  printf("Microstrip transmission line\n");
	  break;

	case 'S':
	case 's':
	  current_line_type = Slotline;
	  printf("Slotline\n");
	  printf("NOTE: due to dispersion calculations,\n");
	  printf("   dimensions are in microns and frequencies\n");
	  printf("   are in GHz.\n");
	  break;

	case 'D':
	case 'd':  /*turn on debug mode*/
	  debug_mode = !debug_mode;
	  OK = false;   /*Need to ask for a REAL answer*/
	  if (debug_mode)
	    printf("DEBUG ON.\n");
	  else
	    printf("DEBUG OFF.\n");
	  break;

	case 'Q':
	case 'q':  /*quit*/
	  P_escapecode = -32;
	  goto _Ltry1;
	  break;

	default:
	  OK = false;
	  break;
	}
      }
      get_str("Do you want to calculate dimensions (D) or impedances (Z)?",
	      Ans);
      Letter_selection = Ans[0];
      if (Letter_selection == 'D' || Letter_selection == 'd')
	calculation = backward;
      else
	calculation = forward;
      if (calculation == backward)
	printf("Input initial guess in place of unknown.\n");
      get_longreal("[1] Relative Dielectric constant (Er)?", &Er);
      get_longreal("[2] Substrate height (h)?", &height);
      switch (current_line_type) {

      case Microstrip:
	get_longreal("[3] Center conductor width:", &w);
	if (calculation == forward) {
	  MSTP_Z(height, Er, w, &Zo, &Eeff);
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	} else {
	  target_parameter = 0;
	  while ((unsigned)target_parameter >= 32 ||
		 ((1 << target_parameter) & 0x6) == 0) {
	    printf("Target Zo [1] or Eeff [2]?");
	    scanf("%d%*[^\n]", &target_parameter);
	    getchar();
	  }
	  get_longreal("Target value?", &target_value);
	  variable_parameter = 0;
	  while ((unsigned)variable_parameter >= 32 ||
		 ((1 << variable_parameter) & 0xe) == 0) {
	    printf("Input number preceeding dimension you want varied:");
	    scanf("%d%*[^\n]", &variable_parameter);
	    getchar();
	  }
	  load_dimension();
	  Newton(current_line_type, dim_vector, variable_parameter, true,
		 result_vector, target_parameter, target_value);
	  printf("Value of dimension %d = %6.3f\n",
		 variable_parameter, dim_vector[variable_parameter - 1]);
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	}
	break;

      case ACPW:
	get_longreal("[3] Center conductor width:", &w);
	get_longreal("[4] First distance to ground strips:", &d1);
	get_longreal("[5] Second distance to ground strips:", &d2);
	if (calculation == forward) {
	  ACPW_Z(height, Er, w, d1, d2, &Zo, &Eeff);
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	} else {
	  target_parameter = 0;
	  while ((unsigned)target_parameter >= 32 ||
		 ((1 << target_parameter) & 0x6) == 0) {
	    printf("Target Zo [1] or Eeff [2]?");
	    scanf("%d%*[^\n]", &target_parameter);
	    getchar();
	  }
	  get_longreal("Target value?", &target_value);
	  variable_parameter = 0;
	  while ((unsigned)variable_parameter >= 32 ||
		 ((1 << variable_parameter) & 0x3e) == 0) {
	    printf("Input number preceeding dimension you want varied:");
	    scanf("%d%*[^\n]", &variable_parameter);
	    getchar();
	  }
	  load_dimension();
	  Newton(current_line_type, dim_vector, variable_parameter, true,
		 result_vector, target_parameter, target_value);
	  printf("Value of dimension %d = %6.3f\n",
		 variable_parameter, dim_vector[variable_parameter - 1]);
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	}
	break;

      case GCPW:
	get_longreal("[3] Center conductor width:", &w);
	get_longreal("[6] Distance to ground strips:", &d);
	if (calculation == forward) {
	  GCPW_Z(height, Er, w, d, &Zo, &Eeff);
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	} else {
	  target_parameter = 0;
	  while ((unsigned)target_parameter >= 32 ||
		 ((1 << target_parameter) & 0x6) == 0) {
	    printf("Target Zo [1] or Eeff [2]?");
	    scanf("%d%*[^\n]", &target_parameter);
	    getchar();
	  }
	  get_longreal("Target value?", &target_value);
	  variable_parameter = 0;
	  while ((unsigned)variable_parameter >= 32 ||
		 ((1 << variable_parameter) & 0x4e) == 0) {
	    printf("Input number preceeding dimension you want varied:");
	    scanf("%d%*[^\n]", &variable_parameter);
	    getchar();
	  }
	  load_dimension();
	  Newton(current_line_type, dim_vector, variable_parameter, true,
		 result_vector, target_parameter, target_value);
	  printf("Value of dimension %5d = %6.3f\n",
		 variable_parameter, dim_vector[variable_parameter - 1]);
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	}
	break;

      case CPW:
	get_longreal("[3] Center conductor width:", &w);
	get_longreal("[6] Distance to ground strips:", &d);
	get_str("Include ground strip width in the analysis?(Y/N)", Ans);
	Letter_selection = Ans[0];
	if (Letter_selection == 'Y' || Letter_selection == 'y') {
	  get_longreal("[7] Width of ground strips:", &gw);
	  Finite_calc = true;
	} else
	  Finite_calc = false;
	if (calculation == forward) {
	  CPW_Z(height, Er, w, d, gw, Finite_calc, &Zo, &Eeff);
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	} else {
	  target_parameter = 0;
	  while ((unsigned)target_parameter >= 32 ||
		 ((1 << target_parameter) & 0x6) == 0) {
	    printf("Target Zo [1] or Eeff [2]?");
	    scanf("%d%*[^\n]", &target_parameter);
	    getchar();
	  }
	  get_longreal("Target value?", &target_value);
	  variable_parameter = 0;
	  while ((unsigned)variable_parameter >= 32 ||
		 ((1 << variable_parameter) & 0xce) == 0) {
	    printf("Input number preceeding dimension you want varied:");
	    scanf("%d%*[^\n]", &variable_parameter);
	    getchar();
	  }
	  load_dimension();
	  Newton(current_line_type, dim_vector, variable_parameter,
		 Finite_calc, result_vector, target_parameter, target_value);
	  printf("Value of dimension %d = %6.5f\n",
		 variable_parameter, dim_vector[variable_parameter - 1]);
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	}
	break;

      case Slotline:
	get_longreal("[3] Slot width:", &w);
	get_longreal("[8] Frequency:", &f);
	if (calculation == forward) {
	  SLT_Z(height, Er, w, f, &Zo, &Eeff, &Calc_error);
	  if (Calc_error != 0) {
	    P_escapecode = 1;
	    goto _Ltry1;
	  }
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	} else {
	  target_parameter = 0;
	  while ((unsigned)target_parameter >= 32 ||
		 ((1 << target_parameter) & 0x6) == 0) {
	    printf("Target Zo [1] or Eeff [2]?");
	    scanf("%d%*[^\n]", &target_parameter);
	    getchar();
	  }
	  get_longreal("Target value?", &target_value);
	  variable_parameter = 0;
	  while ((unsigned)variable_parameter >= 32 ||
		 ((1 << variable_parameter) & 0x10e) == 0) {
	    printf("Input number preceeding dimension you want varied:");
	    scanf("%d%*[^\n]", &variable_parameter);
	    getchar();
	  }
	  load_dimension();
	  Newton(current_line_type, dim_vector, variable_parameter, true,
		 result_vector, target_parameter, target_value);
	  printf("Value of dimension %d = %6.3f\n",
		 variable_parameter, dim_vector[variable_parameter - 1]);
	  printf("Zo=%3.2f Eeff=%3.2f\n", Zo, Eeff);
	  printf("rel. Vp=%1.5f\n", 1 / sqrt(Eeff));
	}
	break;
      }/*case*/
      /*recover block*/
    RECOVER2(try1,_Ltry1);
      switch (P_escapecode) {

      case 1:
	printf("Error: dimensions out allowable range for Slotline model.\n");
	printf("Try again with some new values.\n");
	break;

      default:
	if (P_escapecode >= -31 && P_escapecode <= -22 ||
	    P_escapecode >= -19 && P_escapecode <= -1) {
	  printf("Trapped error:%3d\n", P_escapecode);
	  printf("Try again with some new values.\n");
	} else
	  loop = false;
	break;
      }/*case*/
    ENDTRY(try1);
  }  /*while loop*/
  exit(0);
}  /*program*/




/* End. */
