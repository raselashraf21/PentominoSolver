/*
 * main.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: ashraf
 */
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <cstdlib>
#include <filesystem>
	
#include "solver.h"
#include "placer.h"
#include "board.h"

using namespace std;

/**
 * Command Line Argument:
 * 	-b "-----------0-\n-----------00\n----00--0000-\n00000000000--\n00000000000--\n0000000000---\n00000000-----\n0000000------\n00---00------"
 * 	-b "-00--0000-\n00--000000\n00--000000\n000--00000\n00000--000\n000000--00\n000000--00\n-0000--00-"
 * 	-b "0000000000\n0000000000\n0000000000\n0000000000\n0000000000\n0000000000"
 */

static void print_usage(){
	std::string console_out =""
			"Usage:\n"
			"./solver\n "
			"-b board\n "
			" Each row separated by newline character '\\n' \n"
			" 	Empty cell denoted by '0'\n "
			"		Ex. 6x10 board\n"
			"			0000000000\n"
			"			0000000000\n"
			"			0000000000\n"
			"			0000000000\n"
			"			0000000000\n"
			"			0000000000\n"
			"	Blocked cell denoted by '-'\n "
			"		Ex. 8x8 board\n"
			"			00000000\n"
			"			00000000\n"
			"			00000000\n"
			"			000--000\n"
			"			000--000\n"
			"			00000000\n"
			"			00000000\n"
			"			00000000\n"
			" 	FilledUP by Pentomino pieces denoted by pentomino character e.g. 'T'\n"
			"		Ex. 3x4 board\n"
			"			0TTT\n"
			"			00T0\n"
			"			00T0\n"
			"\n"
			"-n solution number\n"
			"-f input file name\n"
			"-o output directory. Default is ../data/\n"
			"-v print solution in console\n\n";

	std::cout << console_out;
}

bool new_line_found(char start, char end) {
	return (start == '\\' && end == 'n');
}

bool new_line_nfound(char c) {
	return !(c == '\n');
}


int main(int argc, char **argv){

	size_t solution_numbers = 0;
	std::string file_name;
	std::filesystem::path outDir;
	std::string input_board;

	int c=0;
	while( (c=getopt(argc, argv,"b:f:n:vh:o:")) != -1 ){
		switch(c) {
			case 'n':
				char* end;
				solution_numbers = std::strtoul(optarg, &end, 10);
		        break;
		    case 'b':
			    input_board.assign(optarg);
		    	break;
			case 'o':
				outDir.assign(optarg);
				break;
		    case 'h':
		    case 'v':
		    case '?':
		    	print_usage();
		        exit(0);
		}
	}

	if(outDir.empty()){
		outDir.assign("./data/"); // Set default directory
	}
	
	if(outDir.root_directory() == ""){
		std::string path = std::filesystem::current_path().string()+"/"+outDir.relative_path().string();
		outDir.assign(path);
	}

	try{
		std::filesystem::create_directories(outDir);
	}catch(exception&e){
		cout << e.what() << endl;
		cout << "Falling back to default path: ../data/" << endl;
		outDir.assign("../data/");
		std::filesystem::create_directories(outDir);
	}
	if(input_board.empty()){
		std::copy(board8x9.begin(), board8x9.end(),
				std::back_inserter(input_board) );
	}

	vector<vector<char>> board;
	typedef string::iterator iter;
	vector<string> ret;
	iter prev = input_board.begin();
	while (prev != input_board.end()) {
		iter next = adjacent_find(prev, input_board.end(), new_line_found);
		vector<char> row(prev,next);
		board.push_back(row);
		if( next == input_board.end()){
			break;
		}
		next=next+2;
		prev=next;
	}

	const int boardY = max_element(board.begin(), board.end(),
				[](const auto& a, const auto& b) {
					return a.size() < b.size();
				})->size();

	const int boardX = board.size();

	Placer placer(board, Pieces);
	vector<PieceName> placed_pieces = placer.search_pieces(board);
	for(auto& piece: placed_pieces ){
		placer.remove(piece);
	}

	placer.fill_board();
	auto& rows = placer.rows();


#if DEBUG
	for(vector<vector<char>>::iterator itr = rows.begin();
			itr!= rows.end();++itr){
		for(vector<char>::iterator iitr= itr->begin();
			iitr!= itr->end(); ++iitr){
			std::cout << *iitr <<" ";
		}
		std::cout << std::endl;
	}
	std::cout << "Rows: "<<rows.size()<<std::endl;
#endif

	ExactCover ecover(rows,boardX,boardY);

	clock_t start;
	double duration;
	start = std::clock();
#if DEBUG
    std::cout << "Looking for Solution: ........\n";
#endif
    ecover.search(0,solution_numbers);
	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

#if DEBUG
	ecover.print_solution(
			board,
			boardX,
			boardY,
			placer.invalid_cells()
			);
	std::cout << "\nTotal solution: "<< ecover.solution_size()<<endl;
	std::cout << "Total Time: " << duration <<" seconds"<<endl;
#endif
	ecover.write_solution(
				outDir,
				board,
				boardX,
				boardY,
				placer.invalid_cells()
				);



}
