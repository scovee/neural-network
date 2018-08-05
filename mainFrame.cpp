//if you're reading or reviewing my code, sorry in advance, I won't be able to pay for your eye surgery
//this was done by a student so the comments are :okhand:
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
using namespace std;

struct Connection
{
	double weight;
	double deltaWeight;
};

class Neuron {};

typedef vector<Neuron> Layer;

// ************************* Class Neuron ************************
//       callback used purely so the code would look "good"

class Neuron
{
public:
	Neuron(unsigned numOutputs, unsigned myIndex);
	void feedForward(const Layer &prevLayer);
	void setOutputVal(double val) { m_outputVal = val; }
	double getOutputVal(void) const { return m_outputVal; }

	void calcOutputGradients(double targetVal);
	void calcHiddenGradients(const Layer &nextLayer);

private:
	static double randomWeight(void) { return rand() / double(RAND_MAX); }
	static double transferFunction(double x);
	static double transferFunctionDerivative(double x);

	unsigned m_myIndex;
	double m_outputVal;
	double m_gradient;

	vector <Connection> m_outputWeights; //a system to weigh values for the layers to the right

};

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
	double dow = sumDOW(nextLayer);

}
void Neuron::calcOutputGradients(double targetVal)
{
	double delta = targetVal - m_outputVal;
	m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
	for (unsigned c = 0; c < numOutputs; ++c)
	{
		m_outputWeights.push_back(Connection());
		m_outputWeights.back().weight = randomWeight();
	}

	m_myIndex = myIndex;
}

void Neuron::feedForward(const Layer &prevLayer)
{
	double sum = 0.0
		// sum previous layer's outputs(aka inputs)
		// include bias node from previous layer

		for (unsigned n = 0; n < prevLayer.size(); ++n)
		{
			sum += prevLayer[n].getOutputVal() *
				prevLayer[n].m_outputWeights[m_myIndex].weight;
		}

	m_outputVal = Neuron::transferFunction(sum);
}

double Neuron::transferFunction(double x)
{
	//output range is a hyperbolic tan [-1 ... +1]
	return tanh(x);
}

double Neuron::transferFunctionDerivative(double x)
{
	//derivative function is also a hyperbolic tan [-1 ... +1]
	return 1.0 - x * x;
}



// ************************** Class Net **************************

class Net
{
public:
	Net(vector <unsigned> &topology);
	
		void feedForward(const vector<double> &inputVals);
		void backProp(const vector<double> &targetVals);
		void getResults(vector<double> &resultVals) const;
		//vectors are a type of "dynamic array"
		//rather than holding a single data type, they can hold mutliple references to other objects
		
private:
	vector<Layer> m_layers; //a private vector that holds the data for the neural layers (layerNum & neuronNum)

	double m_error;
	double m_recentAverageError;
	double m_recentAverageSmoothingFactor;
};

void Net::backProp(const vector<double> &targetVals)
{
	//calculate overall net error (RMS)
	Layer &outputLayer = m_layers.back();
	m_error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		double delta = targetVals[n] - outputLayer[n].getOutputVal();
		m_error += delta * delta;
	}
	m_error /= outputLayer.size() - 1; //average error squared
	m_error = sqrt(m_error); //RMS

	//Implement a recent average measurement
	m_recentAverageError =
		(m_recentAverageError * m_recentAverageSmoothingFactor + m_error)
		/ (m_recentAverageSmoothingFactor + 1.0);

	//calculate output layer gradients
	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		outputLayer[n].calcOutputGradients(targetVals[n]);
	}

	//calculate gradients on hidden layers
	for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum)
	{
		Layer &hiddenLayer = m_layers[layerNum];
		Layer &nextLayer = m_layers[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); ++n)
		{
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}

	//update connection weights
	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum)
	{
		layer[n].updateInputWeights(prevLayer);
	}
}

Net::Net(const vector <unsigned> &topology)
{
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
	{
		m_layers.push_back(Layer());	//creates layers 
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];
	}

	for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum)	//adds neurons to the created layers
	{
		m_layers.back().push_back(Neuron(numOutputs, neuronNum));
	}
}

void Net::feedForward(const vector<double> &inputVals)
{
	assert(inputVals.size() == m_layers[0].size() - 1); 
	//latches the input values into the input neurons
	for (unsigned i = 0; i < inputVals.size(); i++)
	{
		m_layers[0][i].setOutputVal(inputVals[i]);
	}

	//forward propagate
	for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum)
	{
		Layer &prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n)
		{
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}



int main()
{
	vector<unsigned> topology; //this vector hold the amount of neurons located within each layer, hence the unsigned int
	topology.push_back(3); 
	topology.push_back(2);
	topology.push_back(1);
	//creates input neurons
	
	Net myNet(topology);

	vector<double> inputVals;
	vector<double> targetVals;
	vector<double> resultVals;

	myNet.feedForward(inputVals); //creates hidden bias nodes, neurons for pattern recognition
	myNet.backProp(targetVals);
	myNet.getResults(resultVals);
}