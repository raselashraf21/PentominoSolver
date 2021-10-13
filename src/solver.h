/*
 * dancing_link.h
 *
 *  Created on: Dec 26, 2020
 *      Author: ashraf
 */

#ifndef SOLVER_H_
#define SOLVER_H_



#include "node.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <map>
#include <limits>
#include <filesystem>

const size_t SOLUTION_SIZE = std::numeric_limits<size_t>::max();

class ExactCover{
public:
	typedef std::map<char,std::vector<int>>  solution_type;
	ExactCover()=delete;
	explicit ExactCover(std::vector<std::vector<char>>&, int, int);
	ExactCover(const ExactCover&)=delete;
	ExactCover& operator=(const ExactCover&)=delete;
	~ExactCover();
	void search(int, size_t soln_size= SOLUTION_SIZE);
	void print_solution(
			std::vector<std::vector<char>>,
			int,
			int,
			std::vector<std::pair<int, int>>&
			);

	void write_solution(
			std::filesystem::path,
			std::vector<std::vector<char>>,
			int,
			int,
			std::vector<std::pair<int, int>>&
			);


	void print_matrix_by_column();
	void print_matrix_by_row();

	size_t solution_size(){
		return _all_solutions.size();
	}

private:
	ColumnHead* _root;
	int _boardX;
	int _boardY;
	solution_type _solution;
	std::map<int, solution_type> _all_solutions;

	ColumnHead* min_element()const;
	Node* getnode_by_xy(int, int);
	void push_solution(Node*);
	void pop_solution(Node*);
	void get_solution();

};


#endif /* SOLVER_H_ */
