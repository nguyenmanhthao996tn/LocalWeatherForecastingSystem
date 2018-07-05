#include "neural.h"

int main()
{
  int layers2[] = {4,24,1};
  MultiLayerPerceptron mlp2(3,layers2);
  mlp2.Init();
  
//  mlp2.Run("test.txt",1000);
//  mlp2.Run("test1.txt",1000);
  mlp2.Run("test_4.txt",50000);
  mlp2.Test("test_4.txt");
  return 0;
}
