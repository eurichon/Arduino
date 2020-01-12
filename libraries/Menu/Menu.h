/* 
* Menu.h
*
* Created: 7/8/2019 10:31:56 μμ
* Author: evrichon
*/


#ifndef __MENU_H__
#define __MENU_H__

#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define NAME_SIZE 20
#define NONE 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define MIDDLE 5
#define PARENT 6

class Menu
{
	protected:
	char name[NAME_SIZE];
	int posX, posY,
	numberOfChildeen,
	numberOfNeightboors,
	id;
	int *neighbourDirection;

	Menu **neighbours,
	**childreen,
	*parent;

	void addParent(Menu *parent); //to do break cycle
	void addNeighbourBack(Menu* neighbour,int reverseDirectionCode);
	public:
	Menu(const char name[],int id);
	void setPosition(int posX,int posY);
	void addNeighbour(Menu* neighbour,int directionCode);
	void addChild(Menu *child);
	Menu* getNeighbour(int directionCode);
	Menu* getChild(int id);
	Menu* getParent();
	int getReverseDirCode(int directionCode);
	int getNumOfChildreen();
	int getNumOfNeighbours();
	int getId();
	char* getName();
	void info();
	~Menu();
};


#endif //__MENU_H__
