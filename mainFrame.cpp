//if you're reading or reviewing my code, sorry in advance, I won't be able to pay for your eye surgery
//this was done by a student so the comments are :okhand:
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
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
	void setOutputVal(double val) { m_outputVal = val };
	double getOutputVal(void) { return m_outputVal; }
	void feedForward(const Layer &prevLayer);

private:
	double m_outputVal;
	vector <Connection> m_outputWeights; //a system to weigh values for the layers to the right

	static double randomWeight(void) { return rand() / double(RAND_MAX); }

	unsigned m_myIndex;
	
	void feedForward(const Layer &prevLayer)
	{
		double sum = 0.0
			// sum previous layer's outputs(aka inputs)
			// include bias node from previous layer

			for (unsigned n = 0; n < prevLayer.size(); ++n)
			{
				sum += prevLayer[n].getOutputVal() *
					prevLayer[n].m_outputWeights[m_myIndex].weight;
			}
	}

	Neuron(unsigned numOutputs, unsigned myIndex)
	{
		for (unsigned c = 0; c < numOutputs; ++c)
		{
			m_outputWeights.push_back(Connection());
			m_outputWeights.back().weight = randomWeight();
		}
		m_myIndex = myIndex;
	}
};



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
};

void Net::feedForward(const vector<double> &inputVals)
{
	assert(inputVals.size() == m_layers[0].size() - 1); 
	//assign (latch) the input values into the input neurons
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