# MT6701_STM32_HAL

> #### 写在前面：
> 
> MT6701 是 MagnTek 推出的新一代基于差分霍尔感应原理的磁性角度编码器芯片。值得一提的是 MT6701不仅提供 0~360° 的角度信号，而且还提供了一个“按压”输出，以实现单芯片的按钮（旋转+按压）功能。

本文介绍使用 STM32 HAL 库驱动 MT6701

* IIC 方式驱动（ 此方法仅能读取角度信息 ） 

```c
#define SlaveAddress    0X0C        //MT6701 地址
#define ReadAddress1    0X03        //数据高位寄存器地址
#define ReadAddress2    0X04        //数据低位寄存器地址


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
```

* SSI 方式读取（此方式可以读取磁场信息）

```c
#define Loss_of_Track       4
#define Button_Detected     3
#define Field_Weak          2
#define Field_Strong        1
#define Normal              0


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
```

这里先介绍两种常用的读取方式，对于一些简单的应用应该是足够了。

“**按钮**” 输出使用十分便捷，芯片在检测到磁场变化后，  **PUSH** 引脚直接输出低电平，如果有更高精度要求可以查看手册对其编程。

详细程序放在这里：

https://github.com/BaldnessBaby/MT6701_STM32_HAL 
