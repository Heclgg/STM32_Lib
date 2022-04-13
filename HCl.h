#ifndef INC_2021_TEST_HCL_H
#define INC_2021_TEST_HCL_H

#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"

void Delay_us(uint16_t sysclk, uint32_t nus);
int UART_printf(UART_HandleTypeDef *huart, const char *fmt, ...);

#endif