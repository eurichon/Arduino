// MPU6050.h
#ifndef _MPU6050_h
#define _MPU6050_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "CircularBuffer.h"
#include "Wire.h"
#include "Register.h"

#define BUFF_SIZE 15
#define b 2.5
#define K 0.04

class MPU6050
{
public:
	MPU6050(byte gyro_setting, byte accel_setting,long t);
	void readSensorValues();
	void calculateCurrentAngle();
	void calculateAngleBias();
	void updateAngle();
	float getCurrentAngle();
	float getAngleBias();
	
		
	void printValues();
	short &getValues();
	short  &getMaxs();
	short &getMins();
	float &getAver();
	float getTemprature();

private:
	CircularBuffer<float> *circular_buffer;
	
	float angle_bias,
	      angle_estimate,
	      grav_angle_xy,
	      pre_angle_xy;
	unsigned long clock;

	int accel_range, gyro_range; //default setting according to datasheet
	float temprature;
	
	//table variables
	short max_measurment[6];
	short min_measurment[6];
	short measurment[6];
	float aver_measurment[6];
	const char names[6][15] = { "gyro x: ", "  gyro y: ", "  gyro z: ", "  accel x: " , "  accel y: ", "  accel z: "};
		
	void calculateGravAngle();
	void findAverage(unsigned long t_period);
	void setSensorRanges(int accel_setting, int gyro_setting);
	void setupMPU(byte accel_setting, byte gyro_setting);
};


#endif

