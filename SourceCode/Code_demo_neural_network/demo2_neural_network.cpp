#include<stdio.h>
#include<stdlib.h>
#include <math.h>

class NeuralNetwork {
public:
    double synaptic_weights[3];
    double c[5];
    double sig[5];

	void init(){
		srand(1);
		// We model a single neuron, with 3 input connections and 1 output connection.
        // We assign random weights to a 3 x 1 matrix, with values in the range -1 to 1
        // and mean 0.
		synaptic_weights[0] = rand();
		synaptic_weights[1] = rand();
		synaptic_weights[2] = rand();
		synaptic_weights[0] /= RAND_MAX;
		synaptic_weights[1] /= RAND_MAX;
		synaptic_weights[2] /= RAND_MAX;
	}

	// The Sigmoid function, which describes an S shaped curve.
    // We pass the weighted sum of the inputs through this function to
    // normalise them between 0 and 1.
	void sigmoid(double *x,int n) {
		for(int i=0;i<n;i++)
			sig[i] = 1 / (1 + exp(-x[i]));
	}

	// The derivative of the Sigmoid function.
    // This is the gradient of the Sigmoid curve.
    // It indicates how confident we are about the existing weight.
    void sigmoid_derivative(double *x,int n) {
    	for(int i=0;i<n;i++)
			sig[i] = x[i] * (1 - x[i]);
    }

    void dot_matrix(double *a, double *b, int m,int n, int p) {
   	// a: mxn 
   	// b: nxp
   	// c: mxp
    	for(int i=0;i<5;i++)
    		c[i]=0;
		for(int i =0;i<m;i++)
			for(int j =0;j<p;j++){
	   			for(int k=0;k<n;k++)
	   				c[i*p+j] = c[i*p+j] + a[i*n+k]*b[k*p+j];
	   		}
    }

    // We train the neural network through a process of trial and error.
    // Adjusting the synaptic weights each time.
    void train(double *training_set_inputs,double *training_set_outputs,int number_of_trainin) {
        double output[4];
        double error[4];
        double training_set_inputs_T[12];
        double fix_error[4];
    	for(int i=0;i< number_of_trainin;i++) {
            // Pass the training set through our neural network (a single neuron).
    		think(training_set_inputs,4,3);
            output[0] = sig[0];output[1] = sig[1];output[2] = sig[2];output[3] = sig[3];
            printf("\nOutput:\n");
            for(int i=0;i<4;i++)
                printf("%1f\n",output[i]);

            // Calculate the error (The difference between the desired output
            // and the predicted output).
            for(int j=0;j<4;j++)
                error[j] = training_set_outputs[i] - output[i];
            printf("\nerror:\n");
            for(int i=0;i<4;i++)
                printf("%1f\n",error[i]);
            // Multiply the error by the input and again by the gradient of the Sigmoid curve.
            // This means less confident weights are adjusted more.
            // This means inputs, which are zero, do not cause changes to the weights.
            for(int k=0;k<3;k++)
                for(int l=0;l<4;l++)
                    training_set_inputs_T[k*4+l] = training_set_inputs[l*3+k];

            sigmoid_derivative(output,4);
            for(int j=0;j<4;j++)
                fix_error[i] = error[i] * sig[i];
            printf("\nfix_error\n");
            for(int i=0;i<4;i++)
                printf("%1f\n",fix_error[i]);

            dot_matrix(training_set_inputs_T, fix_error,3,4,1);

            // Adjust the weights.
            for(int j=0;j<4;j++)
                synaptic_weights[i] += c[i];
            printf("\nsynaptic_weights\n");
            for(int i=0;i<3;i++)
                printf("%1f\n",synaptic_weights[i]);
    	}
    }

    void think(double *inputs,int m,int n) {
        dot_matrix(inputs,synaptic_weights,m,n,1);
        sigmoid(c,4);
    }
};


int main(void) {
    // Intialise a single neuron neural network.
    NeuralNetwork neural_network;
    neural_network.init();
    printf("Random starting synaptic weights:\n");
    for(int i=0;i<3;i++)
        printf("%1f\n",neural_network.synaptic_weights[i]);

    // The training set. We have 4 examples, each consisting of 3 input values
    // and 1 output value.
    double training_set_inputs[12] = {0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1};
    double training_set_outputs[4] = {0, 1, 1, 0};

    // Train the neural network using a training set.
    // Do it 10,000 times and make small adjustments each time.
    neural_network.train(training_set_inputs, training_set_outputs, 10000);

    printf("New synaptic weights after training: ");
    for(int i=0;i<3;i++)
        printf("%1f\n",neural_network.synaptic_weights[i]);

    // Test the neural network with a new situation.
    printf("Considering new situation [1, 0, 0] -> ?: ");
    double test[3] = {1,0,0};
    neural_network.think(test,1,3);
    for(int i=0;i<4;i++)
        printf("%1f\n",neural_network.sig[i]);
}