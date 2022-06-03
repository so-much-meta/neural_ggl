#ifndef input_layer_h
#define input_layer_h

#include "neuron.h"

template <class TI>
class InputLayer
{
public:
	InputLayer() {}

	InputLayer(int size)
	{
		TI* data=new TI[size];
		for (int i=0; i<size; i++)
		{
			add_input(data + i);
		}
	}

	void add_input(TI* input)
	{
		inputs.push_back(input);
	}

	void create_input()
	{
		add_input(new TI);
	}

	void set_value(int num, TI val)
	{
		*(inputs[num]) = val;
	}

	int size()
	{
		return inputs.size();
	}
	
	TI* get_input(int i) {return inputs[i];}
	vector<TI*> &get_inputs() {return inputs;}
private:
	vector<TI*> inputs;
};



#endif
