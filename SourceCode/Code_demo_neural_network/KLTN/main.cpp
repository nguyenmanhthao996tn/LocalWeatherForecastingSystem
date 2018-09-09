#include "neural.h"
#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
#include <string.h> 

int main()
{
  int layers2[] = {8,32,1};
  char str[255];
  int chose=-1,num;
  MultiLayerPerceptron mlp2(3,layers2);
  mlp2.Init();
  while(chose != 0) 
  {
  	printf("\n\n 1. Train\n");
  	printf(" 2. Test\n");
  	printf(" 3. Save\n");
  	printf(" 4. Load\n");
	printf(" 0. Exit\n");
	printf(" Chose: ");
	scanf("%d",&chose);
    fflush(stdin);  
    if(chose == 1)
    {
  		printf("\nNhap file: ");
  		gets(str);
  		printf("\nSo lan: ");
  	    scanf("%d",&num);
        fflush(stdin); 
  		mlp2.Run(str,num);
  	}
  	if (chose == 2)
  	{
  		int a,b;
		printf("\nNhap file: ");
  		gets(str);
	  	a = mlp2.Test(str);
	  	b = mlp2.nRight;
	  	printf("%d %d\n",a,b);
	}
	if (chose == 3)
	{
		printf("\nNhap file: ");
  		gets(str);
		mlp2.Save(str);
	}
	if (chose == 4)
	{
		printf("\nNhap file: ");
  		gets(str);
		mlp2.Load(str);	
	}
  }	
//	mlp2.Run("createtest8.txt",200000);
//	mlp2.Test("createtest8.txt");
//	mlp2.Run("test9.txt",100000);
//	mlp2.Test("test9.txt");
  return 0;
}
