#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <array>

#define X 1
#define O 2

typedef int piece_t;
typedef int player_t;

struct move_t
{
	move_t(int b, int s) : board(b), space(s) {}
	int board;
	int space;
	bool exists() const { return board >= 0 and space >= 0; }
};

struct board_t
{
	std::array<piece_t, 9> spaces;
	void clear()
	{
		for (auto &space : spaces)
			space = 0;
	}

	bool available(int space) const
	{
		return !spaces[space];
	}
};

struct meta_board_t
{
	std::array<board_t, 9> boards;
	player_t next;
	move_t last_move;

	bool available(const move_t move) const
   	{ 
		return boards[move.board].available(move.space);
	}
			
	meta_board_t() : next(X), last_move(-1, -1)
	{
		for (auto &board : boards)
			board.clear();
	}

	meta_board_t(const meta_board_t &prior_meta_board, const move_t move) : last_move(move)
	{
		boards = prior_meta_board.boards;
		boards[move.board].spaces[move.space] = prior_meta_board.next;
		next = (prior_meta_board.next == X) ? O : X;
	}

	void render() const
	{
		// TODO(bjabes)
	}

	move_t get_move() const
	{
		// TODO(bjabes): get input from user
		return move_t(-1, -1);
	}
	
	meta_board_t apply_move(const move_t move) const
	{
		assert(available(move));
		return meta_board_t(*this, move);
	}

	bool game_over(player_t &winner, bool &tie) const
	{
		assert(0);

		// TODO: compute whether there is a winner
		return false;
	}
};


int main(int argc, char *argv[])
{
	// setup a board
	meta_board_t meta_board;

	do
	{
		// play and remember valid move from current player
		meta_board.render();
		move_t move = meta_board.get_move();
		meta_board.apply_move(move);
		player_t winner = 0;
		bool tie = false;
		if (meta_board.game_over(winner, tie))
		{
			// TODO: print status
			break;
		}
	}
	while (true);

	return EXIT_SUCCESS;
}

