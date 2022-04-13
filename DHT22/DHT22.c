#include "DHT22.h"

void Delay_us(uint32_t nus)
{
    uint32_t temp;
    SysTick->LOAD = nus * 84;      /* 时间加载-时钟频率 */
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

void DHT22_Rst(void)
{
    GPIO_OUT
    HAL_GPIO_WritePin(DHT22_GPIO_Port,DHT22_Pin,GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(DHT22_GPIO_Port,DHT22_Pin,GPIO_PIN_SET);
    Delay_us(30);
    GPIO_IN
}

uint8_t DHT_Check(void)
{
    uint8_t retry = 0;
    if(DHT_DQ_IN == 0)
    {
        while((DHT_DQ_IN) && (retry <= 85))        //返回低电平
        {
            retry++;
            Delay_us(1);
        }
        if(retry > 85)
            return 1;       //Error
        else
            retry = 0;

        while((!DHT_DQ_IN) && (retry <= 85))         //返回高电平
        {
            retry++;
            Delay_us(1);
        }
        if(retry > 85)
            return 1;   //Error
        else
            return 0;   //Succeed
    }
    else
        return 1;       //Error
}

uint8_t DHT_Read_Bit(void)
{
    GPIO_IN
    uint8_t dat;
    uint8_t retry=100;
    while((!DHT_DQ_IN) && retry)      //等待变为高电平
    {
        retry--;
        Delay_us(1);
    }
    retry=100;
    dat=0;
    Delay_us(40);
    if(DHT_DQ_IN)
        dat=1;
    while(DHT_DQ_IN && retry)     //等待变低电平
    {
        retry--;
        Delay_us(1);
    }
    return dat;
}

uint8_t DHT_Read_Byte(void)
{
    uint8_t i,dat;
    dat=0;
    for (i=0;i<8;i++)
    {
        dat<<=1;
        dat |= DHT_Read_Bit();
    }
    return dat;
}

/* 读取数据，返回“0”正常，返回“1”错误 */
uint8_t DHT_Read_Data(float *temp,float *humi)
{
    uint8_t buf[5] = {0};
    uint8_t i;
    DHT22_Rst();
    if(!DHT_Check())
    {
        while(DHT_DQ_IN);
        for(i=0;i<5;i++)        //读取40位数据
            buf[i]=DHT_Read_Byte();

        //校验和累加时存在相当概率会超出8位(即值大于1个char,故做强制类型转换，只考虑低8位)
        if((unsigned char)(buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
        {
            *humi = (float)((256 * buf[0] + buf[1]) / 10.0);
            *temp = (float)((256 * buf[2] + buf[3]) / 10.0);
            return 0;
        }
    }
    return 1;       //Error
}