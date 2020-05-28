#ifndef _MENU_h
#define _MENU_h


#include "Arduino.h"
#include <Wire.h>
#include "loading.h"
#include "icons.h"
#include "../Extra/EEPROMio.h"
#include "../List/List.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306_STM32.h>


#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDR     0x3C


// Names of the categories contained within the menu
const char CategoryNames[4][10] = {"Coffee", "Beverages", "Juices", "Snacks"};
const char order_table[3][10] = {"Product", "Quantity", "Price"};
// Char array containing the moto message
const char moto[20] = {"Easy-Efficient-Fast"};
const char order[6] = {"Order"};
const char order_accepted[10] = {"Thank you"};
	


// Enum containing the possible Menu inputs
typedef enum{
	middle,
	up,
	down,
	left,
	right,
	none,
}Command;


// Enum containing the possible Menu states
typedef enum{
	MainMenu,
	Selection,
	Confirmation
}MenuState;


typedef enum{
	Accepted,
	Recalled
}OrderState;


class Menu{
	private:
		MenuState state;
		Adafruit_SSD1306 *screen;	
		List list[MENU_CATEGORIES];
		EEPROM_IO *eeprom;
		
		bool has_changed;
		
		uint8_t selected_menu,
				selected_item;
		
		uint8_t quontity;
		OrderState order_state;
		
		const int char_size_x = 6;
		const int char_size_y = 7;
		
		void displayMainMenu();
		void displayMenu();
		void displayItems();
		void displayOrder();
		void dislayLoading();
		
		void updateMainMenu(Command c);
		void updateSelection(Command c);
		void updateConfirmation(Command c);
		
		void itemToString(Item &item, char *str);
	public:
		Menu();	
		void display();
		void update(Command c);
		void addItem(Item &item);
		void deleteItem(Item &item);
		void updateItem(Item &item);
		void clear();
};



#endif	// _MENU_h

