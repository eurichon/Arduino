#include <Servo.h>

Servo motor, elevator, rudder, left_aileron, right_aileron, left_flap, right_flap;

#define INIT_POS 100
#define MIN 630
#define MAX 2370

#define b_port PCIE0
#define c_port PCIE1
#define d_port PCIE2

#define enable_pin_0 0
#define enable_pin_1 1
#define enable_pin_2 2
#define enable_pin_3 3
#define enable_pin_4 4
#define enable_pin_5 5
#define enable_pin_6 6
#define enable_pin_7 7

/*
   PCINT0_vect handles  D8 - D13 -> Port B
   PCINT1_vect handles A0 - A5 -> port C
   PCINT2_vect handles D0 - D7 -> Port D
*/
volatile unsigned long dt0, dt1, dt2, dt3, dt4, dt5;
volatile unsigned long result0, result1, result2, result3, result4, result5;
float error_l_ai, error_r_ai, error_l_f, error_r_f,error_r2_f;
int select;
bool is_pressed;

typedef struct Bits {
  unsigned int pin_0: 1;
  unsigned int pin_1: 1;
  unsigned int pin_2: 1;
  unsigned int pin_3: 1;
  unsigned int pin_4: 1;
  unsigned int pin_5: 1;
  unsigned int pin_6: 1;
  unsigned int pin_7: 1;
} Bits;

typedef union u {
  Bits bitarray;
  byte Pin;
} Port;

volatile Port previous;

void enablePCI(byte port) {
  PCICR = PCICR | (1 << port);
}

void dissablePCI(byte port) {
  PCICR = PCICR & (!(1 << port));
}

void enablePin(byte pos) {
  PCMSK0 = PCMSK0 | (1 << pos);
}



void trimServo() {
  int input = analogRead(A0);

  if (input >= 480 && input < 550 && !is_pressed) {
    select = (++select) % 5;
    is_pressed = true;
  } else if (input >= 550 && input < 700) {
    switch (select) {
      case 0:
        error_l_ai = error_l_ai + 0.2;
        break;
      case 1:
        error_r_ai = error_r_ai + 0.2;
        break;
      case 2:
        error_l_f = error_l_f + 0.2;
        break;
      case 3:
        error_r_f = error_r_f + 0.2;
        break;
      case 4:
        error_r2_f = error_r2_f + 0.2;
        break;
      default:
        break;
    }
  } else if (input >= 700 && input < 850) {
    switch (select) {
      case 0:
        error_l_ai = error_l_ai - 0.2;
        break;
      case 1:
        error_r_ai = error_r_ai - 0.2;
        break;
      case 2:
        error_l_f = error_l_f - 0.2;
        break;
      case 3:
        error_r_f = error_r_f - 0.2;
        break;
      case 4:
        error_r2_f = error_r2_f - 0.2;
        break;
      default:
        break;
    }
  }else if(input >= 850 && input < 1200 && is_pressed){
    is_pressed = false;
  }
}



void setup() {
  // put your setup code here, to run once
  cli();
  PCMSK0 =  0b00111111;
  select = 0;
  is_pressed = false;
  
  pinMode(A0, INPUT);

  previous.Pin = 0;
  pinMode(13, INPUT);
  digitalWrite(13, HIGH);
  pinMode(12, INPUT);
  digitalWrite(12, HIGH);
  pinMode(11, INPUT);
  digitalWrite(11, HIGH);
  pinMode(10, INPUT);
  digitalWrite(10, HIGH);
  pinMode(9, INPUT);
  digitalWrite(9, HIGH);
  pinMode(8, INPUT);
  digitalWrite(8, HIGH);

  sei();

  enablePCI(b_port);
  delay(1000);
  motor.attach(6, MIN, MAX);
  elevator.attach(7, MIN, MAX);
  rudder.attach(5, MIN, MAX);
  left_aileron.attach(4, MIN, MAX);
  right_aileron.attach(3, MIN, MAX);
  left_flap.attach(2, MIN, MAX);
  right_flap.attach(1, MIN, MAX);
  delay(2000);

  error_l_ai = INIT_POS - left_aileron.read();
  error_r_ai = INIT_POS - right_aileron.read();
  error_l_f = INIT_POS - left_flap.read();
  error_r_f = INIT_POS - right_flap.read();
  error_r2_f = INIT_POS - right_flap.read();
  //Serial.begin(9600);
}



ISR(PCINT0_vect)
{
  unsigned long int t = micros();
  Port current;
  current.Pin = PINB;
  if (current.bitarray.pin_0 != previous.bitarray.pin_0) {
    previous.bitarray.pin_0 = current.bitarray.pin_0;
    if (previous.bitarray.pin_0) {
      dt0 = t;
    } else {
      result0 = t - dt0;
    }
  }
  if (current.bitarray.pin_1 != previous.bitarray.pin_1) {
    previous.bitarray.pin_1 = current.bitarray.pin_1;
    if (previous.bitarray.pin_1) {
      dt1 = t;
    } else {
      result1 = t - dt1;
    }
  }
  if (current.bitarray.pin_2 != previous.bitarray.pin_2) {
    previous.bitarray.pin_2 = current.bitarray.pin_2;
    if (previous.bitarray.pin_2) {
      dt2 = t;
    } else {
      result2 = t - dt2;
    }
  }
  if (current.bitarray.pin_3 != previous.bitarray.pin_3) {
    previous.bitarray.pin_3 = current.bitarray.pin_3;
    if (previous.bitarray.pin_3) {
      dt3 = t;
    } else {
      result3 = t - dt3;
    }
  }
  if (current.bitarray.pin_4 != previous.bitarray.pin_4) {
    previous.bitarray.pin_4 = current.bitarray.pin_4;
    if (previous.bitarray.pin_4) {
      dt4 = t;
    } else {
      result4 = t - dt4;
    }
  }
  if (current.bitarray.pin_5 != previous.bitarray.pin_5) {
    previous.bitarray.pin_5 = current.bitarray.pin_5;
    if (previous.bitarray.pin_5) {
      dt5 = t;
    } else {
      result5 = t - dt5;
    }
  }
}


void loop() {
  int airleron_angle = map(result0, 1000, 2000, 0, 180);

  if (result5 > 1500) {
    //flaperors
    int flap_angle = map(result4, 1000, 2000, 0, 60);
    right_flap.write(flap_angle - (error_r_f));
    left_flap.write(flap_angle - (error_l_f));

    right_aileron.write(180 - airleron_angle + (error_r_ai));
    left_aileron.write(airleron_angle - (error_l_ai));
  } else {
    //ailerons
    right_aileron.write(180 - airleron_angle + (error_r_ai));
    left_aileron.write(airleron_angle - (error_l_ai));
    right_flap.write(180 - airleron_angle + (error_r2_f));
    left_flap.write(airleron_angle - (error_l_f));
  }
  elevator.write(map(result1, 1000, 2000, 0, 180));
  rudder.write(map(result3, 1000, 2000, 0, 180));
  delay(20);

  //Serial.println(analogRead(A0));
  trimServo();
//  Serial.print(analogRead(A0));
//  Serial.print(" ");
//  Serial.print(select);
//  Serial.print(" ");
//  Serial.print(error_r_ai);
//  Serial.print(" ");
//  Serial.print(error_l_ai);
//  Serial.print(" ");
//  Serial.print(error_r_f);
//  Serial.print(" ");
//  Serial.println(error_l_f);

  //    Serial.print(result0);
  //    Serial.print(" ");
  //    Serial.print(result1);
  //    Serial.print(" ");
  //    Serial.print(result2);
  //    Serial.print(" ");
  //    Serial.print(result3);
  //    Serial.print(" ");
  //    Serial.print(result4);
  //    Serial.print(" ");
  //    Serial.println(result5);
}
