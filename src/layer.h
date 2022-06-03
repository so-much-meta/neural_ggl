#ifndef layer_h
#define layer_h

#include "input_layer.h"

template <class NEURON>
class Layer
{
public:
	typedef typename NEURON::InputType InputType;
	typedef typename NEURON::OutputType OutputType;
	typedef typename NEURON::ActivationType ActivationType;
	
	Layer(int num)
	{
		for (int i=0; i<num; i++)
		{
			neurons.push_back(new NEURON);
		}
	}

	void randomize(InputType min, InputType max)
	{
		for (int i=0; i<neurons.size(); i++)
		{
			neurons[i]->randomize(min, max);
		}
	}

	void connect(InputLayer<InputType>* input_layer)
	{
		for (int i=0; i<neurons.size(); i++)
		{
			((NeuronInputBase<InputType>*)(neurons[i]))->add_inputs(input_layer->get_inputs());
		}
	}

	void connect(Layer* layer_under)
	{
		for (int i=0; i<neurons.size(); i++)
		{
			for (int j=0; j<layer_under->neurons.size(); j++)
			{
				neurons[i]->add_connection(layer_under->neurons[j]);
			}
		}
	}
	
	void calculate()
	{
		for (int i=0; i<neurons.size(); i++)
		{
			neurons[i]->calculate();
		}
	}
	
#if 0
	/* for generic, non-TD nets */
	void calculate_delta(vector<OutputType> &targets)
	{
		for (int i=0; i<targets.size(); i++)
		{
			neurons[i]->calculate_delta(targets[i]);
		}
	}
	/* for generic, non-TD nets */
	void update_weights(InputType eta)
	{
		for (int i=0; i<neurons.size(); i++)
		{
			neurons[i]->update_weights(eta);
		}
	}
#endif

	void backprop_delta()
	{
		for (int i=0; i<neurons.size(); i++)
		{
			neurons[i]->backprop_delta();
		}
	}
		
	void td_calculate_delta()
	{
		for (int i=0; i<neurons.size(); i++)
		{
			neurons[i]->td_calculate_delta();
		}
	}
	void td_calculate_gradient()
	{
		for (int i=0; i<neurons.size(); i++)
		{
			neurons[i]->td_calculate_gradient();
		}
	}
	void td_reset_trace()
	{
		for (int i=0; i<neurons.size(); i++)
		{
			neurons[i]->td_reset_trace();
		}
	}
	void td_calculate_trace(InputType lambda)
	{
		for (int i=0; i<neurons.size(); i++)
		{
			neurons[i]->td_calculate_trace(lambda);
		}
	}
	void td_update_weights(InputType eta, InputType p_delta)
	{
		for (int i=0; i<neurons.size(); i++)
		{
			neurons[i]->td_update_weights(eta, p_delta);
		}
	}
	OutputType* get_output_p(int num)
	{
		return neurons[num]->get_output_p();
	}
	unsigned int size()
	{
		return neurons.size();
	}
	NEURON* get_neuron(int i)
	{
		return neurons[i];
	}
private:
	vector<NEURON* > neurons;
};

#endif
