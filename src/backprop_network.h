#ifndef backprop_network_h
#define backprop_network_h

#include "network.h"

template <class LAYER>
class TDNetwork : public Network<LAYER>
{
	using Network<LAYER>::layers;
	using Network<LAYER>::input_layers;
	using Network<LAYER>::create_layer;
public:
	typedef typename Network<LAYER>::InputType InputType;
	typedef typename Network<LAYER>::OutputType OutputType;
	typedef typename Network<LAYER>::ActivationType ActivationType;
	
	void create_ff_layer( int size )
	{
		int numlayers;
		if (input_layers.size()==0)
		{
			Network<LAYER>::create_input_layer(size);
			return;
		}
		numlayers = layers.size();
		if (numlayers==0)
		{
			this->connect_inputlayer(0, create_layer(size));
		}
		else
		{
			this->connect_layers(numlayers-1, create_layer(size));
		}
	}

	void create_output_vector()
	{
		LAYER* output_layer=layers[layers.size()-1];
		for (int i=0; i<output_layer->size(); i++)
		{
			outputs.push_back(output_layer->get_output_p(i));
		}
	}

	vector<InputType*> & get_input_vector()
	{
		return (input_layers[0]->get_inputs());
	}

	vector<OutputType*> & get_output_vector()
	{
		return outputs;
	}

	vector<OutputType*> & compute_outputs()
	{
		for (int i=0; i<layers.size(); i++)
		{
			layers[i]->calculate();
		}
		return outputs;
	}

	void td_reset_trace()
	{
		int i;
		for (i=0; i<layers.size(); i++)
		{
			layers[i]->td_reset_trace();
		}
	}

	void td_update_weights(InputType eta, InputType p_delta)
	{
		int i=layers.size()-1;
		for (i=0; i<layers.size(); i++)
		{
			layers[i]->td_update_weights(eta, p_delta);
		}
	}

	void td_calculate_gradient()
	{
		int i=layers.size()-1;
		layers[i]->td_calculate_delta();
		layers[i]->td_calculate_gradient();
		for (i=i-1; i>=0; i--)
		{
			layers[i]->backprop_delta();
			layers[i]->td_calculate_gradient();
		}
	}
	
	void td_calculate_trace(InputType lambda)
	{
		for (int i=0; i<layers.size(); i++)
		{
			layers[i]->td_calculate_trace(lambda);
		}
	}
private:
	vector<OutputType*> outputs;
};

#endif

