#ifndef game_h
#define game_h

#include "game_player.h"

extern int printmoves;

template <class TI, class TO>
class GameState
{
public:
	GameState(int move_size) : moves(move_size) {}
	GameState() : moves(1) { cerr << "1c\n";}
	int game_finished;
	TO game_score;
	int last_move;
	int current_player;
	int move_iter_current;
	int num_moves_avail;
	int moves_dirty;
	vector<int> moves;
};

template <class TI, class TO>
class Game
{
protected:
	Game() { num_saved=0; }
public:
	typedef GamePlayer<TI,TO> GamePlayerType;
	typedef Game<TI,TO> GameType;
	Game(int history) { num_saved=0; game_state = new GameState<TI,TO>; saved_states = new GameState<TI,TO>[history];}
	virtual void inject(vector<TI*> &input_vector)=0;
	virtual void reset(GamePlayer<TI,TO>* p0, GamePlayer<TI,TO>* p1)
	{
		players[0] = p0;
		players[1] = p1;
		game_state->current_player = 0;
		game_state->game_finished = 0;
		game_state->moves_dirty=1;
	}
	virtual void print_board()=0;
	virtual void find_moves()=0; /* { num_moves_avail=0; } */
	int get_moves_avail()
	{ 
		if (game_state->moves_dirty)
			find_moves();
		game_state->moves_dirty=0; 
		return game_state->num_moves_avail;
	}
	int is_done() { return game_state->game_finished; }
	int get_current_player() { return game_state->current_player; }
	TO score_for_current()
	{
		if (game_state->current_player==0) return game_state->game_score;
		else return 1 - game_state->game_score;
	}
	virtual void save_board()
    {
		saved_states[num_saved] = *game_state;
		num_saved++;
    }
	virtual void restore_board()
	{
		num_saved--;
		*game_state = saved_states[num_saved];
	}
	virtual void play_move (int move)
	{
		game_state->moves_dirty=1;
		game_state->last_move=move;
	}
	void play_move(int move, int color)
	{
		game_state->current_player=color;
		play_move(move);
	}
	int get_last_move()
	{
		return game_state->last_move;
	}
	void init_move_iter()
	{
		get_moves_avail();
		game_state->move_iter_current=0;
	}
	int move_iter_first()
	{
		init_move_iter();
		if (game_state->move_iter_current >= game_state->num_moves_avail)
		{
			return 0;
		}
		else
		{
			save_board(); // save board only once...
			if (printmoves) cerr << "trying move at " << game_state->moves[game_state->move_iter_current] << endl;
			play_move(game_state->moves[game_state->move_iter_current]);
			return 1;
		}
	}
	/*
	int move_iter_current_move()
	{
		return game_state->moves[game_state->move_iter_current-1];
	}
	*/
	int num_moves_avail()
	{
		return game_state->num_moves_avail;
	}
	int move_at_index(int i)
	{
		return game_state->moves[i];
	}
	int move_iter_index()
	{
		return game_state->move_iter_current;
	}
	int move_iter_next()
	{
		if (printmoves) cerr << "move iter current=" << game_state->move_iter_current <<  ", avail=" << game_state->num_moves_avail << endl;

		if (game_state->move_iter_current + 1 >= game_state->num_moves_avail)
		{
			if (game_state->num_moves_avail > 0) restore_board();
			return 0;
		}
		else
		{
			saved_states[num_saved-1].move_iter_current++;
			
			restore_board();
			num_saved++; // next call to restore_board restores same board...
			// cerr << "Move iter current=" << game_state->move_iter_current << endl;
			play_move(game_state->moves[game_state->move_iter_current]);
			
			return 1;
		}
	}
	void play_one_move()
	{
		//cerr << "checkb\n";
		int move=players[game_state->current_player]->get_move(this);
		// cerr << "checkb\n";
		play_move(move);
		// cerr << "checkb\n";
	}
protected:
	GameState<TI,TO>* game_state;
	GameState<TI,TO>* saved_states;
	GamePlayer<TI,TO>* players[2];
	
	int num_saved;
};

#endif

