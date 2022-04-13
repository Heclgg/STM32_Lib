# ReadME

此驱动基于 STM32 HAL 库

STM32F4 系列单片机可直接使用，仅需更改引脚即可

其他系列单片机需要更改 DHT22.h 中如下宏：

```c
#define GPIO_IN  do{GPIOB->MODER &= 0XFFFFCFFF;}while(0);
#define GPIO_OUT do{GPIOB->MODER &= 0XFFFFDFFF;}while(0);
```

> 根据单片机数据手册，查看 GPIO 相关寄存器，更改引脚输入输出模式



主程序实例：

```c
int main(void)
{
    float temp,humi;
    while(1)
    {
        if(!DHT_Read_Data(&temp,&humi))
          UART_printf(&huart1,"T:%.2f H:%.2f \r\n",temp,humi);
    }
}
```

* 注意事项：
  
  > 单片机引脚需要设置为开漏模式 (OD)
  > 
  > 引脚需要上拉
