/*
 * pentomino.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: ashraf
 */

#include "pentomino.h"

/**
 * Pentomino Constructor
 * @desc: For every pentomino construction done with calculating of every possible
 * 		  orientation regardless of board configuration.    There can be duplicate
 * 		  orientations, underlying container declared as SET to remove duplicacy.
 *
 * @container:
 * 		_orientations: std::set<std::array<std::pair<int, int>,5>>
 *
 */
Pentomino::Pentomino(PieceName name):_name(name) {
	Orientation def = default_orientation();
	_orientations.insert(coordinates(def));

	Orientation clk = rotate_clockwise(def);
	for(int i = 0; i < 3; ++i){
		_orientations.insert(coordinates(clk));
		clk = rotate_clockwise(clk);
	}

	Orientation anti_clk = rotate_anticlockwise(def);
	for(int i = 0; i < 3; ++i){
		_orientations.insert(coordinates(anti_clk));
		anti_clk = rotate_clockwise(anti_clk);
	}

	Orientation flipH = FlipH(def);
	for(int i = 0; i < 3; ++i){
		_orientations.insert(coordinates(flipH));
		flipH = rotate_clockwise(flipH);
	}

	Orientation flipV = FlipV(def);
	for(int i = 0; i < 3; ++i){
		_orientations.insert(coordinates(flipV));
		flipV = rotate_clockwise(flipV);
	}
}

/**
 * @desc: Every orientation calculation based on string representations. Placement in
 *  	  board required co-ordiate representations. Here conversion is done from
 *  	  string to co-ordiate representations
 *
 * @return:
 * 		std::array<std::pair<int,int>,5>: array of co-ordinates.
 *
 *
 */
Pentomino::Offset_Type Pentomino::coordinates(
		Orientation orienations )const {

	Offset_Type offsets;
	int offset_count=0;
	for(int i = 0; i < orienations.startX; ++i){
		for(int j = 0; j < orienations.startY; ++j){
			if(
			orienations.positions[ ((i*orienations.startY) + j)] == '1'
			){
				offsets[offset_count].first = i;
				offsets[offset_count].second = j;
				++offset_count;
			}
		}
	}

	return offsets;
}

/**
 * @desc: clock-wise rotation
 *
 * @return:
 * 		Orientation: clock-wise string representation
 *
 */
Orientation Pentomino::rotate_clockwise(
		Orientation& basic
		)const{

	std::string representations;
	for(int i = basic.startY; i > 0; --i ){
		for(int j = basic.startX; j > 0; --j){
			representations.push_back(
					basic.positions[ (basic.startY*j) - i ]);
		}
	}

	return Orientation(
			basic.startY,
			basic.startX,
			representations
			);
}

Orientation Pentomino::rotate_anticlockwise(
		Orientation& basic
		)const {

	std::string representations;
	for(int i = 1; i <= basic.startY; ++i ){
		for(int j = 1; j <= basic.startX; ++j){
			representations.push_back(
					basic.positions[ (basic.startY*j) - i]);
		}
	}

	return Orientation(
			basic.startY,
			basic.startX,
			representations
			);
}

Orientation Pentomino::FlipH(
		Orientation& basic
		)const {

	std::string representations;
	for(int i = (basic.startX -1); i >=0; --i ){
		for(int j = 0; j < basic.startY; ++j){
			representations.push_back(
					basic.positions[ (basic.startY*i) + j]);
		}
	}

	return Orientation(
			basic.startX,
			basic.startY,
			representations
			);
}

Orientation Pentomino::FlipV(
		Orientation& basic
		)const {

	std::string representations;
	for(int i=1; i <= basic.startX; ++i ){
		for(int j =1; j <= basic.startY; ++j){
			representations.push_back(
					basic.positions[ (basic.startY*i) - j]);
		}
	}

	return Orientation(
			basic.startX,
			basic.startY,
			representations);
}

