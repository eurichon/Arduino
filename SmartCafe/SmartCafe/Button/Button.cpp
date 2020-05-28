#include "Button.h"



 Button::Button(){
	is_pressed = false;
	state = BtnState::None;
	t = 0;
}


void Button::init(int btn){
	this->btn = btn;
	pinMode(btn, INPUT_PULLUP);
}


void Button::read(){
	bool value = digitalRead(btn);
	
	if(!value && !is_pressed){
		if((millis() - t) > THRESHHOLD){
			is_pressed = true;
			state = BtnState::Falling;
			t = millis();
		}		
	}else if(value && is_pressed){
		if((millis() - t) > THRESHHOLD){
			is_pressed = false;
			state = BtnState::Rising;
			t = millis();
		}
	}
}


bool Button::isPressed(){
	if(state == BtnState::Falling){
		state = BtnState::None;
		return true;
	}else{
		return false;
	}
}


bool Button::isReleased(){
	if(state == BtnState::Rising){
		state = BtnState::None;
		return true;
	}else{
		return false;
	}
}

