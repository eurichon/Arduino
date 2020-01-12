/* 
* Menu.cpp
*
* Created: 7/8/2019 10:31:55 μμ
* Author: evrichon
*/

#include "menu.h"

Menu::Menu(const char menu_name[],int id)
{
	this->id = id;
	strcpy (name,menu_name);
	numberOfChildeen = 0;
	numberOfNeightboors = 0;
	posX = 0;
	posY = 0;
	childreen = NULL;
	neighbours = NULL;
	parent = NULL;
	neighbourDirection = NULL;
}

Menu::~Menu()
{
	delete[] neighbourDirection;
	delete[] childreen;
	delete[] neighbours;
	delete parent;
}

void Menu::setPosition(int pos_x, int pos_y)
{
	this->posX = pos_x;
	this->posY = pos_y;
}

void Menu::info()
{
	printf("The menu is: \"%s\" with an id: %i\n", name, id);
	printf("Message position is: (%i,%i)\n", posX, posY);
	printf("Number of childreen is: %i\n",numberOfChildeen);
	printf("Number of neighbours is: %i\n",numberOfNeightboors);
	printf("=========================\n\n");
}

void Menu::addNeighbour(Menu *neighbour,int directionCode)
{
	numberOfNeightboors++;
	Menu** newNeighbours = new Menu*[numberOfNeightboors];
	if(newNeighbours == NULL){
		printf("Error in memory allocation");
		exit(1);
	}
	memcpy(newNeighbours, neighbours, (numberOfNeightboors-1) * sizeof(Menu*));
	newNeighbours[numberOfNeightboors-1] = neighbour;
	delete[] neighbours;
	neighbours = newNeighbours;

	neighbour->addParent(this->getParent());
	this->getParent()->addChild(neighbour);

	int* newNeighbourDirection = new int[numberOfNeightboors];
	memcpy(newNeighbourDirection, neighbourDirection, (numberOfNeightboors-1) * sizeof(int));
	newNeighbourDirection[numberOfNeightboors-1] = directionCode;
	delete[] neighbourDirection;
	neighbourDirection = newNeighbourDirection;

	neighbour->addNeighbourBack(this,getReverseDirCode(directionCode));
}

void Menu::addNeighbourBack(Menu *neighbour, int reverseDirectionCode)
{
	numberOfNeightboors++;
	Menu** newNeighbours = new Menu*[numberOfNeightboors];
	if(newNeighbours == NULL){
		printf("Error in memory allocation");
		exit(2);
	}
	memcpy(newNeighbours, neighbours, (numberOfNeightboors-1) * sizeof(Menu*));
	newNeighbours[numberOfNeightboors-1] = neighbour;
	delete[] neighbours;
	neighbours = newNeighbours;

	int* newNeighbourDirection = new int[numberOfNeightboors];
	memcpy(newNeighbourDirection, neighbourDirection, (numberOfNeightboors-1) * sizeof(int));
	newNeighbourDirection[numberOfNeightboors-1] = reverseDirectionCode;
	delete[] neighbourDirection;
	neighbourDirection = newNeighbourDirection;
}

Menu* Menu::getNeighbour(int directionCode)
{
	for(int i=0; i<numberOfNeightboors; ++i){
		if(directionCode == neighbourDirection[i]){
			return neighbours[i];
		}
	}
	printf("The neighbour with an direction code \"%i\" did not found",directionCode);
	return NULL;
}

void Menu::addChild(Menu *child)
{
	for(int i = 0;i<numberOfChildeen;i++){
		if(childreen[i] == child){
			printf("The child already exists!");
			return;
		}
	}
	numberOfChildeen++;
	Menu** newChildreen = new Menu*[numberOfChildeen];
	if(newChildreen == NULL){
		printf("Error in memory allocation");
		exit(3);
	}
	memcpy(newChildreen, childreen, (numberOfChildeen-1) * sizeof(Menu*));
	newChildreen[numberOfChildeen-1] = child;
	delete[] childreen;
	childreen = newChildreen;

	child->addParent(this);
}

void Menu::addParent(Menu *parent)
{
	this->parent = parent;
}

Menu *Menu::getChild(int id)
{
	if(id >= 0 && id < numberOfChildeen){
		return childreen[id];
	}else{
		printf("The child with an id of \"%i\" did not found\n",id);
		return NULL;
	}
}

Menu *Menu::getParent()
{
	return this->parent;
}

int Menu::getReverseDirCode(int directionCode)
{
	switch(directionCode){
		case UP:
		return DOWN;
		case DOWN:
		return UP;
		case LEFT:
		return RIGHT;
		case RIGHT:
		return LEFT;
		default:
		return -1;
	}
}

int Menu::getNumOfChildreen()
{
	return this->numberOfChildeen;
}

int Menu::getNumOfNeighbours()
{
	return this->numberOfNeightboors;
}

int Menu::getId()
{
	return this->id;
}

char *Menu::getName()
{
	return this->name;
}




