#ifndef GPS_LIBRARY_H
#define GPS_LIBRARY_H
#pragma once

class GPS
{
  private:
    const char *sentenece[4] = { "GPGGA", "GPGSA", "GPGSV", "GPRMC" };
    const int steps[4] = {13,10,10,13};
    bool is_ready,sentence_started;
    String str;
    void sentence_has_started();
    void section_finished();
    int sentenece_index;
    int step_counter;

    //important variable;
    long int UTC,date;
    int sat_number;
    float speed,altitude,height,hdp;
    char dir_1[2],dir_2[2];

    long int hours,mins,secs;
    float dd_la,dd_lo, latitube,longitube;

    void dmm_to_dd();
    void set_time();
  public:
    GPS();
    void GPS_update(char c);
    bool GPS_is_ready();
    void print_back();
};


GPS::GPS(){
  is_ready = false;
  sentence_started = false;
  this->str = "";
  sentenece_index = 0;
  step_counter = 0;
}

void GPS::set_time(){
  long int temp = UTC;
  
  this->hours = temp/10000;
  temp = temp - hours*10000;
  this->mins = temp/100;
  temp = temp - mins*100;
  this->secs = temp;
  this->hours = hours ;

  Serial.print("time is : ");
  Serial.print(hours);
  Serial.print(":");
  Serial.print(mins);
  Serial.print(":");
  Serial.println(secs);
}


void GPS::dmm_to_dd(){
  long int first = latitube/100;
  float sec = latitube - first * 100;
  this->dd_la = first + sec/60;

  first = longitube/100;
  sec = longitube - first * 100;
  this->dd_lo = first + sec/60;
  
  Serial.print("latitude is: ");
  Serial.println(dd_la,5);
  Serial.print("longitube is:  ");
  Serial.print("   ");
  Serial.println(dd_lo,5);
}

bool GPS::GPS_is_ready(){
  if(is_ready){
    is_ready = false;
    return true;
  }else
    return false;
}

void GPS::GPS_update(char c){
  switch(c){
    case '$':
      sentence_has_started();
      //Serial.println("$ case");
      break;
    case ',':
      section_finished();
      //Serial.println(", case");
      break;      
    case '\n':
    
      break;
    case '*':

      break;
    default:
      str = str + c;
      break;
  }
}


void GPS::sentence_has_started(){
  this->str = "";
  sentence_started = true;
  step_counter = 0 ;
  sentenece_index = -1;
}


void GPS::section_finished(){
  if(sentence_started){
    char temp[6];
    str.toCharArray(temp,6);
    for(int i = 0;i < 4; ++i){
      if(strcmp(sentenece[i], temp)==0){
        sentenece_index = i;
        //Serial.print("Index is: ");  
        //Serial.println(sentenece_index);    
        //step_counter = steps[i];
      }    
    }sentence_started = false;
    str = "";  
  }else{
    switch(sentenece_index){
      case 0:
        switch(step_counter){
          case 6:
            sat_number = str.toInt();
            Serial.print("The number of satelites is:  ");
            Serial.println(sat_number); 
            break;
          case 7:
            hdp = str.toFloat();
            Serial.print("The Horizontal dilution of position is:  ");
            Serial.println(hdp); 
            break;
          case 8:
            altitude = str.toFloat();
            Serial.print("The altitude is:  ");
            Serial.println(altitude); 
            break;
          case 10:
            height = str.toFloat();
            Serial.print("The height is:  ");
            Serial.println(height); 
            Serial.println("==============================");
            break;
            
          default:
            break;
        }
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        switch(step_counter){
          case 0:
              UTC = str.toInt();
              //Serial.print("Time is:  ");
              //Serial.println(UTC);    
              this->set_time();
            break;
          case 2:
              latitube = str.toDouble();
              //Serial.print("latitube is:  ");
              //Serial.println(latitube);    
            break;
          case 3:
              str.toCharArray(dir_1, 2);
              //Serial.print("dir_1 is:  ");
              //Serial.println(dir_1);    
            break;
          case 4:
              longitube = str.toDouble();
              //Serial.print("longitube is:  ");
              //Serial.println(longitube);    
              this->dmm_to_dd();
            break;
          case 5:
              str.toCharArray(dir_2, 2);
              //Serial.print("dir_2 is:  ");
              //Serial.println(dir_2);  
            break;  
          case 6:
              speed = str.toFloat() * 0.5144;
              Serial.print("speed is:  ");
              Serial.println(speed);    
              Serial.print("m/s or ");
              Serial.print(speed * 3.6);    
              Serial.println("km/h");    
              break;
          case 8:
              date = str.toInt();
              Serial.print("date is:  ");
              Serial.println(date);            
              break;
          default:
            //Serial.println("Ignore section"); 
            break;             
        }
        break;
      default:
        break;     
    }
    str = "";
    if(step_counter == steps[sentenece_index]){
      is_ready = true;
      step_counter = 0 ;
    }
    step_counter ++;
    
  }
}

//#GPRMC,1234,A,345.56,S,365.23,N,

//testing
void GPS::print_back(){
  Serial.println(sentenece[1]);
}







#endif
