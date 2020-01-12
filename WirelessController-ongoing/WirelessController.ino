
// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       WirelessController.ino
    Created:	5/7/2019 4:31:47 μμ
    Author:     DESKTOP-0SVRN6F\evrichon
*/

#define NONE 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define MIDDLE 5
#define PARENT 6


#include "Menu/Menu.h"
#include "RotaryEnc/RotaryEncoder.h"
#include "Sound/Sound.h"
#include "Sound/Tracks.h"
#include "MotionSensor/MPU6050.h"
#include <RF24_config.h>
#include <Wire.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
//=====================Definitions======================
#define CONTROL_BUTTONS A1



//=====================Functions==============
void initPins();

//=====================Global Variables==================
RotaryEncoder *encoder;
Menu *main_menu,*settings,*monitor,*radio,*select,*edit,*delete_menu,*new_menu,*c_e_name;
Menu *curr_menu,*pre_menu;
Sound *s;
MPU6050 *mpu;
int action;
uint8_t last_buttun_pressed;
unsigned long t;




void setup()
{	
	Serial.begin(9600);
	while(!Serial){};
	Serial.println("Starting");
	initPins();
	//Wire.begin();
	
	main_menu = NULL;
	settings = NULL;
	monitor = NULL;
	radio = NULL;
	select = NULL;
	edit = NULL;
	delete_menu = NULL;
	new_menu = NULL;
	c_e_name = NULL;
	
	
	Serial.print("Initializing menu....   ");
	//navigators
	pre_menu = NULL;
	
	
	//menu nodes
	main_menu = new Menu("Main Menu",0);
	settings = new Menu("Settings",1);
	monitor = new Menu("Monitor",2);
	radio = new Menu("Radio",3);
	select = new Menu("Select setting",4);
	edit = new Menu("edit setting",5);
	delete_menu = new Menu("delete setting",6);
	new_menu = new Menu("New setting",7);
	c_e_name = new Menu("Create/Edit name",8);
	
	//connections
	main_menu->addChild(settings);
	settings->addNeighbour(radio,RIGHT);
	radio->addNeighbour(c_e_name,DOWN);
	c_e_name->addNeighbour(monitor,LEFT);
	monitor->addNeighbour(settings,UP);
	
	settings->addChild(new_menu);
	new_menu->addNeighbour(edit,DOWN);
	edit->addNeighbour(select,DOWN);
	select->addNeighbour(delete_menu,DOWN);
	
	edit->addChild(c_e_name);
	new_menu->addChild(c_e_name);
	
	Serial.println("Menu initialized");
	
	
	Serial.print("Initializing sound...   ");
	s = new Sound();
	int melody_size;
	melody_size = sizeof(melody)/sizeof(int);
	if(!s->addMelody(melody_size,melody,noteDurations)){
		exit(1);
	}
    melody_size = sizeof(melody2)/sizeof(int);
	if(!s->addMelody(melody_size,melody2,noteDurations2)){
		exit(2);
	}
	Serial.println("Sound initialized");
	
	//s->start(0);
	mpu = new MPU6050(MPU_FS_SEL_0,MPU_AFS_SEL_0,1000);
	mpu->calculateAngleBias();
	

	//initialize buttons
	last_buttun_pressed = NONE;
	t = millis();
	
	Serial.println("Initializing completed");
	
	curr_menu = main_menu;
	
	curr_menu = curr_menu->getChild(2);
	Serial.println(curr_menu->getName());
	curr_menu = curr_menu->getNeighbour(UP);
	Serial.println(curr_menu->getName());
	
	
}


void loop()
{
	
	//updateAction();
	static int pos = 0;
	int newPos = encoder->getPosition();
	if (pos != newPos) {
		Serial.print(newPos);
		Serial.println();
		pos = newPos;
	} 


}



void initPins(){
	//Menu buttons
	pinMode(CONTROL_BUTTONS,INPUT);
	
	//Right joystick
	pinMode(A2,INPUT);
	pinMode(A3,INPUT);
	
	//Left Joystick
	pinMode(A6,INPUT);
	pinMode(A7,INPUT);
	
	//Rotary encoder
	
	encoder = new RotaryEncoder(6,7);
	cli();
	PCICR |= (1 << PCIE2);    // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
	PCMSK2 |= (1 << PCINT22) | (1 << PCINT23);  // This enables the interrupt for pin 6 and 7 of Port D.
	sei();
}

/*
void updateAction(){
	int val = analogRead(CONTROL_BUTTONS);
	//Serial.println(val);
	if(val == 0){
		action = NONE;
	}else if(val > 80 && val < 105 && !(action == UP) && (millis()-t) > 100 ){
		action = UP;
		t = millis();
		executeCommand();
	}else if(val > 140 && val < 150 && !(action == RIGHT) && (millis()-t) > 100 ){
		action = RIGHT;
		t = millis();
		executeCommand();
	}else if(val > 195 && val < 210 && !(action == DOWN) && (millis()-t) > 100 ){
		action = DOWN;
		t = millis();
		executeCommand();
	}else if(val > 280 && val < 310 && !(action == LEFT) && (millis()-t) > 100 ){
		action = LEFT;
		t = millis();
		executeCommand();
	}else if(val > 460 && val < 470 && !(action == MIDDLE) && (millis()-t) > 100 ){
		action = MIDDLE;
		t = millis();
		executeCommand();
	}else{
		return;
	}
}
*/

void executeCommand(){
	pre_menu = curr_menu;
	switch(action){
		case UP:
			curr_menu = curr_menu->getNeighbour(action);
			break;
		case DOWN:
			curr_menu = curr_menu->getNeighbour(action);
			break;
		case LEFT:
			curr_menu = curr_menu->getNeighbour(action);
			break;
		case RIGHT:
			curr_menu = curr_menu->getNeighbour(action);
			break;
		case MIDDLE:
			curr_menu = curr_menu->getChild(0);
			break;
		case PARENT:
			curr_menu = curr_menu->getParent();
			break;
		default:
			break;
	}
	if(curr_menu == nullptr){
		curr_menu = pre_menu;
		Serial.print("Invalid choice  ");
		Serial.print(action);
	}
	curr_menu->info();
	action = -1;
}


ISR(PCINT2_vect) {
	encoder->tick(); // just call tick() to check the state.
}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
	s->play();
}