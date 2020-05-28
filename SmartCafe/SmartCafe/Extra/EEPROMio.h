#ifndef EEPROMIO_H_
#define EEPROMIO_H_

#include <EEPROM.h>
#include <Arduino.h>
#include <flash_stm32.h>


#define MENU_BASE_ADDR			0x0000
#define MENU_CATEGORIES			4
#define BAD_ADDRESS				((uint16)0x0082)
#define ITEM_MEM_SIZE			7


/*	
	Memory structure of every item
	##########	|##		|#   |#
    name		|price	|id  |availability
	
	Initial memory structure of 16 bytes
	####              |############
	number of items   |not used yet
	in each category  |
*/


// Enumeration containing the Menu categories
typedef enum{
	Coffee,
	Juices,
	Beverage,
	Snack
}MenuType;


// Structure containing the fields of each item stored in EEPROM
typedef struct{
	char name[10];
	uint16_t price;
	uint16_t id;
	bool available;
	MenuType type;
}Item;


typedef enum{
	PAGE_1024,
	PAGE_2048
}EEPROM_CONFIG;


class EEPROM_IO{
	private:
		// Base addresses in EEPROM for each category respectivly 
		const int categ_base_addr[4] = {16, 268, 520, 772};
		// Base addresses of free memory space for each category
		int categ_abso_addr[4];
		// Number of elements contains in each category
		uint8_t categ_elem_num[4];
		
		bool getBaseAddrById(int id, uint16_t &address);
	public:
		EEPROM_IO(EEPROM_CONFIG config);
		int isValidAddr(uint16_t addr);
		bool getItem(int id, Item &item);
		void addItem(Item &item);
		void updateItem(Item &item);
		void removeItem(Item &item);
		void clearItems();
};


#endif /* EEPROMIO_H_ */