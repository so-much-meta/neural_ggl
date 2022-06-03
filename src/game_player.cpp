#include <time.h>
#include <stdio.h>
#include <math.h>
#include <list>
#include <vector>
// #include <conio.h>
#include <iostream>

using namespace std;

#include "game.h"
#include "game_player.h"

/* NeuralPlayer implementation */
 
				 /*
FPTYPE NeuralPlayer::evaluate(Game* game, int depth)
{
	max_next_state_value=-1000;
	min_next_state_value=1000;
	int current_move_index=0;
	FPTYPE current_next_state_values[64];

	num_moves_avail=0;
	game->init_move_iter();
	while (game->move_iter_next())
	{
		game->inject(*input_vector);
		if (depth==0)
		{
			network->compute_outputs();
			current_move_index = num_moves_avail;

			moves[current_move_index] = game->move_iter_current_move();

			current_next_state_values[current_move_index]=*((*output_vector)[0]);
			if (current_next_state_values[current_move_index] > max_next_state_value)
				max_next_state_value = current_next_state_values[current_move_index];
			if (current_next_state_values[current_move_index] < min_next_state_value)
				min_next_state_value = current_next_state_values[current_move_index];

			num_moves_avail++;
		}
		else
		{
			return evaluate
		}
	}
}
*/

extern int printmoves;

template <class NETWORK>				 
void NeuralPlayer<NETWORK>::search_moves (
					Game<typename NETWORK::InputType,
					     typename NETWORK::OutputType>* game)
{
	max_next_state_value=-1000;
	min_next_state_value=1000;
	int current_move_index=0;
	
	if (!game->move_iter_first()) return;
	do {
		//cerr << "checka\n";
		game->inject(*input_vector);
		network->compute_outputs();
		//cerr << "checka\n";
		current_move_index = game->move_iter_index();

		next_state_values[current_move_index]=*((*output_vector)[0]);
		if (printmoves) cerr << "move at " << current_move_index << ": " << next_state_values[current_move_index] << endl;
		// cerr << "Value=" << next_state_values[current_move_index] << endl; 
		if (next_state_values[current_move_index] > max_next_state_value)
			max_next_state_value = next_state_values[current_move_index];
		if (next_state_values[current_move_index] < min_next_state_value)
			min_next_state_value = next_state_values[current_move_index];
		//cerr << "checka\n";
	} while (game->move_iter_next());
}

/*
int NeuralPlayer::get_move(Game* game)
=> find move that minimizes opponents position
*/


template <class NETWORK>
int NeuralPlayer<NETWORK>::get_move(
							Game<typename NETWORK::InputType,
							     typename NETWORK::OutputType>* game)
{
	int numchoices=10; // test
	int move_choice=0;
	FPTYPE pchoice;

	search_moves(game);
	move_probs_sum=0;
	if (game->num_moves_avail()==0)
	{
		prob_choice = 1;
		return -1;
	}
	// cerr << "Num moves avail=" << game->num_moves_avail() << endl; 
	if (printmoves) cerr << "num moves avail=" << game->num_moves_avail() << endl;
	if (printmoves) cerr << "min next state = " << min_next_state_value << endl;
	for (int i=0; i < game->num_moves_avail(); i++)
	{
		if (!choose_best)
		{
			move_probs[i] = exp(10+choose_exp_factor*(min_next_state_value - next_state_values[i]));
		}
		else
		{
			move_probs[i] = (next_state_values[i] == min_next_state_value) ? 1 : 0;
			if (printmoves) cerr << "next state value= " << next_state_values[i] << endl;
			if (printmoves) cerr << "  " << i << " is ok... prob=" << move_probs[i] << endl;
		}
		move_probs_sum += move_probs[i];
		if (move_probs[i]>0) numchoices++;
	}
	pchoice = move_probs_sum*drand48();
	if (printmoves) cerr << "initial pchoice = " << pchoice << endl;
	for (int i=0; i < game->num_moves_avail(); i++)
	{
		if (move_probs[i]>0)
		{
			pchoice -= move_probs[i];
			if (printmoves) cerr << "pchoice = " << pchoice << endl;
			numchoices--;
			if (pchoice<0 || numchoices==0)
			{
				prob_choice = move_probs[i]/move_probs_sum;
				choice_index = i;
				if (printmoves) cerr << "choosing index i=" << i << ": " << game->move_at_index(i) << endl;
				return game->move_at_index(i);
			}
		}
	}
	/* should never get here... */
	cerr << "OOPS!!! - didn't get a move\n";
	return 0;
}
