#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

/***********************************************************************/

char *get_str(Prompt)
char *Prompt;
{
  return(readline(Prompt));
}


#define get_double get_longreal
/***********************************************************************/
void get_longreal(Prompt, ret)
char *Prompt;
double *ret;
{
  char *rstr;
  rstr=readline(Prompt);
  sscanf(rstr,"%lg", ret);
  free(rstr);
}

/***********************************************************************/

void get_int(Prompt, ret)
char *Prompt;
long *ret;
{
  char *rstr;
  rstr=readline(Prompt);
  sscanf(rstr,"%ld", ret);
  free(rstr);

}  /*get_longreal*/


int get_yn(char *Prompt)
{
   char ans[255];
   get_str(Prompt,&ans);
   if (toupper(ans[0]='Y')) {
      return(1);
   }
   return(0);
}
