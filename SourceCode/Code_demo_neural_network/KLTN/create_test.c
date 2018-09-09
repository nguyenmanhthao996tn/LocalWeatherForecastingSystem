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
   double tmp,e,f,c,d,h,g;
   int a,b,i;
   srand((unsigned)time( NULL ) );
   fp = fopen("createtest8.txt", "w+");
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
			a = RandomEqualINT(0,12);
			b = RandomEqualINT(0,23);
			c = RandomEqualINT(20,37);
			d = RandomEqualINT(20,37);
			e = RandomEqualINT(50,90);
			f = RandomEqualINT(50,90);
			h = RandomEqualINT(25,50);
			g = RandomEqualINT(25,50);
			c = c/10;
			d = d/10;
			e = e/10;
			f = f/10;
			h = h/10;
			g = g/10;
			tmp = a + b + c + d + e + f + h + g;
			fprintf(fp, " %d %d %.2f %.2f %.2f %.2f %.2f %.2f %.3f\n",a,b,c,d,e,f,h,g,tmp/100);
		}
   fclose(fp);
   return 0;
}
