#ifndef DHT22_DHT22_H
#define DHT22_DHT22_H

#include "main.h"

#define DHT22_Pin GPIO_PIN_12
#define GPIO_IN  do{GPIOB->MODER &= 0XFFFFCFFF;}while(0);
#define GPIO_OUT do{GPIOB->MODER &= 0XFFFFDFFF;}while(0);
#define DHT_DQ_IN HAL_GPIO_ReadPin(DHT22_GPIO_Port,DHT22_Pin)

void Delay_us(uint32_t nus);
void Delay_ms(uint16_t nms);
void DHT22_Rst(void);
uint8_t DHT_Check(void);
uint8_t DHT_Read_Bit(void);
uint8_t DHT_Read_Byte(void);
uint8_t DHT_Read_Data(float *temp,float *humi);

#endif //DHT22_DHT22_H