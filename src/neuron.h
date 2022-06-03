#ifndef neuron_h
#define neuron_h

#include <vector>
#define FPTYPE float
// #define RPROP

template <class T>
class ActivationFunction
{
	T operator()(T);
};

template <class T>
class SigmoidFunctor : public ActivationFunction<T>
{
public:
	T operator()(T val) { return 1/(1+exp(-val)); }
};



template <class TO>
class NeuronOutputBase;

template <class TI>
class NeuronInputBase
{
	template <class TI_, class TO_, template <class T2_> class ActivationFunctor>
		friend class Neuron;
public:
	void randomize(TI min, TI max)
	{
		for (int i=0; i<weights.size(); i++)
		{
			weights[i]=drand48()*(max-min)+min;
		}
	}

	NeuronInputBase()
	{		
		weights.push_back(0);
		eligibility_trace.push_back(0);
#ifdef RPROP
		rprop_last_trace.push_back(0);
		rprop_delta.push_back(.1);
#endif
		gradient.push_back(0);
		// activation_fnc = my_activation_fnc;
	}

	void add_connection(TI* input, TI weight)
	{
		weights.push_back(weight);
		eligibility_trace.push_back(0);
#ifdef RPROP
		rprop_last_trace.push_back(0);
		rprop_delta.push_back(.1);
#endif
		gradient.push_back(0);
		inputs.push_back(input);
	}

	void add_connection(TI* input)
	{
		add_connection(input, TI(0));
	}

	void add_inputs(const vector<TI*> &input_vec)
	{
		for (int i=0; i<input_vec.size(); i++)
		{
			add_connection(input_vec[i]);
		}
	}
public:
protected:
	vector<TI> weights;
	TI delta; // won't work if these are protected...	
	vector<TI*> inputs; // array of input value pointers
	TI sum;
	vector <TI> eligibility_trace;
#ifdef RPROP
	vector <TI> rprop_last_trace;
	vector <TI> rprop_delta;
#endif
	vector <TI> gradient;		
};


template <class TO>
class NeuronOutputBase
{
	template <class TI_, class TO_, template <class T2_> class ActivationFunctor>
		friend class Neuron;
public:
	TO get_output()
	{
		return output;
	}
protected:
	TO output; // can't compile w/o this public
	vector<NeuronInputBase<TO>*> output_neurons; // maintain list of neurons output to for backprop
	vector<int> output_indexes;  // along with the corresponding index... ugghh	
};


template <class TI, class TO, template <class T2> class ActivationFunctor>
class Neuron : public NeuronInputBase<TI> ,
			   public NeuronOutputBase<TO>
{
	using NeuronInputBase<TI>::add_connection;
	using NeuronInputBase<TI>::inputs;
	using NeuronInputBase<TI>::delta;
	using NeuronInputBase<TI>::eligibility_trace;
	using NeuronInputBase<TI>::gradient;
	using NeuronInputBase<TI>::sum;
	using NeuronInputBase<TI>::weights;
#ifdef RPROP
	using NeuronInputBase<TI>::rprop_last_trace;
	using NeuronInputBase<TI>::rprop_delta;
#endif
	using NeuronOutputBase<TO>::output;
	using NeuronOutputBase<TO>::output_neurons;
	using NeuronOutputBase<TO>::output_indexes;
public:
	typedef TI InputType;
	typedef TO OutputType;
	typedef ActivationFunctor<TO> ActivationType;
	static ActivationFunctor<TO> activation_functor;
	void add_connection(NeuronOutputBase<TI>* input_neuron, TI weight)
	{
		add_connection(&(input_neuron->output), weight);
		input_neuron->output_neurons.push_back(this);
		input_neuron->output_indexes.push_back(inputs.size()-1);
	}
	void add_connection(NeuronOutputBase<TI>* input_neuron)
	{
		add_connection(input_neuron, TI(0));
	}
    TO activation_fnc(TO val)
    {
           	// return 1/(1+exp(-val));
           	return activation_functor(val);
    }
    
    /* calculate_delta() and update_weights() for normal backpropagation */
/*
	void calculate_delta(TO target)
	{
		delta = TI(output*(1 - output)*(target - output));
	}
	void update_weights(TI eta)
	{
		weights[0] += eta*delta;
		for (int i=0; i<inputs.size(); i++)
		{
			weights[i+1] += eta * delta * (*(inputs[i]));
		}
	}
*/
	
	void calculate()
	{		
		sum=0;
		sum += weights[0];
		for (int idx=0; idx<inputs.size(); idx++)
		{
			sum += weights[idx+1] * (*(inputs[idx]));
		}
		output = activation_fnc(sum);
	}
	
	/* for now, this works with the "normal" delta and the td delta */
	void backprop_delta()
	{		
		NeuronInputBase<TO>* out_neuron;
		TO out_weight;
		TO backprop_sum = 0;
		for (int i=0; i<output_neurons.size(); i++)
		{
			out_neuron = output_neurons[i];
			out_weight = out_neuron->weights[output_indexes[i]+1];
			backprop_sum += out_neuron->delta * out_weight;
		}
		delta = output*(1 - output)*(backprop_sum);
	}
	void td_reset_trace()
	{
		for (int i=0; i<inputs.size()+1; i++)
		{
			eligibility_trace[i]=0;
		}
	}
	void td_calculate_delta()
	{		
		delta = TI(output*(1 - output));
	}
	void td_calculate_gradient()
	{		
		gradient[0]=delta;
		for (int i=0; i<inputs.size(); i++)
		{
			gradient[i+1]=delta * (*(inputs[i]));
		}
	}
	void td_calculate_trace(TI lambda)
	{		
		eligibility_trace[0]=eligibility_trace[0]*lambda + gradient[0];
		for (int i=0; i<inputs.size(); i++)
		{
			eligibility_trace[i+1]=eligibility_trace[i+1]*lambda + gradient[i+1];
		}
	}
	void td_update_weights(TI eta, TI p_delta)
	{
#ifdef RPROP
		for (int i=0; i<inputs.size()+1; i++)
		{
			TI current_trace = p_delta * eligibility_trace[i];
			if (current_trace*rprop_last_trace[i] <= 0) rprop_delta[i] *= .5; else rprop_delta[i] *= 1.2;
			if (rprop_delta[i]<.0001) rprop_delta[i]=.0001; else if (rprop_delta[i]>.5) rprop_delta[i]=.1;
			if (eligibility_trace[i]>0) weights[i] += eta*p_delta*rprop_delta[i];
			else if (eligibility_trace[i]<0) weights[i] -= eta*p_delta*rprop_delta[i];
			rprop_last_trace[i]=current_trace;
		}
#else		
		weights[0] += eta * p_delta * eligibility_trace[0];
		for (int i=0; i<inputs.size(); i++)
		{
			weights[i+1] += eta * p_delta * eligibility_trace[i+1];
		}
#endif
	}
	TO* get_output_p()
	{		
		return &(output);
	}
};

template <class TI, class TO, template <class T2> class ActivationFunctor>
ActivationFunctor<TO> Neuron<TI, TO, ActivationFunctor>::activation_functor;

#endif
