#include "Menu/Menu.h"
#include "Extra/EEPROMio.h"
#include "Button/Button.h"



#define CE		PB0
#define CSN		PA4


void updateDisplay();


Button *Btn;
Menu *menu;

int numChars = 20;
bool newData;
char receivedChar;
char receivedChars[20];

const int btnList[5] = {PB12, PB13, PB14, PB15, PA8};
char msg[32];




void setup() {
	Btn = new Button[5]();
	
	for(int i = 0;i < 5; i++){
		Btn[i].init(btnList[i]);
	}
	
	Serial.begin(9600);
	
	menu = new Menu();
	newData = false;
	
	
	

}


void loop() {
	updateDisplay();
	
	recvOneChar();
	
	if(newData){
		newData = false;
		if(receivedChar == '0'){
			menu->update(Command::left);
		}else if(receivedChar == '1'){
			menu->update(Command::right);
		}else if(receivedChar == '2'){
			menu->update(Command::up);
		}else if(receivedChar == '3'){
			menu->update(Command::down);
		}else if(receivedChar == '4'){
			Serial.println("Add element");
			Item item;
			
			newData = false;
			Serial.print("Give item name: ");
			while(!newData){
				recvWithEndMarker();
			}
			newData = false;
			for(int i = 0;i < 10;i++){
				item.name[i] = receivedChars[i];
			}
			Serial.println(item.name);
			
			
			Serial.print("Give item price: ");
			while(!newData){
				recvWithEndMarker();
			}
			newData = false;
			item.price = atoi(receivedChars);
			Serial.println(item.price);
			
			
			Serial.print("Give item id: ");
			while(!newData){
				recvWithEndMarker();
			}
			newData = false;
			item.id = atoi(receivedChars);
			Serial.println(item.id);
			
			item.available = 1;
			item.type =  MenuType::Coffee;
			
			
			menu->addItem(item);
			
			Serial.println("Item added!");
		}else if(receivedChar == '5'){
			// update item
			Item item;
			
			Serial.print("Update element with id: ");
			while(!newData){
				recvWithEndMarker();
			}
			newData = false;
			item.id = atoi(receivedChars);
			Serial.println(item.id);
			
			
			newData = false;
			Serial.print("Give item's new name: ");
			while(!newData){
				recvWithEndMarker();
			}
			newData = false;
			for(int i = 0;i < 10;i++){
				item.name[i] = receivedChars[i];
			}
			if(item.name[0] == '\n'){
				Serial.println("Name unchanged");
				item.name[0] = '\0';
				}else{
				Serial.print("New item name: ");
				Serial.println(item.name);
			}
			
			
			Serial.print("Give item's new price: ");
			while(!newData){
				recvWithEndMarker();
			}
			newData = false;
			item.price = atoi(receivedChars);
			Serial.print("New price: ");
			Serial.println(item.price);
			
			item.available = 1;
			item.type =  MenuType::Coffee;
			
			menu->updateItem(item);
			
			Serial.println("Item updated!");
		}else if(receivedChar == '6'){
			// delete item
			Item item;
			
			Serial.print("delete item with id: ");
			while(!newData){
				recvWithEndMarker();
			}
			newData = false;
			item.id = atoi(receivedChars);
			item.type = MenuType::Coffee;
			Serial.println(item.id);
			
			menu->deleteItem(item);
			
			Serial.print("Item deleted!");
		}
	}
	
	
}


void updateDisplay(){
	// read buttons
	for(int i = 0;i < 5;i++){
		Btn[i].read();
		if(Btn[i].isPressed()){
			// update menu & eeprom
			menu->update((Command)i);
		}
	}
	// update display
	menu->display();
}



void recvOneChar() {
	while(Serial.available() > 0 && newData == false){
		char temp = Serial.read();
		if(temp == '\n'){
			newData = true;
		}else{
			receivedChar = temp;
		}
	}
}


void recvWithEndMarker() {
	static byte ndx = 0;
	char endMarker = '\n';
	char rc;
	
	while (Serial.available() > 0 && newData == false) {
		rc = Serial.read();

		if (rc != endMarker) {
			receivedChars[ndx] = rc;
			ndx++;
			if (ndx >= numChars) {
				ndx = numChars - 1;
			}
		}
		else {
			receivedChars[ndx] = '\0'; // terminate the string
			ndx = 0;
			newData = true;
		}
	}
}