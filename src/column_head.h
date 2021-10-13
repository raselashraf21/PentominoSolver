/*
 * column_head.h
 *
 *  Created on: Jan 2, 2021
 *      Author: ashraf
 */

#ifndef COLUMN_HEAD_H_
#define COLUMN_HEAD_H_
#include "node.h"

/**
 *
 * Sparse matrix:
 *
 *   		|column-1|     |column-2|    |column-3|    |column-4|
 *   		|  node  | --> |  node  |--> |  node  |--> |  node  |
 *   		------------------------------------------------------
 *   		    |			  |				|            |
 *			 (node)---------(node)          |
 *   		    |			  |				|            |
 *				|           (node) ------ (node)------ (node)
 *   		    |			  |				|            |
 *			 (node) -------	(node) ------ (node)------ (node)
 *   		    |			  |				|            |
 *
 *
 *  Every column itself include node as well as nodes, every node connected with
 *  left, right, bottom and up nodes. ColumnNode links also established by node.
 *
 */

class ColumnHead{
public:
	typedef ColumnHead* iterator;
	ColumnHead()=delete;
	ColumnHead(int id
			):_id(id),
			_counter(0),
			_node(new Node(this,-1,'*')){}
	ColumnHead(const ColumnHead &) = delete;
	ColumnHead& operator=(const ColumnHead &) = delete;
	~ColumnHead();

	bool operator<(ColumnHead& column) const ;

	int id()const{ return _id;}
	Node* node()const{ return _node;}
	ColumnHead* right(){
		return (_node->right())->head();
	}
	ColumnHead* left(){
			return (_node->left())->head();
		}

	int counter()const {return _counter;}
	void insert_right(ColumnHead*);
	void insert_below(int,char);
	std::vector<Node*> vertical();
	void cover();
	void uncover();
	void remove();
	void restore();
	iterator begin();
	iterator end();

	iterator vbegin();
	iterator vend();



private:
	int _id;
	int _counter;
	Node* _node;
};

ColumnHead* get_by_id(ColumnHead* root, int id);
#endif /* COLUMN_HEAD_H_ */
