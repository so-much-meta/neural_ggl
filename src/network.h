#ifndef network_h
#define network_h

#include "input_layer.h"
#include "layer.h"

template <class LAYER>
class Network
{
public:
	typedef typename LAYER::InputType InputType;
	typedef typename LAYER::OutputType OutputType;
	typedef typename LAYER::ActivationType ActivationType;
	
	void randomize(FPTYPE min, FPTYPE max)
	{
		for (int i=0; i<layers.size(); i++)
		{
			layers[i]->randomize(min, max);
		}
	}
	
	int add_input_layer(InputLayer<InputType>* input_layer)
	{
		input_layers.push_back(input_layer);
		return (input_layers.size()-1);		
	}

	int create_input_layer(int size)
	{
		InputLayer<InputType>* input_layer=new InputLayer<InputType>(size);
		return add_input_layer(input_layer);
	}

	int add_layer(LAYER* layer)
	{
		layers.push_back(layer);
		return (layers.size()-1);		
	}

	int create_layer(int size)
	{
		LAYER* layer = new LAYER(size);
		return add_layer(layer);
	}

	void connect_layers(int from_layer, int to_layer)
	{
		layers[to_layer]->connect(layers[from_layer]);
	}

	void connect_inputlayer(int input_layer, int to_layer)
	{
		layers[to_layer]->connect(input_layers[input_layer]);
	}
	
	/* for regular, non-TD nets */
	void backprop(vector<FPTYPE>& target, FPTYPE eta)
	{
		int i=layers.size()-1;
		layers[i]->calculate_delta(target);
		for (i=i-1; i>=0; i--)
		{
			layers[i]->backprop_delta();
		}
		for (i=0; i<layers.size(); i++)
		{
			layers[i]->update_weights(eta);
		}
	}	
protected:
	vector<InputLayer<InputType>*> input_layers;
	vector<LAYER*> layers;
};

#endif
