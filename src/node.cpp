/*
 * node.cpp
 *
 *  Created on: Dec 26, 2020
 *      Author: ashraf
 */

#include "node.h"
#include <vector>
using namespace std;

bool Node::operator==(const Node& node) const{
	if(_left == node._left &&
		_right == node._right &&
		_up == node._up &&
		_down == node._down &&
		_head == node._head &&
		_id == node._id &&
		_row == node._row){
		return true;
	}else{
		return false;
	}
}

bool Node::operator!=(const Node& node) const{
	return !(*this==node);
}

Node& Node::operator++(){
	return *(this->_right);
}

Node* Node::operator++(int){
	Node* temp = this;
	++(*this);
	return temp;
}

Node& Node::operator--(){
	return *(this->_left);
}

Node* Node::operator--(int){
	Node* temp = this;
	--*this;
	return temp;
}

Node* Node::begin(){
	return _right;
}

Node* Node::end(){
	return this;
}

Node* Node::rbegin(){
	return _left;
}

Node* Node::rend(){
	return this;
}

Node* Node::vbegin(){
	return _down;
}

Node* Node::vend(){
	return this;
}

Node* Node::rvbegin(){
	return _up;
}

Node* Node::rvend(){
	return this;
}

void Node::remove(){
	_right->_left = _left;
	_left->_right = _right;
}

void Node::cover(){
	_up->_down = _down;
	_down->_up = _up;
}

void Node::uncover(){
	_up->_down = this;
	_down->_up = this;
}

void Node::restore(){
	_right->_left = this;
	_left->_right = this;
}

void Node::insert_right(Node* node){
	node->_left = this;
	_right->_left = node;
	node->_right = _right;
	_right = node;
}

void Node::insert_down(Node* node){
	node->_up = this;
	_down->_up = node;
	node->_down = _down;
	_down = node;
}

Node* Node::right()const{
	return _right;
}

Node* Node::down()const{
	return _down;
}

Node* Node::left()const{
	return _left;
}

Node* Node::up()const{
	return _up;
}

vector<Node*> Node::rows(){
	vector<Node*> rvec;
	for(Node* itr=begin(); itr!= end();
			itr=&itr->operator ++()){
		rvec.push_back(itr);
	}

return rvec;
}

vector<Node*> Node::reverse(){
	vector<Node*> rvec;
	for(Node::iterator itr = rbegin(); itr!=rend();
			itr=itr->left()){
		rvec.push_back(itr);
	}
return rvec;
}


