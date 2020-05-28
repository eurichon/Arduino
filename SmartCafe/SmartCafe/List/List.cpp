#include "List.h"

List::List(){
	size = 0;
	curr = NULL;
}


void List::pushAfter(char *str, int id, uint16_t price){
	Node *new_node = new Node();
	
	if(new_node == NULL){
		return;
	}
	
	strncpy(new_node->data, str, 15);
	new_node->id = id;
	new_node->price = price;
	
	if(size == 0){
		curr = new_node;
		new_node->next = new_node;
		new_node->prev = new_node;
	}else{
		Node *after = curr->next;
		
		// link curr with new_node
		curr->next = new_node;
		new_node->prev = curr;
		
		// link ne_node with after
		new_node->next = after;
		after->prev = new_node;		
	}
	
	size = size + 1;
}


void List::pop(){
	if(size == 0){
		curr = NULL;
		size = 0;
	}else if(size == 1){
		delete curr;
		curr = NULL;
		size = 0;
	}else{
		Node *prev_node = curr->prev;
		Node *next_node = curr->next;
		
		// delete the current node
		delete curr;
		
		// link list after extracting the elem
		prev_node->next = next_node;
		next_node->prev = prev_node;
		
		
		curr = next_node;
		
		// update size		
		size = size - 1;
	}
}


void List::next(){
	if(curr == NULL){
		return;
	}else{
		curr = curr->next;
	}
}


void List::prev(){
	if(curr == NULL){
		return;
	}else{
		curr = curr->prev;
	}
}


Node *List::getCurr(){
	return curr;
}


Node *List::getNext(){
	if(curr == NULL){
		return NULL;
	}else{
		return curr->next;
	}
}


Node *List::getPrev(){
	if(curr == NULL){
		return NULL;
	}else{
		return curr->prev;
	}
}


void List::clear(){
	while(size > 0){
		this->pop();
	}	
	
	curr = NULL;
	size = 0;
}


int List::length(){
	return size;
}

List::~List(){
	clear();
}