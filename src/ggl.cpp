// neural.cpp : main project file.

#include <time.h>
#include <stdio.h>
#include <math.h>
#include <list>
#include <vector>
#include <iostream>

using namespace std;

#include <time.h>
#include "game_learner.h"
#include "othello.h"
#include "tictactoe.h"
#include "gtp_neural_interface.h"


#include "game_learner.cpp"
#include "game_player.cpp"
#include "gtp_neural_interface.cpp"

#define DEFAULT_HIDDEN_SIZE 100
#define DEFAULT_LAMBDA_VAL 0.2
#define DEFAULT_ETA_VAL 0.2
#define LEARN_CYCLES 500
#define LEARN_ITERS 50


#ifndef HIIDEN_SIZE
#define HIDDEN_SIZE DEFAULT_HIDDEN_SIZE
#endif

#ifndef LAMBDA_VAL
#define LAMBDA_VAL DEFAULT_LAMBDA_VAL
#endif

#ifndef ETA_VAL
#define ETA_VAL DEFAULT_ETA_VAL
#endif

#define NETWORK_FPTYPE float

typedef TDNetwork<Layer<Neuron<NETWORK_FPTYPE, NETWORK_FPTYPE, SigmoidFunctor> > > NetworkType;
typedef Game<NETWORK_FPTYPE, NETWORK_FPTYPE> GameType;
typedef GameLearner<NetworkType> GameLearnerType;
typedef NeuralPlayer<NetworkType> NeuralPlayerType;

int main()
{
//	srand((unsigned)(time(NULL)));
	srand48((unsigned)(time(NULL)));
	GameLearnerType game_learner;
	NetworkType network;
	char inputstr[100];
#if 1
	Othello othello(2);

	network.create_ff_layer(128);
	network.create_ff_layer(HIDDEN_SIZE);
#ifdef HIDDEN_SIZE_2
	network.create_ff_layer(HIDDEN_SIZE_2);
#endif	
	network.create_ff_layer(1);
	
	network.create_output_vector();
	network.randomize(-.1, .1);
	NeuralPlayerType neural_player(&network, 65);	
	game_learner.init(&neural_player, &othello);
	game_learner.set_lambda(LAMBDA_VAL);
	game_learner.set_eta(ETA_VAL);	
#else
	TicTacToe tictac;

	network.create_ff_layer(9);
	network.create_ff_layer(24);
	//network.create_ff_layer(4);	
	network.create_ff_layer(1);
	network.create_output_vector();
	network.randomize(-.1, .1);

	NeuralPlayer neural_player(&network, 10);	
	game_learner.init(&neural_player, &tictac);
#endif
	FPTYPE cpu_time_used;
	clock_t start, end;
	cerr << "Starting...\n";
	for (int i=0; i < LEARN_CYCLES; i++)
	{
		cerr << "Cycle: " << i << endl;
		neural_player.set_choice_params(0, 30);
		start = clock();
		game_learner.learn(LEARN_ITERS);
		end = clock();
		cerr << "Done learning..." << endl;
		cpu_time_used = ((FPTYPE) (end - start)) / CLOCKS_PER_SEC;
		neural_player.set_choice_params(1, 135);
		cerr << "Iters = " << (i+1)*LEARN_ITERS << ": time="<<cpu_time_used << endl;
		// play_gtp_game( &neural_player, &othello, 0);
		// play_gtp_game( &neural_player, &othello, 1);
	}
	
	close_gtp_file();

	cout << "quit\n";
	getresponse(inputstr);
	char mychar;
	// scanf("%c", &mychar);
	return 0;
}
