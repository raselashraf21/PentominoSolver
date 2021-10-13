/*
 * column_head.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: ashraf
 */

#include "column_head.h"


ColumnHead::~ColumnHead(){
	Node* check_node=_node->down();
	Node* temp;
	while(check_node != _node){
		temp = check_node->down();
		delete check_node;
		check_node = temp;
	}
	delete _node;
}

void ColumnHead::insert_right(ColumnHead* column){
	_node->insert_right(column->_node);
}

void ColumnHead::insert_below(int row, char id){
	Node* node = new Node(this, row, id);
	_node->insert_down(node);
	++_counter;
}

/**
 * @desc: For each selected column, traverse the rows, and remove the rows
 *
 */
void ColumnHead::cover(){
#if DEBUG
	std::cout << "Covering Column: " <<id()<<std::endl;
#endif
	Node* temp;
	_node->remove();
	for(Node* nodes: vertical()){
		temp = nodes->right();
		while(temp != nodes){
			(temp->head())->_counter--;
#if DEBUG
		std::cout <<"\t Covering Node: " << temp->id() <<" Column: "
				<<temp->head()->id()<<" Row: "<<temp->row()<<std::endl;
#endif
			temp->cover();
			temp=temp->right();
		}
	}
}

/*
 * @desc: for each column, restore the soft deleted rows
 */

void ColumnHead::uncover(){
#if DEBUG
	std::cout << "Uncovering Column: " <<id()<<std::endl;
#endif
	Node *n = _node->up();
	while(n != _node){
		Node* temp =n->left();
		while(temp != n){
			temp->uncover();
			(temp->head())->_counter++;
#if DEBUG
		std::cout <<"\t Uncovering Node: " << temp->id() <<" Column: "
				<<temp->head()->id()<<" Row: "<<temp->row()<<std::endl;
#endif
			temp=temp->left();
		}
	n = n->up();
	}
	_node->restore();
}

std::vector<Node*> ColumnHead::vertical(){
	std::vector<Node*> node_vec;
	Node* node= this->node();
	for(Node* itr=node->vbegin(); itr != node->vend();
			itr=itr->down()){
		node_vec.push_back(itr);
	}

	return node_vec;
}

ColumnHead::iterator ColumnHead::begin(){
	return node()->begin()->head();
}

ColumnHead::iterator ColumnHead::end(){
	return node()->end()->head();
}

ColumnHead::iterator ColumnHead::vbegin(){
	return node()->vbegin()->head();
}

ColumnHead::iterator ColumnHead::vend(){
	return node()->vend()->head();
}

ColumnHead* get_by_id(ColumnHead* root, int id){
	Node* temp = root->node()->right();
	while( temp->head()->id() != id ){
		temp = temp->right();
	}

	return temp->head();
}
