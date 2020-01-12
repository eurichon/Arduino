#include "MPU6050.h"

MPU6050::MPU6050(byte gyro_setting, byte accel_setting,long t)
{
	circular_buffer = new CircularBuffer<float>(BUFF_SIZE);
	angle_bias = 0;
	angle_estimate = 0;
	grav_angle_xy = 0;
	pre_angle_xy = 0;
	clock = micros();
	
	foreach(i,6){
		measurment[i] = 0;
		min_measurment[i] = 0;
		max_measurment[i] = 0;
	}
	Serial.println("inside constructor");
	this->setupMPU(accel_setting,gyro_setting);
	Serial.println("here");
	this->findAverage(t);
	Serial.println("there");
	this->calculateAngleBias();
	Serial.println("go");
}


void MPU6050::setupMPU(byte accel_setting, byte gyro_setting)
{
	Wire.beginTransmission(MPU_addr); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
	Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
	Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
	Wire.endTransmission();
	//----------------------------------------
	Wire.beginTransmission(MPU_addr); //I2C address of the MPU
	Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4)
	Wire.write(gyro_setting); //Setting the gyro scale
	Wire.endTransmission();
	//----------------------------------------
	Wire.beginTransmission(MPU_addr); //I2C address of the MPU
	Wire.write(0x1C); //Accessing the register 1C - Accelerometer Configuration (Sec. 4.5)
	Wire.write(accel_setting); //Setting the accel scale
	Wire.endTransmission();

	this->setSensorRanges(accel_setting, gyro_setting);
}

void MPU6050::readSensorValues()
{
	Wire.beginTransmission(MPU_addr); //I2C address of the MPU
	Wire.write(0x3B); //Starting register for Accel Readings
	Wire.endTransmission();
	Wire.requestFrom(MPU_addr, 14); //Request Accel Registers (3B - 40)
	while (Wire.available() < 14);
	measurment[3] = Wire.read() << 8 | Wire.read(); //Store first two bytes into accelX
	measurment[4] = Wire.read() << 8 | Wire.read(); //Store middle two bytes into accelY
	measurment[5] = Wire.read() << 8 | Wire.read(); //Store last two bytes into accelZ

	temprature = Wire.read() << 8 | Wire.read(); //Store two bytes of temprature

	measurment[0] = Wire.read() << 8 | Wire.read(); //Store first two bytes into gyroX
	measurment[1] = Wire.read() << 8 | Wire.read(); //Store middle two bytes into gyroY
	measurment[2] = Wire.read() << 8 | Wire.read(); //Store last two bytes into gyroZ
}

void MPU6050::printValues()
{
	for (int i = 0;i<6;++i) {
		Serial.print(names[i]);
		Serial.print(measurment[i]);
	}
	Serial.println("");
}

void MPU6050::findAverage(unsigned long t_period)
{
	this->readSensorValues();
	//calculate min and max values for y,z angles
	foreach(i, 6) {
		this->max_measurment[i] = measurment[i];
		this->min_measurment[i] = measurment[i];
		this->aver_measurment[i] = 0;
	}
	unsigned long t = millis();
	int counter = 0;
	Serial.println("Calibrating..");
	while ((millis() - t) <= t_period)
	{
		readSensorValues();
		foreach(i, 6) {
			if (max_measurment[i] < measurment[i])
			max_measurment[i] = measurment[i];
			if (min_measurment[i] > measurment[i])
			min_measurment[i] = measurment[i];
			aver_measurment[i] = aver_measurment[i] + measurment[i];
			counter = counter + 1;
		}
		foreach(i, 6)
		aver_measurment[i] = aver_measurment[i] / counter;
	}
	Serial.println("DONE!");
}


short & MPU6050::getValues()
{
	return *measurment;
}

short & MPU6050::getMaxs()
{
	return *max_measurment;
}

short & MPU6050::getMins()
{
	return *min_measurment;
}

float & MPU6050::getAver()
{
	return *aver_measurment;
}

float MPU6050::getTemprature()
{
	return calc_temp(this->temprature);
}

void MPU6050::setSensorRanges(int accel_setting, int gyro_setting)
{
	//choosing accel & gyro ranges according the setting respectivly
	switch (gyro_setting)
	{
		case 0b00000000:
		gyro_range = 131;
		break;
		case 0b00001000:
		gyro_range = 65.5;
		break;
		case 0b00010000:
		gyro_range = 32.75;
		break;
		case 0b00011000:
		gyro_range = 16.375;
		break;
		default:
		gyro_range = 0;
	};
	switch (accel_setting)
	{
		case 0b00000000:
		accel_range = 16384;
		break;
		case 0b00001000:
		accel_range = 8192;
		break;
		case 0b00010000:
		accel_range = 4096;
		break;
		case 0b00011000:
		accel_range = 2048;
		break;
		default:
		accel_range = 0.0;
	}
}


void MPU6050::calculateGravAngle(){
	float root_xy = (float)sqrt((float)measurment[5] * measurment[5] + (float)measurment[3] * measurment[3]);
	
	if (root_xy) {
		grav_angle_xy = radtodeg((float)atan(measurment[4] / root_xy));
	}else
	root_xy = -1;
	
	//apply low pass filter
	if (measurment[2] >= b * max_measurment[2]) {
		if ( grav_angle_xy - pre_angle_xy >= 0) {
			grav_angle_xy = 0.85 * pre_angle_xy + 0.15 * grav_angle_xy ;
		}
	}
	if (measurment[2] <= b * min_measurment[2]) {
		if ( grav_angle_xy - pre_angle_xy <= 0) {
			grav_angle_xy = 0.85 * pre_angle_xy + 0.15 * grav_angle_xy ;
		}
	}
}


void MPU6050::calculateCurrentAngle(){
	unsigned long clock = micros();
	this->calculateGravAngle();
	this->circular_buffer->addElement(grav_angle_xy);
	
	float aver = circular_buffer->getMovAver();
	float dt = micros() - clock;
	if (measurment[2] >= b * max_measurment[2] || measurment[2] <= b * min_measurment[2]) {
		angle_estimate = angle_estimate  + ((measurment[2] - aver_measurment[2]) / gyro_range) * dt / 1000000;
	}
	angle_estimate = (1 - K) * angle_estimate + K * aver;
	
	pre_angle_xy = grav_angle_xy;
}



void MPU6050::calculateAngleBias(){
	int counter = 0;
	unsigned long t = millis();
	while((millis()-t) < 1000){
		this->readSensorValues();
		this->calculateCurrentAngle();
		counter++;
		angle_bias = angle_bias + angle_estimate;
	}
	angle_bias = angle_bias / counter;
}



float MPU6050::getCurrentAngle(){
	return (angle_estimate - angle_bias);
}



float MPU6050::getAngleBias(){
	return angle_bias;
}


void MPU6050::updateAngle(){
	this->readSensorValues();
	this->calculateCurrentAngle();
}