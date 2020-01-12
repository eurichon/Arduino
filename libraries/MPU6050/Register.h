/*
 * Register.h
 *
 * Created: 22/6/2019 6:12:56 μμ
 *  Author: evrichon
 */ 


#ifndef REGISTER_H_
#define REGISTER_H_

#define radtodeg(x) (x*57.23)
#define degtorad(x) (x*0.01745)
#define calc_temp(x) (x/340.00+36.53)
#define foreach(i,n) for(int i = 0; i < n;++i)

//MPU6050 ADDRESSES
#define MPU_addr 0x68

#define MPU_FS_SEL_0  0b00000000 //set gyro range to +/- 250 deg/s
#define MPU_FS_SEL_1  0b00001000 //set gyro range to +/- 500 deg/s
#define MPU_FS_SEL_2  0b00010000 //set gyro range to +/- 1000 deg/s
#define MPU_FS_SEL_3  0b00011000 //set gyro range to +/- 2000 deg/s

#define MPU_AFS_SEL_0  0b00000000 // set accel range to +/- 2g
#define MPU_AFS_SEL_1  0b00001000 // set accel range to +/- 4g
#define MPU_AFS_SEL_2  0b00010000 // set accel range to +/- 8g
#define MPU_AFS_SEL_3  0b00011000 // set accel range to +/- 16g

#define MPU6050_GYRO_XOUT_H        0x43   // R
#define MPU6050_GYRO_XOUT_L        0x44   // R
#define MPU6050_GYRO_YOUT_H        0x45   // R
#define MPU6050_GYRO_YOUT_L        0x46   // R
#define MPU6050_GYRO_ZOUT_H        0x47   // R
#define MPU6050_GYRO_ZOUT_L        0x48   // R

#define MPU6050_ACCEL_XOUT_H       0x3B   // R
#define MPU6050_ACCEL_XOUT_L       0x3C   // R
#define MPU6050_ACCEL_YOUT_H       0x3D   // R
#define MPU6050_ACCEL_YOUT_L       0x3E   // R
#define MPU6050_ACCEL_ZOUT_H       0x3F   // R
#define MPU6050_ACCEL_ZOUT_L       0x40   // R



#endif /* REGISTER_H_ */