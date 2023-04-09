//
// Created by HCl on 2022/5/11.
//

#include "MT6701.h"
#include "i2c.h"
#include "usart.h"

/*
 * IIC 方式读取角度信息
 * 返回数据为 0 ~ 360 之间的浮点数
 * STM32 开启 IIC 高速模式
 */
double ReadAngleIIC(void)
{
    uint32_t angle = 0;
    double fangle = 0;
    uint8_t ReadBuffer1,ReadBuffer2;

    HAL_I2C_Mem_Read(&hi2c1,SlaveAddress,ReadAddress1,I2C_MEMADD_SIZE_8BIT,&ReadBuffer1,1,0XFF);
    angle = ReadBuffer1;
    angle <<= 8;
    HAL_I2C_Mem_Read(&hi2c1,SlaveAddress,ReadAddress2,I2C_MEMADD_SIZE_8BIT,&ReadBuffer2,1,0XFF);
    angle += ReadBuffer2;
    angle >>= 2;            //取数据高 14 位
    fangle = (double)(angle * 360.0) / 16384.0;

    return fangle;
}

/*
 * SSI 方式读取信息
 * mode == 0 返回角度信息，0~360 浮点数
 * mode == 1 返回磁场信息
 */
double ReadDataSSI(uint8_t mode)
{
    uint8_t RawData[25] = {0};
    uint16_t angle = 0;
    double fangle = 0;

    HAL_GPIO_WritePin(SSI_CSN_GPIO_Port,SSI_CSN_Pin,GPIO_PIN_RESET);        //起始信号
    Delay_us(2);

    HAL_GPIO_WritePin(SSI_CLK_GPIO_Port,SSI_CLK_Pin,GPIO_PIN_RESET);        //跳过第一个时钟沿
    Delay_us(2);
    HAL_GPIO_WritePin(SSI_CLK_GPIO_Port,SSI_CLK_Pin,GPIO_PIN_SET);
    Delay_us(2);

    for(uint8_t i = 0; i < 25; i++)                                                                 //读取数据
    {
        HAL_GPIO_WritePin(SSI_CLK_GPIO_Port,SSI_CLK_Pin,GPIO_PIN_RESET);
        RawData[i] = HAL_GPIO_ReadPin(SSI_DI_GPIO_Port,SSI_DI_Pin);
        HAL_GPIO_WritePin(SSI_CLK_GPIO_Port,SSI_CLK_Pin,GPIO_PIN_SET);
        Delay_us(2);
    }
    HAL_GPIO_WritePin(SSI_CSN_GPIO_Port,SSI_CSN_Pin,GPIO_PIN_SET);          //结束信号

    for(uint8_t i = 0; i < 14; i++)
        angle += RawData[i] * (2 << (13 - i));
    fangle = (double)(angle * 180.0) / 16384.0;     //角度信息

    switch (mode)
    {
        case 0:
            return fangle;
        case 1:
        {
            if(RawData[14])
                return Loss_of_Track;
            else if(RawData[15])
                return Button_Detected;
            else if(RawData[16])
                return Field_Weak;
            else if(RawData[17])
                return Field_Strong;
            else
                return Normal;
        }
        default:
            return 0;
    }
}

void Delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * 84;       /* 时间加载-时钟频率 */
    SysTick->VAL = 0x00;            /* 清空计数器 */
    SysTick->CTRL |= 1 << 0 ;       /* 开始倒数 */

    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16)));     /* CTRL.ENABLE位必须为1, 并等待时间到达 */

    SysTick->CTRL &= ~(1 << 0) ;    /* 关闭SYSTICK */
    SysTick->VAL = 0X00;            /* 清空计数器 */
}

void Delay_ms(uint16_t nms)
{
    uint32_t repeat = nms / 1000;   /*  这里用1000,是考虑到可能有超频应用,
                                     *  比如128Mhz的时候, delay_us最大只能延时1048576us左右了
                                     */
    uint32_t remain = nms % 1000;

    while (repeat)
    {
        Delay_us(1000 * 1000);      /* 利用delay_us 实现 1000ms 延时 */
        repeat--;
    }

    if (remain)
    {
        Delay_us(remain * 1000);    /* 利用delay_us, 把尾数延时(remain ms)给做了 */
    }
}

void HAL_Delay(uint32_t Delay)
{
    Delay_ms(Delay);
}