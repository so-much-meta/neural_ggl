#include <time.h>
#include <stdio.h>
#include <math.h>
#include <list>
#include <vector>
// #include <conio.h>
#include <iostream>
#include <cmath>

using namespace std;

#include "game_learner.h"
#include "backprop_network.h"
#include "game_player.h"

char player_symbols[2]={'X', 'O'};

#define PRINT_ITERS 300
#define QUIET

template <class NETWORK>
void GameLearner<NETWORK>::learn(int iters)
{
	OutputType last_output=0;
	int user_input=0;
	int last_up=0;
	int xup=0;
	OutputType target;
	vector<InputType*> *input_vector;
	vector<OutputType*> *output_vector;

	FPTYPE last_prob=1;
	FPTYPE current_prob=1;


	NETWORK &mynetwork = *(player->get_network());
	
	input_vector = &(mynetwork.get_input_vector());
	output_vector = &(mynetwork.compute_outputs());

	for (int i=0; i<iters; i++)
	{
		game->reset(player, player);
		mynetwork.td_reset_trace();
#ifdef ENABLE_VAR_LAMBDA
		InputType current_lambda;
#endif		

		//if (i<10000) eta=.01; else if (i<50000) eta=.02; else if (i<100000) eta=.01; else eta=.001;
		// if (kbhit()) { _getch(); user_input=1; }
		// else user_input=0;
		/*evaluate();*/
		
		/* create initial eligibility trace */
		
		game->inject(*input_vector);
		mynetwork.compute_outputs();
		mynetwork.td_calculate_gradient();
		last_output=*((*output_vector)[0]);

		while (true)
		{
			FPTYPE max_value, min_value, choice_value;
			int moves_avail;

#ifndef QUIET
			if (!(i%PRINT_ITERS))
			{
				game->print_board();
				printf("\n\nCurrent player=%c, output=%3.4f\n", player_symbols[game->get_current_player()], last_output);
			}
#endif
			moves_avail = game->get_moves_avail();
			game->play_one_move();
									
			if (game->is_done()) break;

#ifdef ENABLE_VAR_LAMBDA	
			current_lambda = current_prob*lambda; // last_prob
#endif			

			last_prob = current_prob;
			current_prob = player->get_prob_choice();
#ifdef ENABLE_VAR_LAMBDA
			mynetwork.td_calculate_trace(current_lambda);
#else						
			mynetwork.td_calculate_trace(lambda);
#endif			
			max_value = 1-player->get_min_next_state_value();
			min_value = 1-player->get_max_next_state_value();
			choice_value = 1-player->get_choice_value();
			target=max_value;
			//target = choice_value;
#ifndef QUIET
			if (!(i%PRINT_ITERS))
			{
				printf("Done evaluating possibilities: max=%3.4f, min=%3.4f, chose=%3.4f\n", max_value, min_value, choice_value);
				printf("Moves avail=%d, prob choice=%3.4f\n", player->get_num_moves_avail(), player->get_prob_choice());
				printf("Updating weights: target=%3.4f, last output=%3.4f\n", target, last_output);
			}
#endif
			mynetwork.td_update_weights(eta, target-last_output);
			
			game->inject(*input_vector);
			mynetwork.compute_outputs();
			mynetwork.td_calculate_gradient();
			last_output=*((*output_vector)[0]);
		}
		
		target = game->score_for_current();

#ifndef QUIET
		if (!(i%PRINT_ITERS)) {
			game->print_board();
			printf("Game finished: current_player=%d, score=%3.4f\n", player_symbols[game->get_current_player()], target);
			printf("Updating weights: target=%3.4f, last output=%3.4f\n", target, last_output);
		}
#endif

		mynetwork.td_calculate_trace(1);
		/* this time the player hasn't changed */
		mynetwork.td_update_weights(eta, target-last_output);
	}		
}
