/*
 * parser.h
 *
 *  Created on: Dec 29, 2020
 *      Author: ashraf
 */

#ifndef PLACER_H_
#define PLACER_H_

#include "pentomino.h"

class Placer {
public:
	typedef std::vector<std::vector<char>> Board_Type;
	Placer(
			Board_Type board,
			const PieceName (&pieces)[12]);
	void read_from_file();
	Board_Type read_board(std::string&, int, int);
	void remove(PieceName&);
	std::vector<PieceName> search_pieces(Board_Type&);


	void find_positions(int, int, std::array<std::pair<int,int>,5> , PieceName);
	void fill_board();

	std::vector<Pentomino>& pentominos(){
		return _pentominos;
	}
	Board_Type& rows(){
		return _rows;
	}
	std::vector<std::pair<int, int>>& invalid_cells(){
		return _invalid_cells;
	}
private:
	Board_Type _board;
	std::vector<Pentomino> _pentominos;
	/**
	 *  It is collection of rows, each row represents possible placement of a pentomino
	 *  on the board.
	 *
	 * Board: 3x4
	 *  	-----------------
	 *  	| 1 | 2 | 3 | 4 |
	 *  	-----------------
	 *  	| 5 | 6 | 7 | 8 |
	 *  	-----------------
	 *  	| 9 |10 |11 |12 |
	 *  	-----------------
	 *
	 * Pentomino: T
	 *
	 *
	 * Columns:		1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12
	 * 				------------------------------------------------
	 * rows[0]:		1 | 1 | 1 | 0 | 0 | 1 | 0 | 0 | 0 | 1  | 0  |  0
	 *   .			.................................................
	 *   .			.................................................
	 *   .			.................................................
	 * rows[n]:     x | x | x | x | x | x | x | x | x | x | x  |  x  		{x:0/1;}
	 *
	 *
	 */
	Board_Type _rows;

	/**
	 *
	 * Let's consider, Board: 3x4
	 *
	 *     	-----------------
	 *  	| 0 | 0 | 0 | 0 |
	 *  	-----------------
	 *  	| 0 | X | X | 0 |			(X: blocked cells or Filled by pentominos already;
	 *  	-----------------			 Use-Case:
	 *  	| 0 | 0 | 0 | 0 |						Exclude those cells from further calculation
	 *  	-----------------						and reuse that for creating solution.
	 */
	std::vector<std::pair<int, int>> _invalid_cells;

	bool is_invalid (int,int);
	void set_invalid_cells(int, int);
	void remove_empty_columns();

};

#endif /* PLACER_H_ */
