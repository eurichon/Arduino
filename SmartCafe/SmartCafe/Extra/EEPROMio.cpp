#include "EEPROMio.h"


EEPROM_IO::EEPROM_IO(EEPROM_CONFIG config){
	switch(config){
		case EEPROM_CONFIG::PAGE_1024:
		EEPROM.PageBase0 = 0x801F000;
		EEPROM.PageBase1 = 0x801F800;
		EEPROM.PageSize  = 0x400;
		break;
		case EEPROM_CONFIG::PAGE_2048:
		EEPROM.PageBase0 = 0x801F000;
		EEPROM.PageBase1 = 0x801F800;
		EEPROM.PageSize  = 0x800;
		break;
		default:
		break;
	}
	
	EEPROM.init();
}


int EEPROM_IO::isValidAddr(uint16_t addr){
	// find if this is a initialized address
	uint16_t data, status;
	bool pos_found = false;
	
	status = EEPROM.read(addr, &data);
	
	if(status != BAD_ADDRESS){		//also maybe status must be ok to prevent other errors
		// find if it holds a non-deleted item aka has a valid id
		status = EEPROM.read(addr + 6, &data);
		uint16_t id = data & 0b0001111111111111;
		
		if(status != BAD_ADDRESS && id > 0){
			return id;			
		}else if(status != BAD_ADDRESS && id == 0){
			return 0;
		}else{
			return -2;
		}
	}
	
	return -1;
}


bool EEPROM_IO::getBaseAddrById(int id, uint16_t &address){
	uint16_t base_addr, data, status;
	bool pos_found = false;
	base_addr = MENU_BASE_ADDR;
	
	while(!pos_found){
		status = EEPROM.read(base_addr, &data);
		
		if(status == BAD_ADDRESS){
			return false;
		}else{
			uint16_t word = EEPROM.read(base_addr + 6);
			word = word & 0b0001111111111111;
			
			if(word == id){
				address = base_addr;
				return true;
			}
		}
		
		base_addr = base_addr + ITEM_MEM_SIZE;
	}
	
}


bool EEPROM_IO::getItem(int id, Item &item){
	uint16_t base_addr, word, status;
	
	if(getBaseAddrById(id, base_addr)){
		item.id = id;
		
		EEPROM.read(base_addr + 6, &word);
		word = word >> 13;
		word = word & 0b0000000000000011;
		item.type = (MenuType)word;
		
		EEPROM.read(base_addr + 6, &word);
		word = word >> 15;
		item.available = word;
		
		EEPROM.read(base_addr + 5, &item.price);
		
		for(int i = 0; i < 5; i++){
			EEPROM.read(base_addr + i, &word);
			word = word >> 8;
			item.name[2 * i] = word & 0b0000000011111111;
			
			EEPROM.read(base_addr + i, &word);
			item.name[2 * i + 1] = word & 0b0000000011111111;
		}
		
		return true;
	}else{
		return false;
	}
}


void EEPROM_IO::addItem(Item &item){
	uint16_t base_addr, data, status;
	bool pos_found = false;
	base_addr = MENU_BASE_ADDR;
	
	// if the id already exists abort
	if(getBaseAddrById(item.id,  base_addr)){
		Serial.println("Id already exists!");
		return;
	}
	
	// find an empty position - toadd error detection
	while(!pos_found){
		status = EEPROM.read(base_addr, &data);
		if(status == BAD_ADDRESS){
			pos_found = true;
			}else{
			base_addr = base_addr + ITEM_MEM_SIZE;
		}
	}
	
	// write name to memory
	for(int i = 0; i < 5; i++){
		uint16_t word = item.name[2 * i];
		word = word << 8;
		word = word | (uint16_t)item.name[2 * i + 1];
		EEPROM.write(base_addr + i, word);
	}
	
	// write price to memeory
	EEPROM.write(base_addr + 5, item.price);
	
	// write availability, type, id
	uint16_t word = item.available;
	word = word << 15;
	uint16_t word2 = item.type;
	word2 = word2 << 12;
	uint16_t word3 = item.id;
	uint16_t result = (word | word2 | word3);
	EEPROM.write(base_addr + 6, result);
}


void EEPROM_IO::updateItem(Item &item){
	uint16_t base_addr;
	if(getBaseAddrById(item.id, base_addr)){
		// write name to memory
		for(int i = 0; i < 5; i++){
			uint16_t word = item.name[2 * i];
			word = word << 8;
			word = word | (uint16_t)item.name[2 * i + 1];
			EEPROM.update(base_addr + i, word);
		}
		
		// write price to mem?ry
		EEPROM.update(base_addr + 5, item.price);
		
		// write availability, type, id
		uint16_t word = item.available;
		word = word << 15;
		uint16_t word2 = item.type;
		word2 = word2 << 12;
		uint16_t word3 = item.id;
		uint16_t result = (word | word2 | word3);
		EEPROM.update(base_addr + 6, result);
	}
}


void EEPROM_IO::removeItem(Item &item){
	uint16_t base_addr;
	if(getBaseAddrById(item.id, base_addr)){
		// write availability, type, id
		EEPROM.write(base_addr + 6, 0);
	}
}


void EEPROM_IO::clearItems(){
	EEPROM.format();
	EEPROM.init();
}