#ifndef _BUTTON_h
#define _BUTTON_h

#include "arduino.h"
#include "../Menu/Menu.h"

#define THRESHHOLD		50

typedef enum{
	None,
	Falling,
	Rising
}BtnState;


class Button{
	private:
		int btn;
		unsigned long t;
		bool is_pressed;
		BtnState state;
	public:
		Button();
		void init(int btn);
		void read();
		bool isPressed();
		bool isReleased();
};



#endif

