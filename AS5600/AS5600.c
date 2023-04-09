#include "AS5600.h"

static float angle_data_prev; //上次位置
/*!
 * 初始化 AS5600 并获得初始角度，校准未来角度
 */

void bsp_as5600Init(void)
{
    /* init i2c interface */

    /* init var */
    angle_data_prev = bsp_as5600GetRawAngle();
}

/*!
 * AS5600 初始数据
 */
uint16_t bsp_as5600GetRawAngle(void)
{
    uint16_t raw_angle;
    uint8_t buffer[2] = {0};

    HAL_I2C_Mem_Read(&hi2c1,AS5600_ADDR,AS5600_RAW_ANGLE_REGISTER, I2C_MEMADD_SIZE_8BIT,buffer,2,100);
    raw_angle = (buffer[0] &0b00001111)<< 8 | buffer[1];
    return raw_angle;
}

/*!
 * 角度
 */
float bsp_as5600GetAngle(void)
{
    float angle_data = bsp_as5600GetRawAngle();
    float d_angle=angle_data - angle_data_prev;
    float angle=(d_angle*360/AS5600_RESOLUTION);
    if(angle<0){angle+=360;}
    return angle;
}
