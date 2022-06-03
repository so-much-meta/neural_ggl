#ifndef othello_h
#define othello_h

#include "game.h"

extern int printmoves;
#define OTHELLO_FPTYPE float

class OthelloGameState : public GameState<OTHELLO_FPTYPE, OTHELLO_FPTYPE>
{
public:
	OthelloGameState() : GameState<OTHELLO_FPTYPE,OTHELLO_FPTYPE>(65) {}
	int board[8][8];
	int last_cantgo;
};

class Othello : public Game<OTHELLO_FPTYPE, OTHELLO_FPTYPE>
{
public:
	Othello(int history) : continuous_score(0)
	{ 
		game_state = new OthelloGameState;
		saved_states = new OthelloGameState[history];
	}

	void print_board()
	{
		if (!printmoves) return;
		cerr << "\n";
		int (&board)[8][8] = ((OthelloGameState*)game_state)->board;
		for (int i=0; i<8; i++)
		{
			for (int j=0; j<8; j++)
			{
				switch (board[i][j])
				{
				case 1:
					cerr << "X";
					break;
				case -1:
					cerr << "O";
					break;
				default:
					cerr << ".";
					break;
				}
			}
			cerr << "\n";
		}
	}

	void evaluate()
	{
		int piece_tally=0;
		int (&board)[8][8] = ((OthelloGameState*)game_state)->board;
		for (int i=0; i<8; i++)
		{
			for (int j=0; j<8; j++)
			{
				piece_tally+=board[i][j];
			}
		}
		if (continuous_score)
		{
			((OthelloGameState*)game_state)->game_score=piece_tally/128.0+.5; /* put in 0..1 range */
		}
		else
		{
			if (piece_tally>0) ((OthelloGameState*)game_state)->game_score=1.0;
			else if (piece_tally==0) ((OthelloGameState*)game_state)->game_score=0.5;
			else ((OthelloGameState*)game_state)->game_score=0;
		}
	}

	void reset(GamePlayerType* p0, GamePlayerType* p1)
	{
		GameType::reset(p0, p1);
		int (&board)[8][8] = ((OthelloGameState*)game_state)->board;
		
		((OthelloGameState*)game_state)->last_cantgo=0;
		for (int i=0; i<8; i++)
		{
			for (int j=0; j<8; j++)
			{
				board[i][j]=0;
			}
		}
		board[3][3]=-1;
		board[4][4]=-1;
		board[3][4]=1;
		board[4][3]=1;
	}

	void make_move(int i, int j, int di, int dj)
	{
		if (!move_avail(i, j, di, dj)) return;
		int (&board)[8][8] = ((OthelloGameState*)game_state)->board;
		int xup=1-2*((OthelloGameState*)game_state)->current_player;
		for (int ii=i+di, jj=j+dj; 
				 ii>=0 && ii<8 && jj>=0 && jj<8;
				 ii+=di, jj+=dj)
		{
			if (board[ii][jj]==xup || board[ii][jj]==0) return;
			board[ii][jj]=xup;
		}
	}

	void make_move(int i, int j)
	{
		int (&board)[8][8] = ((OthelloGameState*)game_state)->board;
		int xup=1-2*((OthelloGameState*)game_state)->current_player;;
		make_move(i, j, -1, -1);
		make_move(i, j, -1, 0);
		make_move(i, j, -1, 1);
		make_move(i, j, 0, -1);
		make_move(i, j, 0, 1);
		make_move(i, j, 1, -1);
		make_move(i, j, 1, 0);
		make_move(i, j, 1, 1);
		board[i][j]=xup;
	}

	int move_avail(int i, int j, int di, int dj)
	{
		int target_sat=0;
		int (&board)[8][8] = ((OthelloGameState*)game_state)->board;
		int xup=1-2*((OthelloGameState*)game_state)->current_player;;

		for (int ii=i+di, jj=j+dj; 
				 ii>=0 && ii<8 && jj>=0 && jj<8;
				 ii+=di, jj+=dj)
		{
			if (target_sat)
			{
				if (board[ii][jj]==xup) return 1;
				if (board[ii][jj]==0) return 0;
			}
			else
			{
				if (board[ii][jj]==-xup)
				{
					target_sat=1;
				}
				else 
				{
					return 0;
				}
			}
		}
		return 0;
	}

	int move_avail(int i, int j)
	{
		int (&board)[8][8] = ((OthelloGameState*)game_state)->board;
		
		if (board[i][j]) return 0;
		if (move_avail(i, j, -1, -1)) return 1;
		if (move_avail(i, j, -1, 0)) return 1;
		if (move_avail(i, j, -1, 1)) return 1;
		if (move_avail(i, j, 0, -1)) return 1;
		if (move_avail(i, j, 0, 1)) return 1;
		if (move_avail(i, j, 1, -1)) return 1;
		if (move_avail(i, j, 1, 0)) return 1;
		if (move_avail(i, j, 1, 1)) return 1;
		return 0;
	}

	void play_move(int move)
	{
		GameType::play_move(move);
		if (move==-1)
		{
			if (((OthelloGameState*)game_state)->last_cantgo)
			{
				((OthelloGameState*)game_state)->game_finished=1;
				evaluate();
				return;
			}
			((OthelloGameState*)game_state)->current_player =
					1-((OthelloGameState*)game_state)->current_player;
			((OthelloGameState*)game_state)->last_cantgo=1;
		}
		else
		{
			((OthelloGameState*)game_state)->last_cantgo=0;
			make_move(move/8, move%8);
			((OthelloGameState*)game_state)->current_player=
					1-((OthelloGameState*)game_state)->current_player;
		}
	}

	void find_moves()
	{
		((OthelloGameState*)game_state)->num_moves_avail=0;
		for (int i=0; i<8; i++)
		{
			for (int j=0; j<8; j++)
			{
				if (move_avail(i,j))
				{
					if (printmoves)
						cerr << "move " << i << ", " << j << " available!" << endl;
					((OthelloGameState*)game_state)->moves[((OthelloGameState*)game_state)->num_moves_avail]=8*i+j;
					((OthelloGameState*)game_state)->num_moves_avail++;
				}
			}
		}
		/* if no moves found, then make only possible move a pass = -1 */
		if (((OthelloGameState*)game_state)->num_moves_avail==0)
		{
			((OthelloGameState*)game_state)->moves[0] = -1;
			((OthelloGameState*)game_state)->num_moves_avail=1;
		}
	}

	void save_board()
	{
		((OthelloGameState*)saved_states)[num_saved] = *((OthelloGameState*)game_state);
		num_saved++;
		/*
		Game::save_board();
		saved_cantgo=last_cantgo;
		for (int i=0; i<8; i++)	
			for (int j=0; j<8; j++)
				saved_board[i][j]=board[i][j];
		*/
	}

	void restore_board()
	{
		num_saved--;
		*((OthelloGameState*)game_state) = ((OthelloGameState*)saved_states)[num_saved];
		/*
		Game::restore_board();
		last_cantgo=saved_cantgo;
		for (int i=0; i<8; i++)
			for (int j=0; j<8; j++)
				board[i][j]=saved_board[i][j];
		*/
	}
	
	void inject(vector<OTHELLO_FPTYPE*> &input_vector)
	{
		int (&board)[8][8] = ((OthelloGameState*)game_state)->board;
		if (((OthelloGameState*)game_state)->current_player==0)
		{
			for (int i=0; i<64; i++)
			{
				if (board[i/8][i%8]==1) *(input_vector[2*i])=1; else *(input_vector[2*i])=0;
				if (board[i/8][i%8]==-1) *(input_vector[2*i+1])=1; else *(input_vector[2*i+1])=0;
			}
		}
		else
		{
			for (int i=0; i<64; i++)
			{
				if (board[i/8][i%8]==-1) *(input_vector[2*i])=1; else *(input_vector[2*i])=0;
				if (board[i/8][i%8]==1) *(input_vector[2*i+1])=1; else *(input_vector[2*i+1])=0;
			}
		}
	}
private:
	int continuous_score;
	/*	
	int board[8][8];
	int saved_board[8][8];
	int saved_cantgo;
	int last_cantgo;
	*/
};

#endif
