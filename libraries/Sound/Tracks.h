/*
 * Tracks.h
 *
 * Created: 7/8/2019 10:35:50 μμ
 *  Author: evrichon
 */ 


#ifndef TRACKS_H_
#define TRACKS_H_

const int melody[] = {
	NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
const int noteDurations[] = {
	4, 8, 8, 4, 4, 4, 4, 4
};

const int melody2[] = {
	NOTE_E5, NOTE_E5, NOTE_E5,
	NOTE_E5, NOTE_E5, NOTE_E5,
	NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
	NOTE_E5,
	NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
	NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
	NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
	NOTE_D5, NOTE_G5
};

const int noteDurations2[] = {
	8, 8, 4,
	8, 8, 4,
	8, 8, 8, 8,
	2,
	8, 8, 8, 8,
	8, 8, 8, 16, 16,
	8, 8, 8, 8,
	4, 4
};




#endif /* TRACKS_H_ */