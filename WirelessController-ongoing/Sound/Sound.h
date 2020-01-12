/* 
* Sound.h
*
* Created: 8/7/2019 2:32:15 πμ
* Author: evrichon
*/

#ifndef __SOUND_H__
#define __SOUND_H__

#pragma once
#include "Pitch.h"
#include "Arduino.h"

#define BUZZER 8

class Sound
{
	private:
		int **track_melody,
			**track_duration;
		int *track_size;
		int number_of_tracks,
			current_melody,
			current_track;
	public:
		Sound();
		bool addMelody(int size, int *melody,int *duration);
		bool start(int track);
		bool isPlaying();
		void stop();
		void play();
};



#endif
