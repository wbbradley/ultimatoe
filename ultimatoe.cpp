#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <tr1/array>
#include <string.h>

#define X 1
#define O 2

typedef int piece_t;
typedef int player_t;
struct board_t;
struct meta_board_t;

struct move_t
{
	move_t(int b, int s) : board(b), space(s) {}
	int board;
	int space;
	bool exists() const { return board >= 0 and space >= 0; }
};

player_t get_space_owner(const board_t &board, int space);
player_t get_space_owner(const meta_board_t &meta_board, int board);
bool all_spaces_owned(const board_t &board);
bool all_spaces_owned(const meta_board_t &board);

template <typename B>
player_t game_over(const B &board, player_t &winner, bool &tie)
{
	struct 
	{
		int a, b, c;
	} combos[] = {
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8},
		{0, 3, 6},
		{1, 4, 7},
		{2, 5, 8},
		{0, 4, 8},
		{2, 4, 6},
	};
	for (auto &combo : combos)
	{
		auto winner_a = get_space_owner(board, combo.a);
		if (!winner_a)
			continue;
		auto winner_b = get_space_owner(board, combo.b);
		if (!winner_b || (winner_a != winner_b))
			continue;
		auto winner_c = get_space_owner(board, combo.c);
		if (!winner_c || (winner_b != winner_c))
			continue;
		winner = winner_c;
		tie = false;
		return true;
	}

	if (all_spaces_owned(board))
	{
		winner = 0;
		tie = true;
		return true;
	}

	return false;
}

struct board_t
{
	board_t()
   	{
		memset(&spaces[0], 0, sizeof(piece_t) * 9);
	}
	std::tr1::array<piece_t, 9> spaces;
	
	player_t winner() const
	{
		player_t winner = 0;
		bool tie = false;
		game_over(*this, winner, tie);
		return winner;
	}

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
	std::tr1::array<board_t, 9> boards;
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

	static void render_space(piece_t space)
	{
		switch (space) {
			case 0: std::cout << "-"; break;
			case X: std::cout << "x"; break;
			case O: std::cout << "o"; break;
			default: assert(false); break;
		}
	}

	void render_metaboard_row_row(int meta_row, int board_row) const
	{
		const char *sep = "";
		for (int board = 3 * meta_row; board < 3 * meta_row + 3; ++board)
		{
			std::cout << sep;
			sep = " | ";
			for (int space = 3 * board_row; space < 3 * board_row + 3; ++space)
			{
				render_space(boards[board].spaces[space]);
			}
		}
		std::cout << std::endl;	
	}

	void render_metaboard_row(int i) const
	{
		render_metaboard_row_row(i, 0);
		render_metaboard_row_row(i, 1);
		render_metaboard_row_row(i, 2);
	}

	void render() const
	{
		//  
		// oxo | xox | xxx 
		// o-o | xox | x-x 
		// ox- | -ox | xxx 
		// =============== 
		// oxo | xox | xxx 
		// o-o | xox | x-x 
		// ox- | -ox | xxx 
		// =============== 
		// oxo | xox | xxx 
		// o-o | xox | x-x 
		// ox- | -ox | xxx 
		//  

		render_metaboard_row(0);
		std::cout << "===============" << std::endl;
		render_metaboard_row(1);
		std::cout << "===============" << std::endl;
		render_metaboard_row(2);
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
		return ::game_over(*this, winner, tie);
	}
};

player_t get_space_owner(const board_t &board, int space)
{
	return board.spaces[space];
}

player_t get_space_owner(const meta_board_t &meta_board, int board)
{
	return meta_board.boards[board].winner();
}

bool all_spaces_owned(const board_t &board)
{
	for (const auto space : board.spaces)
	{
		if (!space)
			return false;
	}
	return true;
}

bool all_spaces_owned(const meta_board_t &meta_board)
{
	for (const auto &board : meta_board.boards)
	{
		if (!board.winner())
			return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	// setup a board
	meta_board_t meta_board;

	if (argc == 2 && strcmp(argv[1], "-t") == 0)
	{
		std::cout << "Running ultimatoe tests..." << std::endl;

		// test simple game over cases
		meta_board_t meta_board;
		meta_board.boards[0].spaces[0] = X;
		meta_board.boards[0].spaces[1] = X;
		meta_board.boards[0].spaces[2] = X;
		player_t winner = false;
		bool tie = false;
		assert(meta_board.boards[0].winner() == X);
		assert(!meta_board.game_over(winner, tie));
		assert(!tie);
		return EXIT_SUCCESS;
	}

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

