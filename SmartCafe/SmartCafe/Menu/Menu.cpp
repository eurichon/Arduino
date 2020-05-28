#include "Menu.h"


 Menu::Menu(){
	screen = new Adafruit_SSD1306(OLED_RESET);
	screen->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDR);
	
	eeprom = new EEPROM_IO(EEPROM_CONFIG::PAGE_2048);
	
	has_changed = true;
	
	selected_item = 0;
	selected_menu = 0;
	
	state = MenuState::MainMenu;
	
	// populate list by parsing eeprom
	uint16_t base_addr = MENU_BASE_ADDR;
	int id = eeprom->isValidAddr(base_addr);
	
	while(id >= 0){
		if(id > 0){
			Item item;
			eeprom->getItem(id, item);
			char temp[15];
			itemToString(item,temp);
			list[item.type].pushAfter(temp, id, item.price);
		}
				
		base_addr = base_addr + ITEM_MEM_SIZE;
		id = eeprom->isValidAddr(base_addr);
	}
}


void Menu::displayMainMenu(){
	screen->clearDisplay();

	screen->fillRect(0, 0, 128, 16, WHITE);
	
	int offset_x = (screen->width() - strlen(moto) * char_size_x)/2;
	int offset_y = (16 - char_size_y)/2;
	
	screen->setTextSize(1);
	screen->setTextColor(BLACK);
	screen->setCursor(offset_x, offset_y);
	screen->print(moto);
	
	screen->drawBitmap(0, 16, logo_icon, 128, 48, 1);
	
	screen->display();
}


void Menu::displayMenu(){
	screen->clearDisplay();
	
	screen->fillRect(0, 0, 128, 16, WHITE);
	
	int offset_x = (screen->width() - strlen(CategoryNames[selected_menu]) * char_size_x)/2;
	int offset_y = (16 - char_size_y)/2;
	screen->setTextColor(BLACK);
	screen->setCursor(offset_x, offset_y);
	screen->print(CategoryNames[selected_menu]);
	
	switch(selected_menu){
		case 0:
			screen->drawBitmap(0, 16, coffee_icon, IMAGE_WIDTH, IMAGE_HEIGHT, 1);
			break;
		case 1:
			screen->drawBitmap(0, 16, beverage_icon, IMAGE_WIDTH, IMAGE_HEIGHT, 1);
			break;
		case 2:
			screen->drawBitmap(0, 16, juice_icon, IMAGE_WIDTH, IMAGE_HEIGHT, 1);
			break;
		case 3:
			screen->drawBitmap(0, 16, snack_icon, IMAGE_WIDTH, IMAGE_HEIGHT, 1);
			break;
		default:
			break;
	}

	screen->display();
}


void Menu::displayItems(){
	screen->fillRect(44, 16, 84, 48, BLACK);
	
	Node *curr = list[selected_menu].getCurr();
	
	int index = 0;
	while(index < 5 && index < list[selected_menu].length()){
		if(curr != NULL){
			if(index == 0){
				screen->fillRect(44, 18 , 84, 9, WHITE);
				screen->setTextColor(BLACK);
			}else{
				screen->setTextColor(WHITE);
			}
			
			screen->setCursor(44 ,19 + index * 9);
			screen->print(curr->data);
			
			curr = curr->next;
		}
		index++;
	}
	
	screen->display();
}


void Menu::displayOrder(){
	screen->clearDisplay();

	screen->fillRect(0, 0, 128, 16, WHITE);
	int offset_x = (screen->width() - strlen(order) * char_size_x)/2;
	int offset_y = (16 - char_size_y)/2;
	
	screen->setTextSize(1);
	screen->setTextColor(BLACK);
	screen->setCursor(offset_x, offset_y);
	screen->print(order);
	
	screen->drawRect(0, 16, 128, 30, WHITE);
	
	screen->setTextColor(WHITE);
	for(int i = 0;i < 3;i++){
		screen->setCursor(5, 19 + i * 9);
		screen->print(order_table[i]);
	}
	
	char order_name[10];
	strncpy ( order_name, list[selected_menu].getCurr()->data , sizeof(order_name));
	screen->setCursor(65, 19);
	screen->print(order_name);
	
	screen->setCursor(65, 28);
	screen->print(quontity);
	
	float order_price = quontity * list[selected_menu].getCurr()->price;
	order_price = order_price / 100;
	screen->setCursor(65, 37);
	screen->print(order_price);
	
	
	// draw buttons
	if(order_state == OrderState::Accepted){
		screen->drawRect(65, 53, 26, 10, WHITE);
		screen->setTextColor(WHITE);
		screen->setCursor(67, 54);
		screen->print("Back");
		
		screen->fillRect(96, 53, 32, 10, WHITE);
		screen->setTextColor(BLACK);
		screen->setCursor(97, 54);
		screen->print("Order");
		
	}else{
		screen->fillRect(65, 53, 26, 10, WHITE);
		screen->setTextColor(BLACK);
		screen->setCursor(67, 54);
		screen->print("Back");
		
		screen->drawRect(96, 53, 32, 10, WHITE);
		screen->setTextColor(WHITE);
		screen->setCursor(97, 54);
		screen->print("Order");
		
	}
	
	
	screen->display();
}


void Menu::updateMainMenu(Command c){
	if(c >= 0 && c <= 4){
		state = MenuState::Selection;
		has_changed = true;
	}
}


void Menu::updateSelection(Command c){
	switch(c){
		case Command::middle: 
			if(list[selected_menu].length() != 0){
				state = MenuState::Confirmation;
				order_state = OrderState::Recalled;
				has_changed = true;
				quontity = 1;
			}
			break;
		case Command::up:
			list[selected_menu].next();
			has_changed = true;
			break;
		case Command::down:
			list[selected_menu].prev();
			has_changed = true;
			break;
		case Command::left:
			selected_menu = (selected_menu - 1 + MENU_CATEGORIES) % MENU_CATEGORIES;
			has_changed = true;
			break;
		case Command::right:
			selected_menu = (selected_menu + 1) % MENU_CATEGORIES;
			has_changed = true;
			break;
		default:
			break;
	}
}


void Menu::updateConfirmation(Command c){
	switch(c){
		case Command::middle:
			if(order_state == OrderState::Accepted){
				Serial.println("Order confirmed");
				dislayLoading();
			}else{
				Serial.println("Order cancelled");
			}
		
			state = MenuState::Selection;
			has_changed = true;
			state = MenuState::MainMenu;
						
			break;
		case Command::up:
			if(quontity < 5){
				order_state = OrderState::Recalled;
				quontity = quontity + 1;
				has_changed = true;
			}
			break;
		case Command::down:
			if(quontity > 1){
				order_state = OrderState::Recalled;
				quontity = quontity - 1;
				has_changed = true;
			}
			break;
		case Command::left:
			if(order_state == OrderState::Accepted){
				order_state = OrderState::Recalled;
				has_changed = true;
			}
			break;
		case Command::right:
			if(order_state == OrderState::Recalled){
				order_state = OrderState::Accepted;
				has_changed = true;
			}
			break;
		default:
			break;
	}
}


void Menu::itemToString(Item &item, char *str){
	bool name_ended = false; 
	for(int i = 0;i < 10; i++){
		if(item.name[i] == '\0'){
			name_ended = true;
		}
		
		if(name_ended){
			str[i] = 32;
		}else{
			str[i] = item.name[i];
		}
	}
	
	float price = item.price;
	price = price / 100;
	
	str[10] = 32;
	str[11] = '0' + (int)price;
	str[12] = 46;
	price = price -  (int)price;
	price = price * 10;
	str[13] = '0' + ceilf(price);
	str[14] = '\0';
}


void Menu::display(){
	if(has_changed){
		has_changed = false;
		
		switch(state){
			case MenuState::MainMenu:
				displayMainMenu();
				break;
			case MenuState::Selection:
				displayMenu();
				displayItems();
				break;
			case MenuState::Confirmation:
				displayOrder();
				break;
			default:
				break;
		}
	}
}


void Menu::update(Command c){
	switch(state){
		case MenuState::MainMenu:
			updateMainMenu(c);
			break;
		case MenuState::Selection:
			updateSelection(c);
			break;
		case MenuState::Confirmation:
			updateConfirmation(c);
			break;
		default:
			break;
	}
}


void Menu::addItem(Item &item){
	// add item to list
	char temp[15];
	itemToString(item,temp);
	list[item.type].pushAfter(temp, item.id, item.price);
	
	// add item to eeprom
	eeprom->addItem(item);
	
	has_changed = true;
}


void Menu::deleteItem(Item &item){
	// delete item to list
	Node *temp = list[item.type].getCurr();
	int index = 0;
	while(item.id != list[item.type].getCurr()->id && index < list[item.type].length()){
		list[item.type].next();
		index = index + 1;
	}
	
	if(index == list[item.type].length()){
		Serial.println("Wrong id");
	}else{
		list[item.type].pop();
		
		// add item to eeprom
		eeprom->removeItem(item);
		
		has_changed = true;
	}
}

void Menu::updateItem(Item &item){
	// update item to list
	Node *temp = list[item.type].getCurr();
	int index = 0;
	while(temp->id != item.id && index < list[item.type].length()){
		temp = temp->next;
		index = index + 1;
	}
	
	if(index == list[item.type].length()){
		Serial.println("Wrong id");
	}else{
		char data[15];
		itemToString(item, data);
		strncpy(temp->data, data, 15);
		temp->price = item.price;
		
		// add item to eeprom
		eeprom->updateItem(item);
		
		has_changed = true;
	}
}


void Menu::dislayLoading(){
	screen->clearDisplay();
	
	int offset_x = (screen->width() - strlen(order_accepted) * char_size_x)/2;
	int offset_y = (16 - char_size_y)/2;
	
	screen->fillRect(0, 0, 128, 16, WHITE);
	screen->setTextSize(1);
	screen->setTextColor(BLACK);
	screen->setCursor(offset_x, offset_y);
	screen->print(order_accepted);
	
	
	unsigned long t = millis();
	int i = 0;
	
	while(millis()-t < 2000){
		screen->fillRect(40, 18, 40, 40, BLACK);
		screen->drawBitmap(40, 18, animation[i],40, 40, 1);
		delay(4);
		screen->display();
		i = (i + 1) % 28;
	}
}