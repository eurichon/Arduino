/* 
* Sound.cpp
*
* Created: 8/7/2019 2:32:15 πμ
* Author: evrichon
*/

#include "Sound.h"


Sound::Sound()
{
	this->number_of_tracks = 0;
	this->current_track = -1;
	this->current_melody = -1;
	
	track_melody = NULL;
	track_duration = NULL;
	track_size = NULL;
}

bool Sound::addMelody(int size, int *melody, int *duration)
{
	//add a track dynamically
	number_of_tracks = number_of_tracks + 1;
	track_size = (int*)realloc(track_size, number_of_tracks * sizeof(int));
	
	if(track_size != NULL){
		track_size[number_of_tracks-1] = size;
	}else{
		Serial.println("Out of memory (track_size)");
		return false;
	}
	
	track_melody = (int**)realloc(track_melody,number_of_tracks * sizeof(int*));
	if(track_melody != NULL){
		track_melody[number_of_tracks-1] = melody;
	}else{
		Serial.println("Out of memory (track_melody)");
		return false;
	}
	
	track_duration = (int**)realloc(track_duration,number_of_tracks * sizeof(int*));
	if(track_duration != NULL){
		track_duration[number_of_tracks-1] = duration;
	}else{
		Serial.println("Out of memory (track_duration)");
		return false;
	}
	
	return true;
}



bool Sound::start(int track)
{
	current_track = track;
	current_melody = 0;
	if( current_track >= 0 && current_track < number_of_tracks){
		//initialize timer
		cli();
		//set timer1 interrupt at 1Hz
		TCCR1A = 0;// set entire TCCR1A register to 0
		TCCR1B = 0;// same for TCCR1B
		TCNT1  = 0;//initialize counter value to 0
		// set compare match register for 1hz increments
		//OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
		this->play();
		// turn on CTC mode
		TCCR1B |= (1 << WGM12);
		// Set CS10 and CS12 bits for 1024 prescaler
		TCCR1B |= (1 << CS12) | (1 << CS10);
		// enable timer compare interrupt
		TIMSK1 |= (1 << OCIE1A);
		sei();
		return true;
	}else{
		return false;
	}
}

bool Sound::isPlaying()
{
	if(current_melody != -1){
		return true;
	}else{
		return false;
	}
}

void Sound::stop()
{
	current_track = -1;
	current_melody = -1;
	cli();
	TIMSK1 &= !(1 << OCIE1A);
	sei();
}

void Sound::play()
{
	if(current_melody >= 0 && current_melody < track_size[current_track]){
		int toneDuration = 1000 / track_duration[current_track][current_melody];
		int melody = track_melody[current_track][current_melody];
		tone(BUZZER, melody, toneDuration);
		if((current_melody + 1) <  track_size[current_track]){
			current_melody++;
			cli();
			OCR1A = (int)((20.15 * toneDuration) - 1);
			sei();
		}else{
			this->stop();	
		}
	}
}
