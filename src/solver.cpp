/*
 * dancing_link.cpp
 *
 *  Created on: Dec 26, 2020
 *      Author: ashraf
 */

#include "solver.h"
#include "node.h"
#include "column_head.h"
#include "pentomino.h"

#include <cassert>
#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>
#include <csignal>
#include <fstream>
#include <cstdio>
#include <filesystem>
using namespace std;

ExactCover::~ExactCover(){
	ColumnHead* next = _root->right();
	while(next != _root){
		ColumnHead* next_temp = next->right();
		delete next;
		next=next_temp;
	}
}

void ExactCover::print_matrix_by_row(){

	cout<< "Adjaceny Matrix  by ROW:\n";
	Node* d = _root->right()->node();
	Node*n = d->down();
	cout << n->id() <<" head: "
			<< n->head()->id() <<endl;
	do{
		Node* temp = n;
		do{
			cout << temp->id() <<" ";
			temp = temp->right();
		}while(temp != n);
		n = n->down();
		cout << endl;
	}while(n != d);
}

void ExactCover::print_matrix_by_column(){

	cout<< "Adjaceny Matrix by COLUMN:\n";
	for(ColumnHead* itr =_root->begin(); itr != _root->end();
			itr = itr->node()->right()->head()){
		cout << "Column: "<< itr->id() <<
				" Counter: "<< itr->counter() <<endl;
		for(Node* nitr = itr->node()->vbegin();
				nitr != itr->node()->vend();
				nitr = nitr->down()){
			cout <<"Id: " <<nitr->id()
					<<" Row: " <<nitr->row() <<endl;
		}
	}

	cout << endl;
}

/**
 * @Desc: In sparse matrix representation, each nodes are connected by with adjacent
 * 		  node in all direction [up,down, right, left]. Every node has unique Id.
 * 		  collect node by the position in terms of co-ordinates (x,y)
 *
 * @param
 * 		x: int; row
 * 		y: int; column
 *
 * @return:
 * 		Node*: node selected by row and column
 *
 */
Node* ExactCover::getnode_by_xy(int x, int y){
	ColumnHead* column = get_by_id(_root, y);
	Node* ret = column->node()->down();
	while( x != ret->row()){
		ret = ret->down();
	}
	return ret;
}

/**
 * @Desc: Every nodes directly or indirectly connected to column node, counter value
 * 		  indicate how many nodes under a column node. Minimum values of counter represents
 * 		  minimum number of child's or nodes under a column
 *
 *@return:
 *	ColumnHead*:
 *		column with minimum number of child's or nodes
 */
ColumnHead* ExactCover::min_element()const{
	int max=INT_MAX;
	ColumnHead* temp = _root->right();
	ColumnHead* min;
	while( temp != _root){
		if(temp->counter() < max){
			min = temp;
			max=min->counter();
		}
		temp = temp->right();
	}

	return min;
}

void ExactCover::push_solution(Node* n){
	Node* temp = n;
	vector<int> column;
	do{
		if(temp->head()->id() >= (_boardX*_boardY)){
			temp=temp->right();
			continue;
		}
#if DEBUG
		cout << " "<<temp->id() <<" ("<<temp->head()->id()<<")";
#endif
		column.push_back(temp->head()->id());
		temp=temp->right();
	}while(temp != n);

	_solution.emplace(temp->id(),column);
}

void ExactCover::pop_solution(Node *n){
	_solution.erase(_solution.find(n->id()));
}

void ExactCover::get_solution(){
	_all_solutions.emplace(_all_solutions.size(), _solution);
	//solution.clear();
}

void ExactCover::print_solution(
		vector<vector<char>> board,
		int bX,
		int bY,
		vector<pair<int, int>>&invalid_cells
		){

	typedef map<int,map<char,vector<int>>>::iterator Soln_Itr;
	int cx, cy;
	if(_all_solutions.size() < 1){
		cout << "No Solution Found :(" << endl;
		return;
	}

	for(Soln_Itr itr= _all_solutions.begin();
			itr != _all_solutions.end(); ++itr){

		vector<vector<char>> print_board(board);
		cout << "Solution Number: " << itr->first << endl;

		for( map<char,vector<int>>::iterator sitr = itr->second.begin();
				sitr!= itr->second.end(); ++sitr){

				for(vector<int>::iterator vitr = sitr->second.begin();
						vitr != sitr->second.end(); ++vitr){

					int index =*vitr;
					if(index >= bX*bY){
						continue;
					}

					for(size_t i=0; i < invalid_cells.size(); ++i){
						if(index >= (
								(invalid_cells[i].first *bY+ invalid_cells[i].second))
								){
							++index;
						}
					}
					cx= (index)/bY;
					cy = (index)%bY;
					if(cx >= bX){
						continue;
					}

					print_board[cx][cy]=sitr->first;
				}
			}

		for(int i = 0; i < bX; ++i){
				for(int j = 0; j < bY; ++j){
					cout<< " " << print_board[i][j]<<" | ";
				}
				cout <<endl;
		}
	}
}

void ExactCover::write_solution(
		std::filesystem::path outDir,
		vector<vector<char>> board,
		int bX,
		int bY,
		vector<pair<int, int>>&invalid_cells
		){

	std::string board_name = outDir.has_filename() ? outDir.string() : outDir.string()+"board-"+std::to_string(bX)+"x"+std::to_string(bY);
	ofstream fboard;
	fboard.exceptions(std::ifstream::failbit);


	try{
		remove(board_name.c_str());
		fboard.open (board_name);
	}catch(exception&e ){
		// std::cout << "Error in file deletion";
		std::cout << e.what() << endl;
		fboard.close();
		return;
	}
	
	typedef map<int,map<char,vector<int>>>::iterator Soln_Itr;
	int cx, cy;
	if(_all_solutions.size() < 1){
		cout << "No Solution Found :(" << endl;
		fboard.close();
		return;
	}
	static size_t line_counter=0;
	for(Soln_Itr itr= _all_solutions.begin();
			itr != _all_solutions.end(); ++itr){

		vector<vector<char>> print_board(board);
		for( map<char,vector<int>>::iterator sitr = itr->second.begin();
				sitr!= itr->second.end(); ++sitr){

				for(vector<int>::iterator vitr = sitr->second.begin();
						vitr != sitr->second.end(); ++vitr){

					int index =*vitr;
					if(index >= bX*bY){
						continue;
					}

					for(size_t i=0; i < invalid_cells.size(); ++i){
						if(index >= (
								(invalid_cells[i].first *bY+ invalid_cells[i].second))
								){
							++index;
						}
					}
					cx= (index)/bY;
					cy = (index)%bY;
					if(cx >= bX){
						continue;
					}

					print_board[cx][cy]=sitr->first;
				}
			}

		++line_counter;
		fboard << std::to_string(line_counter) << ", ";
		for(int i = 0; i < bX; ++i){
			std::string row;
			for(int j = 0; j < bY; ++j){
				print_board[i][j] = print_board[i][j]== '-' ?'.':print_board[i][j];
				row.push_back(print_board[i][j]);
				}
				row.push_back(' ');
				fboard << row;
		}
		fboard << "\n";
	}
	fboard.close();
}

ExactCover::ExactCover(
		vector<vector<char>>& matrix,
		int boardX,
		int boardY
		):_root(new ColumnHead(-1)),
		_boardX(boardX),
		_boardY(boardY){

	const int width = (matrix.size() >= 1 )? matrix[0].size():0;

#if DEBUG
	cout << "width: " << width << " value: " << matrix.size() << endl;
#endif
	for(int i = 0; i < width;++i){
		ColumnHead* temp = new ColumnHead(i);
		_root->insert_right(temp);
	}

	for(size_t i = 0; i< matrix.size(); ++i){
		for(int j=0; j < width; ++j){
			if(
				(toupper(matrix[i][j]) >='A' /* Not perfect, may need to change*/
				 && toupper(matrix[i][j])<='Z'
				) || toupper(matrix[i][j])=='1'
				) {
				get_by_id(_root,j)->insert_below(i, toupper(matrix[i][j]));
			}
		}
	}

	for(size_t i = 0; i < matrix.size(); ++i){
		Node* prev_node=nullptr;
		for(int j = 0; j < width; ++j){
			if(
				(toupper(matrix[i][j]) >='A'
				&& toupper(matrix[i][j])<='Z'
				) || toupper(matrix[i][j])=='1'
			 ){
				Node* node = getnode_by_xy(i, j);
				if(prev_node != nullptr){
					prev_node->insert_right(node);
				}
				prev_node = node;
			}
		}
	}
}

/**
 * D. Knuth Paper: https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/0011047.pdf
 */

void ExactCover::search(int depth, size_t soln_size){

	if(_root->right() == _root ){
		get_solution();
		if(soln_size!=0 &&
			_all_solutions.size() >= soln_size){
			;//exit(SIGTERM);
		}
		return;
	}else{
#if DEBUG
		cout << "Depth: " <<depth<<endl;
		print_matrix_by_row();
#endif

		ColumnHead *c = min_element();
		c->cover();
		for(Node* n : c->vertical()) {
			push_solution(n);
			for(Node* p : n->rows()) {
				p->head()->cover();
			}
			search(depth + 1,soln_size);
			pop_solution(n);
			c=n->head();
			for(Node*p : n->reverse()) {
				p->head()->uncover();
			}
		}
		c->uncover();
	}
}
