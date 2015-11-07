#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

void printva(int num, ...){
	long a;
	long b;
	long c;
	long s;
	long d;
	float f;
	va_list ap;
	va_start(ap, num);
	a = va_arg(ap, long);
	b = va_arg(ap, long);
	c = va_arg(ap, long);
	s = va_arg(ap, long);
	d = va_arg(ap, long);
	f = va_arg(ap, double);
	printf("%i, %lu, %ld, %s, %c, %f\n", (int)a, b, c, (char*)s, (char)d, (float)f);
}

/*converts (unsigned) long/int to string*/
static char *int10_to_str(long int val, char *dst)
{
  char buffer[65];
  register char *p;
  long int new_val;


	if (val < 0)
	{
	  *dst++ = '-';
	  val = -val;
	}
  

  p = &buffer[sizeof(buffer)-1];
  *p = '\0';
  new_val= (long) ((unsigned long int) val / 10);
  *--p = '0'+ (char) ((unsigned long int) val - (unsigned long) new_val * 10);
  val = new_val;

  while (val != 0)
  {
    new_val=val/10;
    *--p = '0' + (char) (val-new_val*10);
    val= new_val;
  }
  while ((*dst++ = *p++) != 0) ;
  return dst-1;
}


int main(void){
	int i;
	int a = -1000000;
	unsigned long b = 4294967295;
	long c = -13242324;
	char d = 'd';
	char* s = "hello";
	float f = 3.1415926;
	char* ss, sss;
	printva(3, a,b,c,s,d,f);

	ss = (char*)malloc(30);
	int10_to_str(c, ss);

}