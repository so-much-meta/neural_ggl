#ifndef tictactoe_h
#define tictactoe_h

#include "game.h"

#if 0
class TicTacToe : public Game
{
public:
	TicTacToe() {}

	void print_board()
	{
		printf("\n");
		for (int i=0; i<3; i++)
		{
			for (int j=0; j<3; j++)
			{
				switch (board[i][j])
				{
				case 1:
					printf("X");
					break;
				case -1:
					printf("O");
					break;
				default:
					printf(".");
					break;
				}
			}
			printf("\n");
		}
	}

	void evaluate()
	{
		if (board[1][1])
		{
			if ((board[1][1]==board[0][0] && board[1][1]==board[2][2]) ||
				(board[1][1]==board[0][2] && board[1][1]==board[2][0]) ||
				(board[1][1]==board[1][0] && board[1][1]==board[1][2]) ||
				(board[1][1]==board[0][1] && board[1][1]==board[2][1]))
			{
				game_score=board[1][1]==1 ? 1 : 0;
				game_finished=1;
				return;
			}
		}	
		if (board[0][0])
		{
			if ((board[0][0]==board[0][1] && board[0][1]==board[0][2]) ||
				(board[0][0]==board[1][0] && board[1][0]==board[2][0]))
			{
				game_score=board[0][0]==1 ? 1 : 0;
				game_finished=1;
				return;
			}
		}
		if (board[2][2])
		{
			if ((board[2][2]==board[2][1] && board[2][1]==board[2][0]) ||
				(board[2][2]==board[1][2] && board[1][2]==board[0][2]))
			{
				game_score=board[2][2]==1 ? 1 : 0;
				game_finished=1;
				return;
			}
		}
		game_finished=0;
		return;		
	}

	void reset(GamePlayer* p0, GamePlayer* p1)
	{
		Game::reset(p0, p1);
		for (int i=0; i<3; i++)
		{
			for (int j=0; j<3; j++)
			{
				board[i][j]=0;
			}
		}
		moves.resize(10);
	}
	
	void make_move(int i, int j)
	{
		int xup=1-2*current_player;
		board[i][j]=xup;
	}

	int move_avail(int i, int j)
	{
		if (board[i][j]==0) return 1; else return 0;
	}

	void play_move(int move)
	{
		Game::play_move(move);
		if (move==-1)
		{
			if (game_finished) return;
			game_finished=1;
			game_score=0.5;
			return;
		}
		else
		{
			make_move(move/3, move%3);
			current_player=1-current_player;
		}
	}

	void find_moves()
	{
		num_moves_avail=0;
		evaluate();
		if (game_finished)
		{
			moves[0] = -1;
			num_moves_avail=1;
			return;
		}	
		for (int i=0; i<3; i++)
		{
			for (int j=0; j<3; j++)
			{
				if (move_avail(i,j))
				{
					moves[num_moves_avail]=3*i+j;
					num_moves_avail++;
				}
			}
		}
		/* if no moves found, then make only possible move a pass = -1 */
		if (num_moves_avail==0)
		{
			moves[0] = -1;
			num_moves_avail=1;
		}
	}

	void save_board()
	{
		Game::save_board();
		saved_player=current_player;
		for (int i=0; i<3; i++)	
			for (int j=0; j<3; j++)
				saved_board[i][j]=board[i][j];
	}

	void restore_board()
	{
		Game::restore_board();
		current_player=saved_player;
		for (int i=0; i<3; i++)
			for (int j=0; j<3; j++)
				board[i][j]=saved_board[i][j];
	}
	
	void inject(vector<FPTYPE*> &input_vector)
	{
		if (current_player==0)
		{
			for (int i=0; i<9; i++)
			{
				*(input_vector[i])=board[i/3][i%3];
			}
		}
		else
		{
			for (int i=0; i<9; i++)
			{
				*(input_vector[i])=-board[i/3][i%3];
			}
		}
	/*
		if (current_player==0)
		{
			for (int i=0; i<9; i++)
			{
				if (board[i/3][i%3]==1) *(input_vector[2*i])=1; else *(input_vector[2*i])=0;
				if (board[i/3][i%3]==-1) *(input_vector[2*i+1])=1; else *(input_vector[2*i+1])=0;
			}
		}
		else
		{
			for (int i=0; i<9; i++)
			{
				if (board[i/3][i%3]==-1) *(input_vector[2*i])=1; else *(input_vector[2*i])=0;
				if (board[i/3][i%3]==1) *(input_vector[2*i+1])=1; else *(input_vector[2*i+1])=0;
			}
		}
	*/
	}
private:
	int board[3][3];
	int saved_board[3][3];
};
#endif

#endif
