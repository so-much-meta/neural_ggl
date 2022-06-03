#ifndef game_player_h
#define game_player_h

#include "backprop_network.h"

template <class TI, class TO> class Game;
typedef TDNetwork<Layer<Neuron<FPTYPE, FPTYPE, SigmoidFunctor> > > NetworkType;

template <class TI, class TO>
class GamePlayer
{
public:
	virtual int get_move(Game<TI, TO>* game)=0;
private:
};

template <class NETWORK>
class NeuralPlayer : public GamePlayer<typename NETWORK::InputType,
					 				   typename NETWORK::OutputType>
{
public:
	typedef typename NETWORK::InputType InputType;
	typedef typename NETWORK::OutputType OutputType;
	typedef typename NETWORK::ActivationType ActivationType;
	
	NeuralPlayer(NetworkType* nw, int num_moves_possible)
	{
		network=nw;
		input_vector = &(network->get_input_vector());
		output_vector = &(network->get_output_vector());
		next_state_values.resize(num_moves_possible);
		// moves.resize(num_moves_possible);
		move_probs.resize(num_moves_possible);
		choose_best=0;
		choose_exp_factor = 30;
		// game_finished=0;
	}
	
	OutputType get_max_next_state_value() {return max_next_state_value;}
	OutputType get_min_next_state_value() {return min_next_state_value;}
	
	FPTYPE get_choice_value() {return next_state_values[choice_index];}
	FPTYPE get_prob_choice() {return prob_choice;}
	// int get_num_moves_avail() {return num_moves_avail;}

	void search_moves(Game<InputType, OutputType>* game);
	void set_choice_params(int _choose_best, FPTYPE _choose_exp_factor)
	{ 
		choose_best = _choose_best;
		choose_exp_factor = _choose_exp_factor;
	}
	int get_move(Game<InputType, OutputType>* game);
	NetworkType* get_network() { return network; }
protected:
	NetworkType* network;
	vector<InputType*> *input_vector;
	vector<OutputType*> *output_vector;
	vector<OutputType> next_state_values;
	vector<FPTYPE> move_probs;
	FPTYPE prob_choice;
	FPTYPE move_probs_sum;
	OutputType max_next_state_value;
	OutputType min_next_state_value;
	int choice_index;
	bool choose_best;
	FPTYPE choose_exp_factor;
};

/*
class GtpPlayer : public GamePlayer
{
	int get_move(Game* game)
	{
	}
}
*/

#endif

