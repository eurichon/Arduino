import processing.serial.*;
import controlP5.*;

// Serial port & graphics
Serial myPort;
ControlP5 cp5;

Textlabel portText, baudrateText;
DropdownList ddl1, ddl2;

int[] baudrates = {300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, 115200};
int selected_port, selected_baudrate;
boolean isInit;
String[] pre_list;
long t;

// read input and clear all the incoming junk during serial communication with uno
boolean msg_started = false;
boolean msg_finished = true;
int[] buffer = new int[32];
int index = 0;
boolean newData = false;

// list of devices
IntList  devices;


enum InputState {
  Scan
}

InputState state = InputState.Scan;




void setup() {
  // set window size/color
  cp5 = new ControlP5(this);
  devices = new IntList();
  size(1200, 700);
  background(128);
  
  setupPortSelection();
  
  cp5.addButton("Scan")
     .setValue(0)
     .setPosition(20,100)
     .setSize(150 ,30)
     ;
   
  
}


void draw() {
  background(128);
 
  
  if(isInit){
    strokeWeight(2);
    fill(color(0,255,0));
    rect(595,25,20,20);
  }else{
    strokeWeight(2);
    fill(color(255,0,0));
    rect(595,25,20,20);
  }
  
  
  if(millis()-t > 1000){
    if(Serial.list().length != pre_list.length){
      updatePortList();
    }else{
      for(int i = 0;i < Serial.list().length; i++){
        if(Serial.list()[i].equals(pre_list[i])){
          updatePortList();
          break;
        }
      }
    }
    t = millis();
  }
  
  
  if(isInit){
    // read input
    readInput();
    
    if(newData){
      switch(state){
        case Scan:
          // check if device exist else add it
          boolean device_exists = false;
          for(int i = 0; i < devices.size();i++){
             if(buffer[0] == devices.get(i)){
               device_exists = true;
             }
          }
          if(!device_exists){
             devices.append(buffer[0]);
          }
          
          for(int i = 0; i < devices.size();i++){
             print("List is: ");
             println(devices.get(i));
          }
          
          break;
        default:
          break;
        
      }
      newData = false;
    }
  }
}




void readInput(){
  while(myPort.available() > 0){
      int inByte = (char)myPort.read();
      if(inByte == 200 && msg_finished){
        index = 0;
        msg_started = true;
        msg_finished = false;
      }else if(inByte == 255 && msg_started){
        msg_finished = true;
        msg_started = false;
      }else if(msg_started){
        buffer[index] = inByte;
        index++;
        newData = true;    
      }
    }
}






void controlEvent(ControlEvent theEvent) {
  if (theEvent.isGroup()) {
    
    println("event from group : "+theEvent.getGroup().getValue()+" from "+theEvent.getGroup());
  } 
  else if (theEvent.isController()) {
    if(theEvent.getController().getName().equals("port")){
      selected_port = parseInt(theEvent.getController().getValue());
      println("Selected port is: ", selected_port);
      
    }else if(theEvent.getController().getName().equals("baudrate")){
      selected_baudrate = parseInt(theEvent.getController().getValue());
      println("selected baudrate is: ", selected_baudrate);
    
    }
  }
}


public void Connect(int theValue){
  if(selected_port != -1 && selected_baudrate != -1 && !isInit){
      try{
        isInit = true;
        myPort = new Serial(this, Serial.list()[selected_port], baudrates[selected_baudrate]);
      }catch (Exception e){
        isInit = false;
        System.out.println(e); 
      }
      
      if(isInit){
        cp5.getController("Disconnect").show();
        cp5.getController("Connect").hide();
        ddl1.lock();
        ddl2.lock();
        println("Serial communication initialized");
      }
    }else{
      if(selected_port == -1 && selected_baudrate == -1){
        println("No port nor baudrate is selected!");
      }else if(selected_port == -1){
        println("No port");
      }else if(selected_baudrate == -1){
        println("No baudrate is selected!");
      }
    }
}


public void Disconnect(int theValue){
  if(isInit){
    myPort.stop();
    isInit = false;
    ddl1.unlock();
    ddl2.unlock();
    cp5.getController("Disconnect").hide();
    cp5.getController("Connect").show();
    println("Serial communication stopped");
  }else{
    println("Port not initialized");
  }
}


public void Scan(int theValue){
  if(isInit){
     state = InputState.Scan;
     myPort.write(7);
     myPort.write(255);
  }
}


void updatePortList(){
  ddl1.clear();
    for (int i = 0; i < Serial.list().length; i++) {
      ddl1.addItem(Serial.list()[i], i);
    }
}




void setupPortSelection(){
  // initialize selected menus
  ddl1 = cp5.addDropdownList("port").setPosition(125, 25);
  ddl2 = cp5.addDropdownList("baudrate").setPosition(370, 25);
  
  ddl1.setBackgroundColor(color(190));
  ddl1.setItemHeight(15);
  ddl1.setBarHeight(20);
  ddl1.getCaptionLabel().set("None selected");
 
  for (int i = 0; i < Serial.list().length; i++) {
    ddl1.addItem(Serial.list()[i], i);
  }
  pre_list = Serial.list();
  
  ddl1.setColorBackground(color(60));
  ddl1.setColorActive(color(255, 128));
  
  ddl2.setBackgroundColor(color(190));
  ddl2.setItemHeight(15);
  ddl2.setBarHeight(20);
  ddl2.getCaptionLabel().set("None selected");
 
  for (int i = 0; i < baudrates.length; i++) {
    ddl2.addItem(String.valueOf(baudrates[i]), i);
  }
  
  ddl2.setColorBackground(color(60));
  ddl2.setColorActive(color(255, 128));
  
  ddl1.close();
  ddl2.close();
  
  
  
  isInit = false;
  selected_port = -1;
  selected_baudrate = -1;
  t = millis();
  
  
  // initialize button
  
  cp5.addButton("Connect")
     .setValue(0)
     .setPosition(490,25)
     .setSize(100,21)
     ;
     
  cp5.addButton("Disconnect")
     .setValue(100)
     .setPosition(490,25)
     .setSize(100,21)
     ;
  
  portText = cp5.addTextlabel("port_label")
                    .setText("Select Port: ")
                    .setPosition(20,21)
                    .setColorValue(0xffffff00)
                    .setFont(createFont("Calibri",20))
                    ;
                    
  baudrateText = cp5.addTextlabel("baudrate_label")
                    .setText("Select baudrate:")
                    .setPosition(230,21)
                    .setColorValue(0xffffff00)
                    .setFont(createFont("Calibri",20))
                    ;             
  
  cp5.getController("Disconnect").hide();
}



void drawDevice(int x, int y, int id){
  strokeWeight(2);
  
  fill(color(0,50,50));
  rect(x, y, 200, 250);
  fill(color(200,50,50));
  
  String temp, light, humidity;
  temp = "temprature: 10";
  light = "light: 10";
  humidity = "humidity: 10";
  
  fill(color(255,255,255));
  text(temp, x + 20, y + 20);
  text(light, x + 20, y + 35);
  text(humidity, x + 20, y + 50);
  
 
  pushMatrix();
  translate(x + 100, y + 160);
  
  fill(color(255,50,10));
  circle(0, 0, 120);
  
  
  
  for(int i = 0; i < 4; i++){
      rotate(PI/2.0);
      fill(color(250,0,0));
      rect(50, 50, 20 ,20);
  }
  
  fill(color(255,255,255));
  text("id = " + id, -20, 0);
  
  popMatrix();
  
}
