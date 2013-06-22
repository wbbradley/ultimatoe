#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tr1/array>

#define X 1
#define Y 2

typedef int piece_t;
typedef int player_t;

struct board_t
{
	std::tr1::array<piece_t, 9> spaces;
	void clear()
	{
		for (auto &space : spaces)
			space = 0;
	}
};

struct meta_board_t
{
	std::tr1::array<board_t, 9> boards;
	player_t next;

	meta_board_t() : next(X)
	{
		for (auto &board : boards)
			board.clear();
	}
};


int main(int argc, char *argv[])
{
	// setup a board
	meta_board_t meta_board;

	// set turn to p1
	// do
		// play and remember valid move from current player
		// if game over quit
		// determine next possible moves
	// while true

	return EXIT_SUCCESS;
}

