/*
 * parser.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: ashraf
 */

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "placer.h"

using namespace std;

ostream& operator<<(ostream& out, Pentomino &pn) {
	return out<< static_cast<char>(pn._name);
}

void test_pentomino(Pentomino pentomino) {
	cout << pentomino <<" :" << endl;
	auto &orientations = pentomino.orientations();
	for(auto& orientation : orientations) {
		for(int i = 0; i < 5; ++i) {
			cout <<" ("<<orientation[i].first<<","<<orientation[i].second <<") ";
		}
		cout << endl;
	}
}

/*
 * @desc: Board can has some block cells or filled cells. Solution must exclude those
 * 		  cells from sparse matrix.
 *
 * @param:
 * 		X: int; row
 * 		Y: int; column
 * */
void Placer::set_invalid_cells(int X, int Y) {
	_invalid_cells.push_back(make_pair(X,Y));
}

bool Placer::is_invalid (int x, int y) {
	auto itr = find_if(
			_invalid_cells.begin(),
			_invalid_cells.end(),
			[x,y](const pair<int,int> &m) -> bool {
				return m.first == x && m.second==y; }
	);

	return itr != _invalid_cells.end();
}

Placer::Placer(
		Placer::Board_Type board,
		const PieceName (&pieces)[12]
		):_board(board) {

	for(auto piece: Pieces ){
		_pentominos.push_back(Pentomino(piece));
	}
}

/**
 * @desc: Remove pentomino piece from pentomino-list that already placed in the board
 *
 * Board: 3x4
 * Placed Pentomino: T
 * Pentomino-List: {L,V,W,X,Y,Z,F,U,N,T,P,I}
 *
 *  	-----------------
 *  	| T | T | T | P |
 *  	-----------------
 *  	| 0 | T | P | P |
 *  	-----------------
 *  	| 0 | T | P | P |
 *  	-----------------
 *
 * Result Pentomino-List: {L,V,W,X,Y,Z,F,U,N,I}
 *
 *
 */
void Placer::remove(PieceName& pn) {
	_pentominos.erase(
			remove_if(
					_pentominos.begin(),
					_pentominos.end(),
					[pn](Pentomino p) {

		return (p.name() == pn);
	}), _pentominos.end());

}

/**
 * @desc: read pentomino board, search for any pentomino tiles already placed,
 * 		  and set the invalid-cells if there any
 */
vector<PieceName> Placer::search_pieces(Board_Type& board) {
	vector<PieceName> pieces;
	for(size_t i = 0; i < board.size(); ++i){
		for(size_t j = 0; j < board[i].size(); ++j){
			switch(board[i][j]){
			case State::empty:
				break;
			case State::invalid:
				set_invalid_cells(i,j);
				break;
			case State::full:
			default:
				pieces.push_back(
						static_cast<PieceName>(tolower(board[i][j]))
						);
				break;
			}
		}
	}

	return pieces;
}

/**
 * @desc: recursively iterate over pentomino board, find the possible positions of
 * 		 a pentomino on the board.
 *
 * 		 Board: 6x10, F can be placed 192 different ways
 */
void Placer::find_positions(
		int posX,
		int posY,
		Pentomino::Offset_Type orientation,
		PieceName pn
		){

	vector<pair<int,int>> cells;
	typedef vector<pair<int,int>>::iterator Itr_Type;

	int boardX=_board.size();
	int boardY=(boardX >=1)?_board[0].size():0;

	for(int i = 0; i < 5; ++i) {
		if(
			is_invalid(
					posX+orientation[i].first,
					posY+orientation[i].second
					)
		){
			break;
		}

		if( posX+orientation[i].first >= boardX ) {
			return;
		}

		if( posY+orientation[i].second >= boardY ) {
			posX=posX+1;
			posY=-1;
			break;
		}

		cells.push_back(
				make_pair(posX+orientation[i].first,
						posY+orientation[i].second)
					);
	}

	if(cells.size() == 5) {
		int column_size = ( boardX * boardY );
		int x = distance(
				Pieces,
				find(Pieces, Pieces + 12, pn)
				);
		vector<char> column( column_size + 12, '0');

		for(Itr_Type itr = cells.begin();
				itr != cells.end();++itr){

			column[ (itr->first* boardY + itr->second) ] = static_cast<char>(pn);
			column[ (column_size+x) ] = static_cast<char>(pn);
		}
		_rows.push_back(column);
	}

	find_positions(posX, posY+1, orientation, pn);

}

/**
 * @desc: If there is a blocked cells on the board, then find_positions() exclude this
 * 		  cells, therefore rows representation include those cells as column with empty row values
 *
 *	Board: 3x4
 *  	-----------------
 *  	| 0 | 0 | 0 | 0 |
 *  	-----------------
 *  	| 0 | X | X | 0 |			(X: Blocked Cells)
 *  	-----------------
 *  	| 0 | 0 | 0 | 0 |
 *  	-----------------
 *
 * Pentomino: T
 *
 *
 * Columns:		1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12
 * 				------------------------------------------------
 * rows[n]:     x | x | x | x | x | 0 | 0 | x | x | x | x  |  x  		{x:0/1;}
 *
 * Dancing link will find that column 6 & 7 is the minimum column with values 0,
 * it will find that no pentominos covered by those cells, which in turn find that
 * no solution is possible.
 *
 *In exact cover problem, every column must have at least a value, otherwise it can
 *be deliberately argued that no solution is possible since it breaks the constraints
 *
 *
 */
void Placer::remove_empty_columns(){
	for(size_t i = 0; i<_invalid_cells.size();++i){
		int column_size = (
				( _invalid_cells[i].first * _board[0].size() ) +
				_invalid_cells[i].second
				);
		for_each(
				_rows.begin(),
				_rows.end(),
				[&](vector<char>& row) {
						row.erase(
								next(row.begin(),
								column_size-i)
								);
				});
	}

}

/**
 * @desc: Dynamically fill-up the board with every orientation of pentomino and collect
 * 		  the rows
 */
void Placer::fill_board(){

	for(vector<Pentomino>::iterator itr = _pentominos.begin();
			itr != _pentominos.end();++itr){
		auto &orientations = itr->orientations();
		for(auto orientation: orientations){
			find_positions(0,0, orientation, itr->name());
		}
	}

	remove_empty_columns();

}
