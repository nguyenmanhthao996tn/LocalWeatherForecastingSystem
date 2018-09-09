#ifndef _NEURAL_H_
#define _NEURAL_H_


struct Neuron {
  double  m_out;     /* outpt */
  double  error;     /* error */
  double* weight;     /* weight  */
  double* last_weight;    /* last weight for momentum  */
  double* save_weight; /* saved weight */
};

struct Layer {
  int     nNumNeurons;
  Neuron* pNeurons;
};

class MultiLayerPerceptron {

  int    nNumLayers;
  Layer* pLayers;
  
  double dMSE;
  double dMAE;

  void RandomWeights();
  void SetInputSignal (double* input);
  void GetOutputSignal(double* output);
  void SaveWeights();
  void RestoreWeights();
  
  void PropagateSignal();
  void ComputeOutputError(double* target);
  void BackPropagateError();
  
  void AdjustWeights();
  void Simulate(double* input, double* output, double* target, bool training);

public:

  double dEta;
  double dAlpha;
  double dGain;
  double dAvgTestError;
  int	nRight;

  MultiLayerPerceptron(int nl, int npl[]);
  ~MultiLayerPerceptron();
  
  void Init();
  int Train(const char* fname);
  int Test(const char* fname);
  void Save(const char* fname);
  void Load(const char* fname);
  void Run(const char* fname, const int& maxiter);

};

#endif
