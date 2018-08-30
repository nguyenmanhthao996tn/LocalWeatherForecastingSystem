#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int RandomEqualINT(int Low, int High)
{
  return rand() % (High-Low+1) + Low;
}

int main()
{
   FILE *fp;
   double tmp;
   int a,b,c,d,e,f,h,g,i;
   srand((unsigned)time( NULL ) );
   fp = fopen("createtest7.txt", "w+");
//   fprintf(fp, "Vi du kiem tra ham fprintf ...\n");
//	for( a=0;a<10;a++)
//	for( b=0;b<10;b++)
//	for( c=0;c<10;c++)
//	for( d=0;d<10;d++)
//	for( e=0;e<10;e++)
//	for( f=0;f<10;f++)
//	for( h=0;h<10;h++)
//	for( g=0;g<10;g++)
	for(i=0;i<10;i++)
		{
			a = RandomEqualINT(0,15);
			b = RandomEqualINT(0,15);
			c = RandomEqualINT(0,15);
			d = RandomEqualINT(0,15);
			e = RandomEqualINT(0,15);
			f = RandomEqualINT(0,15);
			h = RandomEqualINT(0,15);
			g = RandomEqualINT(0,15);
			tmp = a + b + c + d + e + f + h + g;
			fprintf(fp, " %d %d %d %d %d %d %d %d %.2f\n",a,b,c,d,e,f,h,g,tmp/100);
		}
   fclose(fp);
   return 0;
}
