#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "neural.h"


void InitializeRandoms()
{
  //  srand( (unsigned)time( NULL ) );
  srand(4711);
}

int RandomEqualINT(int Low, int High)
{
  return rand() % (High-Low+1) + Low;
}

double RandomEqualREAL(double Low, double High)
{
  return ((double) rand() / RAND_MAX) * (High-Low) + Low;
}


MultiLayerPerceptron::MultiLayerPerceptron(int nl, int npl[]) :
  nNumLayers(0),
  pLayers(0),
  dMSE(0.0),
  dMAE(0.0),
  dEta(0.9),
  dAlpha(0.25),
  dGain(1.0),
  dAvgTestError(0.0), 
  degr_avg(0)
{
	int i,j;

  /* --- creation of diapers --- */
  nNumLayers = nl;
  pLayers    = new Layer[nl];

  /* --- init layers --- */
  for ( i = 0; i < nl; i++ )
    {

      /* --- creation of neurons --- */
      pLayers[i].nNumNeurons = npl[i];
      pLayers[i].pNeurons    = new Neuron[ npl[i] ];

      /* --- init neurons --- */
      for( j = 0; j < npl[i]; j++ )
	{
	  pLayers[i].pNeurons[j].m_out  = 1.0;
	  pLayers[i].pNeurons[j].error  = 0.0;
	  if(i>0)
	    {
	      pLayers[i].pNeurons[j].weight     = new double[ npl[i-1] ];
	      pLayers[i].pNeurons[j].last_weight    = new double[ npl[i-1] ];
	      pLayers[i].pNeurons[j].save_weight = new double[ npl[i-1] ];
	    }
	  else
	    {
	      pLayers[i].pNeurons[j].weight     = NULL;
	      pLayers[i].pNeurons[j].last_weight    = NULL;
	      pLayers[i].pNeurons[j].save_weight = NULL;
	    }
	}

    }


}

MultiLayerPerceptron::~MultiLayerPerceptron() {
  int i,j;
  for( i = 0; i < nNumLayers; i++ )
    {
      if ( pLayers[i].pNeurons )
	{
	  for( j = 0; j < pLayers[i].nNumNeurons; j++ )
	    {
	      if ( pLayers[i].pNeurons[j].weight )
		delete[] pLayers[i].pNeurons[j].weight;
	      if ( pLayers[i].pNeurons[j].last_weight )
		delete[] pLayers[i].pNeurons[j].last_weight;
	      if ( pLayers[i].pNeurons[j].save_weight )
		delete[] pLayers[i].pNeurons[j].save_weight;
	    }
	}
      delete[] pLayers[i].pNeurons;
    }
  delete[] pLayers;
}

void MultiLayerPerceptron::RandomWeights()
{
  int i,j,k;
  for( i = 1; i < nNumLayers; i++ )
    {
      for( j = 0; j < pLayers[i].nNumNeurons; j++ )
	{
	  for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ )
	    {
	      pLayers[i].pNeurons[j].weight [k]    = RandomEqualREAL(0, 1);
	      pLayers[i].pNeurons[j].last_weight[k]    = 0.0;
	      pLayers[i].pNeurons[j].save_weight [k] = 0.0;
	    }
	}
    }
}

void MultiLayerPerceptron::SetInputSignal(double* input)
{
  int i;
  for ( i = 0; i < pLayers[0].nNumNeurons; i++ )
    {
      pLayers[0].pNeurons[i].m_out = input[i];
    }
}

void MultiLayerPerceptron::GetOutputSignal(double* output)
{
  int i;
  for ( i = 0; i < pLayers[nNumLayers-1].nNumNeurons; i++ )
    {
      output[i] = pLayers[nNumLayers-1].pNeurons[i].m_out;
    }
}

void MultiLayerPerceptron::SaveWeights()
{
  int i,j,k;
  for( i = 1; i < nNumLayers; i++ )
    for( j = 0; j < pLayers[i].nNumNeurons; j++ )
      for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ )
	pLayers[i].pNeurons[j].save_weight[k] = pLayers[i].pNeurons[j].weight[k];
}

void MultiLayerPerceptron::RestoreWeights()
{
  int i,j,k;
  for( i = 1; i < nNumLayers; i++ )
    for( j = 0; j < pLayers[i].nNumNeurons; j++ )
      for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ )
	pLayers[i].pNeurons[j].weight[k] = pLayers[i].pNeurons[j].save_weight[k];
}

/* --- calculate and feedforward outputs from the first layer to the last ---*/

void MultiLayerPerceptron::PropagateSignal()
{
  int i,j,k;

  /* --- the loop starts with the second layer --- */
  for( i = 1; i < nNumLayers; i++ )
    {
      for( j = 0; j < pLayers[i].nNumNeurons; j++ )
	  {
		  /* --- calculating the weighted sum in the input --- */
		  double sum = 0.0;
		  for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ )
		    {
		      double out = pLayers[i-1].pNeurons[k].m_out;
		      double w   = pLayers[i  ].pNeurons[j].weight[k];
		      sum += w * out;
		    }
		  /* --- application of the activation function (sigmoid) --- */
		  pLayers[i].pNeurons[j].m_out = 1.0 / (1.0 + exp(-sum));
	  }
    }
}

void MultiLayerPerceptron::ComputeOutputError(double* target)
{
  int  i;
  for( i = 0; i < pLayers[nNumLayers-1].nNumNeurons; i++)
    {
      double x = pLayers[nNumLayers-1].pNeurons[i].m_out;
      double d = target[i] - x;
	  pLayers[nNumLayers-1].pNeurons[i].error =  x * (1.0 - x) * d;
    }
}

/* --- backpropagate error from the output layer through to the first layer --- */

void MultiLayerPerceptron::BackPropagateError()
{
  int i,j,k;
  /* --- the loop starts at the last layer --- */
  for( i = (nNumLayers-2); i >= 0; i-- )
    {
      /* --- inferior layer --- */
      for( j = 0; j < pLayers[i].nNumNeurons; j++ )
	{
	  double x = pLayers[i].pNeurons[j].m_out;
	  double E = 0.0;
	  /* --- upper layer --- */
	  for ( k = 0; k < pLayers[i+1].nNumNeurons; k++ )
	    {
	      E += pLayers[i+1].pNeurons[k].weight[j] * pLayers[i+1].pNeurons[k].error;
	    }
	 pLayers[i].pNeurons[j].error = x * (1.0 - x) * E;
	}
    }
}

/* --- update weights for all of the neurons from the first to the last layer --- */

void MultiLayerPerceptron::AdjustWeights()
{
  int i,j,k;
  /* --- the loop starts with the second layer --- */
  for( i = 1; i < nNumLayers; i++ )
    {
      for( j = 0; j < pLayers[i].nNumNeurons; j++ )
	{
	  for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ )
	    {
	      double x  = pLayers[i-1].pNeurons[k].m_out;
	      double e  = pLayers[i  ].pNeurons[j].error;
	      double dw = pLayers[i  ].pNeurons[j].last_weight[k];
	      pLayers[i].pNeurons[j].weight [k] += dEta * x * e + dAlpha * dw;
	      pLayers[i].pNeurons[j].last_weight[k]  = dEta * x * e;
//		  pLayers[i].pNeurons[j].last_weight[k]	 = pLayers[i].pNeurons[j].weight [k];
	    }
	}
    }
}

void MultiLayerPerceptron::Simulate(double* input, double* output, double* target, bool training)
{
  if(!input)  return;
  if(!target) return;
  double tmp;
  /* --- the signal is passed through the network --- */
  SetInputSignal(input);
  PropagateSignal();
  if(output) GetOutputSignal(output);

  if(output && !training) {
  	  if(fabs(target[0]-output[0]) < 0.01 ) tmp = 0.0;
  	  else {
  	  	if (target[0] > output[0])
			tmp = (fabs(target[0]-output[0])/target[0])*100.0;
		else 
			tmp = (fabs(target[0]-output[0])/output[0])*100.0;
	  }
	  degr_avg += tmp;
	  printf("test: %.2f   %.3f %.3f %.3f %.3f %.3f %.3f %.3f= %.3f\n",tmp, input[0],input[1],input[2],input[3],input[4],input[5],target[0],output[0]);
  }
  /* --- if it's an apprenticeship, we do a re-propagation of the error */
  if (training)
    {
    	  /* --- calculation of the output error in relation to the target --- */
        /*     this calculation serves as a basis for the retropulation     */
  	  ComputeOutputError(target);
      BackPropagateError();
      AdjustWeights();
//      SaveWeights();
    }
}

bool read_number(FILE* fp, double* number)
{
  char szWord[256];
  int i = 0;
  int b;

  *number = 0.0;

  szWord[0] = '\0';
  while ( ((b=fgetc(fp))!=EOF) && (i<255) )
    {
      if( (b=='-') ||
	  (b=='.') ||
	  (b=='0') ||
	  (b=='1') ||
	  (b=='2') ||
	  (b=='3') ||
	  (b=='4') ||
	  (b=='5') ||
	  (b=='6') ||
	  (b=='7') ||
	  (b=='8') ||
	  (b=='9') )
	{
	  szWord[i++] = (char)b;
	}
      else
	if(i>0) break;
    }
  szWord[i] = '\0';

  if(i==0) return false;

  *number = atof(szWord);

  return true;
}

int MultiLayerPerceptron::Train(const char* fname)
{
  int count = 0;
  int nbi   = 0;
  int nbt   = 0;
  double* input  = NULL;
  double* output = NULL;
  double* target = NULL;
  FILE*   fp = NULL;

  fp = fopen(fname,"r");
  if(!fp) return 0;

  input  = new double[pLayers[0].nNumNeurons];
  output = new double[pLayers[nNumLayers-1].nNumNeurons];
  target = new double[pLayers[nNumLayers-1].nNumNeurons];

  if(!input) return 0;
  if(!output) return 0;
  if(!target) return 0;


  while( !feof(fp) )
    {
      double dNumber;
      if( read_number(fp,&dNumber) )
	{
	  /* --- we transform it into input / target --- */
	  if( nbi < pLayers[0].nNumNeurons )
	    input[nbi++] = dNumber;
	  else if( nbt < pLayers[nNumLayers-1].nNumNeurons )
	    target[nbt++] = dNumber;

	  /* --- we make an apprenticeship of the king with this line --- */
	  if( (nbi == pLayers[0].nNumNeurons) &&
	      (nbt == pLayers[nNumLayers-1].nNumNeurons) )
	    {
	      Simulate(input, output, target, true);
	      nbi = 0;
	      nbt = 0;
	      count++;
	    }
	}
      else
	{
	  break;
	}
    }

  if(fp) fclose(fp);

  if(input)  delete[] input;
  if(output) delete[] output;
  if(target) delete[] target;

  return count;
}

int MultiLayerPerceptron::Test(const char* fname)
{
  int count = 0;
  int nbi   = 0;
  int nbt   = 0;
  double* input  = NULL;
  double* output = NULL;
  double* target = NULL;
  FILE*   fp = NULL;

  fp = fopen(fname,"r");
  if(!fp) return 0;

  input  = new double[pLayers[0].nNumNeurons];
  output = new double[pLayers[nNumLayers-1].nNumNeurons];
  target = new double[pLayers[nNumLayers-1].nNumNeurons];

  if(!input) return 0;
  if(!output) return 0;
  if(!target) return 0;

  dAvgTestError = 0.0;

  while( !feof(fp) )
    {
      double dNumber;
      if( read_number(fp,&dNumber) )
	{
	  /* --- on le transforme en input/target */
	  if( nbi < pLayers[0].nNumNeurons )
	    input[nbi++] = dNumber;
	  else if( nbt < pLayers[nNumLayers-1].nNumNeurons )
	    target[nbt++] = dNumber;

	  if( (nbi == pLayers[0].nNumNeurons) &&
	      (nbt == pLayers[nNumLayers-1].nNumNeurons) )
	    {
	      Simulate(input, output, target, false);
	      dAvgTestError += dMAE;
	      nbi = 0;
	      nbt = 0;
	      count= count +1;
	    }
	}
      else
	{
	  break;
	}
	
    }

  dAvgTestError /= count;

  if(fp) fclose(fp);

  if(input)  delete[] input;
  if(output) delete[] output;
  if(target) delete[] target;

  return count;
}


void MultiLayerPerceptron::Save(const char* fname)
{
  int i,j,k;
  FILE * fp_w;
  
  fp_w = fopen(fname, "w+");
  for( i = 1; i < nNumLayers; i++ )
    for( j = 0; j < pLayers[i].nNumNeurons; j++ )
      for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ ) {
			pLayers[i].pNeurons[j].save_weight[k] = pLayers[i].pNeurons[j].weight[k];
			fprintf(fp_w," %.17f",pLayers[i].pNeurons[j].save_weight[k]);
		}
	fclose(fp_w);
}

void MultiLayerPerceptron::Load(const char* fname)
{
  int i,j,k;
  FILE * fp_r;	
  double dNumber;
  
  fp_r = fopen(fname, "r");
  
	//
  	for( i = 1; i < nNumLayers; i++ )
    	for( j = 0; j < pLayers[i].nNumNeurons; j++ )
      		for ( k = 0; k < pLayers[i-1].nNumNeurons; k++ ) {
      			read_number(fp_r,&dNumber);
      			pLayers[i].pNeurons[j].weight [k]    = dNumber;
      			pLayers[i].pNeurons[j].save_weight[k] = pLayers[i].pNeurons[j].weight[k];
    		}

  if(fp_r) fclose(fp_r);
}


void MultiLayerPerceptron::Init()
{
  /* --- initiator of the random number generator ---  */
  /* --- and generation of random weights --- */
  InitializeRandoms();
  RandomWeights();	
}

void MultiLayerPerceptron::Run(const char* fname, const int& maxiter)
{
  int    countTrain = 0;
  int    countLines = 0;
  
  /* --- we start learning with tests --- */
  do {

    countLines += Train(fname);
//    Test(fname);
    countTrain++;
	printf("tranin: %d count\n",countTrain);
  } while ( (countTrain<maxiter) );

}



