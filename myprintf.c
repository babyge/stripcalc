/* My printf does a fflush after every write */
/*

$Header$

$Log$
Revision 1.1  2000/06/21 18:07:37  mikef
*** empty log message ***

Revision 1.2  1999/12/13 22:51:13  mikef
Add header and log keywords to all files

*/

#include <stdio.h>
#include <stdarg.h>

int printf(const char *format, ...) {
   int ret;
   va_list ap;

   va_start(ap, format);
   ret=vprintf(format, ap);
   va_end(ap);
   fflush(stdout);
   return(ret);
}

int fprintf(FILE *stream, const char *format, ...) {
   int ret;
   va_list ap;

   va_start(ap, format);
   ret=vfprintf(stream, format, ap);
   va_end(ap);
   fflush(stream);
   return(ret);
}
   
