/*
 * node.h
 *
 *  Created on: Dec 26, 2020
 *      Author: ashraf
 */

#ifndef NODE_H_
#define NODE_H_

#include <cstdlib>
#include <iostream>
#include <vector>

class ColumnHead;

class Node {
public:
	using value_type = Node;
	using pointer = value_type*;
	using reference = value_type&;
	using difference_type = ptrdiff_t;
	using iterator_category = std::bidirectional_iterator_tag;

	typedef Node* iterator;

	Node()=delete;
	Node(ColumnHead* head, int row, char id):
			_id(id),
			_row(row),
			_left(this),
			_right(this),
			_up(this),
			_down(this),
			_head(head){}

	Node(Node* cnode)=delete;
	Node& operator=(const Node&)=delete;
	~Node() = default;

	bool operator==(const Node& node) const;
	bool operator!=(const Node& node) const;

	Node* operator->();
	Node& operator++();
	Node* operator++(int index);
	Node& operator--();
	Node* operator--(int);


	iterator begin();
	iterator end();
	iterator vbegin();
	iterator vend();

	iterator rbegin();
	iterator rend();
	iterator rvbegin();
	iterator rvend();


	void remove();
	void cover();
	void uncover();
	void restore();

	void insert_right(Node*);
	void insert_down(Node*);

	Node* right()const;
	Node* down()const;
	Node* up()const;
	Node* left()const;
	char id(){ return _id; }
	int row()const{return _row;}
	std::vector<Node*> rows();
	std::vector<Node*> reverse();
	ColumnHead* head()const{ return _head;}

private:
	char _id;
	int _row;
	Node* _left;
	Node* _right;
	Node* _up;
	Node* _down;
	ColumnHead* _head;
};


#endif /* NODE_H_ */
