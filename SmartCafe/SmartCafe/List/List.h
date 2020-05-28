#ifndef _LIST_h
#define _LIST_h

#include "Arduino.h"


typedef struct Node{
	int id;
	char data[15];
	uint16_t price;
	Node *prev;
	Node *next;
}Node;


class List{
	private:
		int size;
		Node *curr;
	public:	
		List();
		void pushAfter(char *str, int id, uint16_t price);
		void pop();
		void next();
		void prev();
		Node* getCurr();
		Node* getPrev();
		Node* getNext();
		void clear();
		int length();
		~List();
};



#endif

