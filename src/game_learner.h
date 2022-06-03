#ifndef game_learner_h
#define game_learner_h

#include "game.h"
#include "game_player.h"

template <class NETWORK>
class GameLearner
{
public:
	typedef typename NETWORK::InputType InputType;
	typedef typename NETWORK::OutputType OutputType;
	typedef typename NETWORK::ActivationType ActivationType;

	void init(NeuralPlayer<NETWORK>* _player, Game<InputType, OutputType>* _game)
	{
		player = _player;
		game = _game;
		eta=0.1;
		lambda=.1;
	}
	void set_lambda( InputType lambda_val )
	{
		lambda = lambda_val;
	}
	void set_eta(InputType eta_val)
	{ 
		eta = eta_val;
	}
	void learn(int iters);
private:
	InputType eta;
	InputType lambda;
	NeuralPlayer<NETWORK>* player;
	Game<InputType, OutputType>* game;
};

#endif
