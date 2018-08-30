#include "neural.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int layers2[] = {8,32,1};
  MultiLayerPerceptron mlp2(3,layers2);
  mlp2.Init();
  
//  mlp2.Run("test_4.txt",10000);
//  mlp2.Test("test_4.txt");
//  mlp2.Run("test1.txt",1000);
//  mlp2.Run("createtest2.txt",500000);
//  mlp2.Test("createtest2.txt");
//  printf("\n\n");
//  mlp2.Run("createtest3.txt",505000);
//  mlp2.Test("createtest3.txt");
//    printf("\n\n");
//  mlp2.Test("createtest4.txt");
//      printf("\n\n");
//  mlp2.Test("createtest5.txt");
//      printf("\n\n");
//  mlp2.Test("createtest6.txt");
//      printf("\n\n");
//  mlp2.Test("createtest7.txt");
  printf("\n\n");
  mlp2.Run("dedata_v2_2.txt",505000);
  mlp2.Test("dedata_v2_2.txt");
  printf("\n\n");
  mlp2.Test("dedata_v2_1.txt");
  return 0;
}
