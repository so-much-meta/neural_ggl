#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <list>
#include <vector>

using namespace std;
#include "game_learner.h"
#include "othello.h"
#include "tictactoe.h"
#include "gtp_neural_interface.h"

#define GTP_PASS_STR "= pass"
const int GTP_PASS_LEN=strlen(GTP_PASS_STR);

#define INPUT_STR_LEN 100

void getresponse(char str[])
{
	while (1)
	{
		cin.getline(str, INPUT_STR_LEN);
		if (str[0]=='=') break;
		if (str[0]=='?')
		{
			cerr << "OOPS: " << str << endl;
			break;
		}
	}
}
int printmoves=0;
const char* player_colors[]={"black", "white"};

static fstream* p_logfile=0;

void close_gtp_file()
{
	if (!p_logfile) return;
	fstream &myfile = *p_logfile;
	myfile.close();
}

template<class PLAYER, class GAME>
int play_gtp_game(PLAYER* neural_player, GAME* game, int color)
{
	int pass=0;
	char inputstr[INPUT_STR_LEN];
	int compresign=0;
	player_t cur_player=BLACK;
	// printmoves=1;
	if (!p_logfile) p_logfile=new fstream("gtp_log.txt", ios::out);
	fstream &myfile = *p_logfile;
	// ostream &myfile = cerr;
	game->reset( neural_player, neural_player );
	cur_player = player_t(game->get_current_player());
	cout << "clear_board\n";
	if (printmoves) cerr << "playing...\n";
	getresponse(inputstr);
	pass=0;	
	while (!game->is_done())
	{
		if (cur_player==color)
		{
			game->play_one_move();
			int last_move = game->get_last_move();
			if (last_move != -1)
			{
				char col=last_move%8 + 'A';
				char row=last_move/8 + '1';
				cout << "play " << player_colors[cur_player] << " " << col << row << endl;
				if (printmoves) cerr << "play " << player_colors[cur_player] << " " << col << row << endl;
				getresponse(inputstr);
				if (printmoves) cerr << ">> " << inputstr << endl;
				// myfile << "Player " << player_colors[cur_player] << " move = " << col << row << " (NEURAL)" << endl;
				if (printmoves) cerr << "Player " << player_colors[cur_player] << " move = " 
					 << col << row << " (NEURAL) score=" << 1-neural_player->get_min_next_state_value() << endl;
				if (printmoves) game->print_board();
			}
		}
		else
		{
			if (cur_player==BLACK) {
				cout << "genmove b\n";
				if (printmoves) cerr << "genmove b\n";
			}
			else {
				cout << "genmove w\n";
				if (printmoves) cerr << "genmove w\n";
			}
			getresponse(inputstr);
			if (printmoves) cerr << ">> " << inputstr << endl;
			// myfile << "Player " << player_colors[cur_player] << " move = " << inputstr+2 << endl;
			if (printmoves) cerr << "Player " << player_colors[cur_player] << " move = " << inputstr+2 << endl;
			if (printmoves) game->print_board();
			if (!strncmp(inputstr, GTP_PASS_STR, GTP_PASS_LEN))
			{
				int move=-1;
				game->play_move(move);
				// pass=1;
			}
			else
			{
				int move=(*(inputstr+3)-'1')*8 + (*(inputstr+2)-'A');
				if (move>=64) //computer resigns....
				{
					compresign=1;
					break;
				}
				game->play_move(move);
				// pass=0;
			}
		}
		cur_player = player_t(game->get_current_player());
	}
	if (!compresign)
	{
		cout << "final_score\n";
		getresponse(inputstr);
		cerr << "Neural = " << player_colors[color] << ": Final score = " << inputstr+2 << endl;
		if (inputstr[0]=='?')
		{
			cerr << "OOPS... cur_player=" << int(cur_player) << endl;
			game->print_board();
			cout << "genmove w\n";
			getresponse(inputstr);
			cerr << inputstr << endl;
			cout << "genmove b\n";
			getresponse(inputstr);
			cerr << inputstr << endl;
			cout << "genmove w\n";
			getresponse(inputstr);
			cerr << inputstr << endl;
			cout << "genmove b\n";
			getresponse(inputstr);
			cerr << inputstr << endl;
			cout << "genmove w\n";
			getresponse(inputstr);
			cerr << inputstr << endl;
			cout << "genmove b\n";
			getresponse(inputstr);
			cerr << inputstr << endl;				
			cout << "final_score\n";
			getresponse(inputstr);
			cerr << inputstr;
		}
	}
	else
	{
		cerr << "GTP opponent resigned!!!!\n";
	}
	// cerr << "Last believed score = " << 1-neural_player->get_min_next_state_value() << endl;
	if (compresign)
	{
		myfile << "+64\tR" << endl;
	}
	else if (inputstr[2]=='B' && color==0 || inputstr[2]=='W' && color==1)
	{ // I won!!!
		myfile << "+" << inputstr+4 << endl;
	}
	else if (inputstr[2]=='0')
	{ // I tied...
		myfile << "0" << endl;
	}
	else
	{ // I lost :(
		myfile << "-" << inputstr+4 << endl;
	}
		
	// myfile << "Final score = " << inputstr+2 << endl;
	myfile.flush();
	// printmoves=0;
	return 0;
	// cout << "quit\n";
	// myfile.close();	
}

