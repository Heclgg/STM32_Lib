#include "HCl.h"

static uint16_t fac_us = 0;

/* sysclk 为时钟频率，单位为MHz nus 为延时时间，最大为 2^32/fac_us */
void Delay_us(uint16_t sysclk, uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload;

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
    fac_us = sysclk / 8;

    reload = SysTick->LOAD;     /* LOAD的值 */
    ticks = nus * fac_us;     /* 需要的节拍数 */
    told = SysTick->VAL;        /* 刚进入时的计数器值 */

    while (1)
    {
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;    /* 这里注意一下SYSTICK是一个递减的计数器就可以了. */
            }
            else
            {
                tcnt += reload - tnow + told;
            }

            told = tnow;

            if (tcnt >= ticks) break;   /* 时间超过/等于要延迟的时间,则退出. */
        }
    }
}

/* 串口打印重定向 */
int UART_printf(UART_HandleTypeDef *huart, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int length;
    char buffer[128];
    length = vsnprintf(buffer, 128, fmt, ap);
    HAL_UART_Transmit(huart, (uint8_t *)buffer, length, HAL_MAX_DELAY);
    va_end(ap);
    return length;
}