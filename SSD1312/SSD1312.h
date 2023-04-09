#ifndef __TEST_H
#define __TEST_H 

#include "main.h"

#define ADDRESS_OLED 0x78
#define OLED_CMD     0
#define OLED_DATA    1

void iic_send_byte(uint8_t data);

void OLED_Init(void);
void OLED_Clear(void);
void OLED_WR_Byte(uint8_t dat,uint8_t mode);
void OLED_Refresh(void);
void OLED_ColorTurn(uint8_t i);
void OLED_DisPlay_Off(void);
void OLED_DisPlay_On(void);
void OLED_DisplayTurn(uint8_t i);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
uint32_t OLED_Pow(uint8_t m,uint8_t n);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);

#endif