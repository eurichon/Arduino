void setup() {
/*
 * SmartCafe.ino
 *
 * Created: 5/14/2020 3:47:29 PM
 * Author: evrichon
 */ 

#include "Extra/EEPROMio.h"

const byte numChars = 15;
char receivedChars[numChars];
boolean newData = false;
int c = 0;
EEPROM_IO *io;
char input;


const char menu_names[4][10] = {"Coffee", "Beverages", "Juices", "Snacks"};
const char item_avail[2][15] = {"available", "not available"};


void setup()
{
  Serial.begin(115200);
  while(!Serial);
  delay(500);
  Serial.println("Starting!");
  
  EEPROM.PageBase0 = 0x801F000;
  EEPROM.PageBase1 = 0x801F800;
  EEPROM.PageSize  = 0x800;
  
  
  io = new EEPROM_IO(EEPROM_CONFIG::PAGE_2048);
  
  while(1){
    recvOneChar();
    if(newData){
      newData = false;
      
      if(input == '0'){                 // display items
        Serial.println("Current menu items");
        DisplayPages(0x70);
        
      }else if(input == '1'){               // clear items  
        Serial.println("Items deleted");
        io->clearItems();
        Serial.println("Items cleared!");
      }else if(input == '2'){               // add element
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
        io->addItem(item);
        Serial.println("Item added!");
        
      }else if(input == '3'){               // get element
        Serial.print("Get element by id: ");
        newData = false;
        while(!newData){
          recvWithEndMarker();
        }
        newData = false;

        int id = atoi(receivedChars);
        Serial.println(id);
        
        Item item;
        if(io->getItem(id,item)){
          Serial.println(item.name);  
          Serial.println((float)item.price/100);  
          Serial.println(item_avail[item.available]); 
          Serial.println(menu_names[item.type]);  
          Serial.println(item.id);  
        }else{
          Serial.println("Item doesnt exist");
        }
        
      }else if(input == '4'){               // update item
        Serial.print("Update item by id: ");
        while(!newData){
          recvWithEndMarker();
        }
        newData = false;
        int id = atoi(receivedChars);
        Serial.println(id);
        
        Item item;
        if(io->getItem(id,item)){
          Serial.println(item.name);
          Serial.println((float)item.price/100);
          Serial.println(item_avail[item.available]);
          Serial.println(menu_names[item.type]);
          Serial.println(item.id);
          
          
          item.price++;
          Serial.print("Setting price: ");
          Serial.println(item.price);
          io->updateItem(item);
          Serial.println("Item updated!");
        }else{
          Serial.println("Item doesnt exist");
        }
        
        
      }else if(input == '5'){               // delete item
        Serial.print("Delete item by id: ");
        while(!newData){
          recvWithEndMarker();
        }
        newData = false;
        int id = atoi(receivedChars);
        Serial.println(id);
        Item item;
        item.id = id;
        
        io->removeItem(item);
        Serial.println("Item deleted");
      }
    }
  }
  
  
  
  
  
  
}

void loop()
{
  
}








void DisplayHex(uint16 value)
{
  if (value <= 0xF)
  Serial.print("000");
  else if (value <= 0xFF)
  Serial.print("00");
  else if (value <= 0xFFF)
  Serial.print("0");
  Serial.print(value, HEX);
}

void DisplayPages(uint32 endIndex)
{
  Serial.println("Page 0     Top         Page 1");

  for (uint32 idx = 0; idx < endIndex; idx += 4)
  {
    Serial.print  (EEPROM.PageBase0 + idx, HEX);
    Serial.print  (" : ");
    DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx));
    Serial.print  (" ");
    DisplayHex(*(uint16*)(EEPROM.PageBase0 + idx + 2));
    Serial.print  ("    ");
    Serial.print  (EEPROM.PageBase1 + idx, HEX);
    Serial.print  (" : ");
    DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx));
    Serial.print  (" ");
    DisplayHex(*(uint16*)(EEPROM.PageBase1 + idx + 2));
    Serial.println();
  }
}



void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  
  // if (Serial.available() > 0) {
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

void recvOneChar() {
  while(Serial.available() && newData == false){
    char c = Serial.read();
    
    if(c != '\n'){
      input = c;
    }else{
      newData = true;
    }
  }
}
