#ifndef FOCTEST_AS5600_H
#define FOCTEST_AS5600_H

#include "i2c.h"

#define AS5600_I2C_HANDLE hi2c1

#define I2C_TIME_OUT_BASE   10
#define I2C_TIME_OUT_BYTE   1

#define abs(x) ((x)>0?(x):-(x))
#define _2PI 6.28318530718

/*
注意：AS5600 的地址 0X36 是指原始7位设备地址，而 ST I2C 库中的设备地址是指原始设备地址左移一位得到的设备地址
*/

#define AS5600_RAW_ADDR    0x36
#define AS5600_ADDR        (AS5600_RAW_ADDR << 1)

#define AS5600_RESOLUTION 4096 //12bit Resolution

#define AS5600_RAW_ANGLE_REGISTER  0x0C

void bsp_as5600Init(void);
uint16_t bsp_as5600GetRawAngle(void);
float bsp_as5600GetAngle(void);






#endif //FOCTEST_AS5600_H
