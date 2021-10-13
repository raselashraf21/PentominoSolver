/*
 * pentomino.h
 *
 *  Created on: Jan 2, 2021
 *      Author: ashraf
 */

#ifndef PENTOMINO_H_
#define PENTOMINO_H_

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <map>
#include <set>

enum class PieceName
	:char {
		f = 'f',
		i = 'i',
		l = 'l',
		n = 'n',
		p = 'p',
		t = 't',
		u = 'u',
		v = 'v',
		w = 'w',
		x = 'x',
		y = 'y',
		z = 'z'
};

const PieceName Pieces[] = {
	PieceName::f,
	PieceName::i,
	PieceName::l,
	PieceName::n,
	PieceName::p,
	PieceName::t,
	PieceName::u,
	PieceName::v,
	PieceName::w,
	PieceName::x,
	PieceName::y,
	PieceName::z
};

enum State {
	invalid = '-',
	empty = '0',
	full = '1',
	pentomino = '*'
};

struct Orientation {
	Orientation()=default;
	Orientation(
			int x,
			int y,
			std::string orientation
			):startX(x),
			  startY(y),
			  positions(orientation){}

	~Orientation()=default;
	int startX;
	int startY;
	std::string positions;
};

static 	std::map<PieceName,Orientation> DefaultOrientations= {
		{
			PieceName::f,
			Orientation(3,3,"011110010")
		},
		{
			PieceName::i,
			Orientation(1,5,"11111")
		},
		{
			PieceName::l,
			Orientation(4,2,"10101011")
		},
		{
			PieceName::n,
			Orientation(4,2,"01011110")
		},
		{
			PieceName::p,
			Orientation(3,2,"111110")
		},
		{
			PieceName::t,
			Orientation(3,3,"111010010")
		},
		{
			PieceName::u,
			Orientation(2,3,"101111")
		},
		{
			PieceName::v,
			Orientation(3,3,"100100111")
		},
		{
			PieceName::w,
			Orientation(3,3,"100110011")
		},
		{
			PieceName::x,
			Orientation(3,3,"010111010")
		},
		{
			PieceName::y,
			Orientation(2,4,"00101111")
		},
		{
			PieceName::z,
			Orientation(3,3,"110010011")
		}
};


class Pentomino {
public:

	typedef std::array<std::pair<int,int>,5> Offset_Type;
	typedef std::set<Offset_Type> Orientation_Type;

	Pentomino()=delete;
	Pentomino(PieceName name);

	Orientation default_orientation() const {
		return DefaultOrientations[_name];
	}
	Orientation_Type& orientations() {
		return _orientations;
	}

	PieceName name(){
		return _name;
	}

	friend std::ostream& operator<<(std::ostream& out, Pentomino &pn);

private:
	Orientation_Type _orientations;
	PieceName _name;

	Offset_Type coordinates( Orientation orienations)const;
	Orientation rotate_clockwise(Orientation& basic)const;
	Orientation rotate_anticlockwise(Orientation& basic)const;
	Orientation FlipH(Orientation& basic)const;
	Orientation FlipV(Orientation& basic)const;
};




#endif /* PENTOMINO_H_ */
