#include "test.h"
#include "main.h"
#include "i2c.h"
#include "oledfont.h"

uint8_t OLED_GRAM[136][4];
uint8_t x_offset;

//发送一个字节
void iic_send_byte(uint8_t data)
{
	
	uint8_t Data[1] = {data};
	
	HAL_I2C_Master_Transmit(&hi2c2, ADDRESS_OLED, (uint8_t*)Data, 1, 500);
}


/* OLED 相关 */

//反显函数
void OLED_ColorTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
		}
}

//屏幕旋转180度
void OLED_DisplayTurn(uint8_t i)
{
	if(i==0)
		{
			x_offset=8;
		  OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	    OLED_WR_Byte(0x12,OLED_CMD); 
			OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
	if(i==1)
		{
			x_offset=0;
			OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	    OLED_WR_Byte(0x2e,OLED_CMD); 
			OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
}

//发送一个字节
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(uint8_t dat,uint8_t mode)
{
	uint8_t Data[2];
	
	if(mode)
		Data[0] = 0x40;
	else
		Data[0] = 0x00;
	
	Data[1] = dat;
	
	HAL_I2C_Master_Transmit(&hi2c2, ADDRESS_OLED, (uint8_t*)Data, 2, 500);
}


//清屏函数
void OLED_Clear(void)
{
	uint8_t i,n;
	
	for(i=0;i<4;i++)
	   for(n=0;n<136;n++)
			 OLED_GRAM[n][i]=0; //清除所有数据
	
	OLED_Refresh();//更新显示
}


//更新显存到OLED	
void OLED_Refresh(void)
{
	uint8_t i,n;
	uint8_t Data[1] = {0x40};
	
	for(i=0;i<4;i++)
	{
		OLED_WR_Byte(0xb0+i, OLED_CMD); //设置行起始地址
		OLED_WR_Byte(0x00+x_offset, OLED_CMD);   //设置低列起始地址
		OLED_WR_Byte(0x10, OLED_CMD);   //设置高列起始地址
		
		for(n=0;n<136;n++)
			OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
  }
}


void OLED_Init(void)
{
	HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_SET);
	
	OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_WR_Byte(0x1b,OLED_CMD); /*duty = 1/28*/ 
	OLED_WR_Byte(0xad,OLED_CMD); /*External or Internal IREF Selection */ 
	OLED_WR_Byte(0x40,OLED_CMD);
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_WR_Byte(0x12,OLED_CMD); 
	OLED_WR_Byte(0xA0,OLED_CMD); /*set segment remap*/ 
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/ 
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/ 
	OLED_WR_Byte(0x40,OLED_CMD); /*set display start line*/ 
	OLED_WR_Byte(0xA4,OLED_CMD); 
	OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/ 
	OLED_WR_Byte(0x30,OLED_CMD); /*128*/ 
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_WR_Byte(0x22,OLED_CMD); 
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_WR_Byte(0x41,OLED_CMD); 
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_WR_Byte(0x10,OLED_CMD); 
	OLED_WR_Byte(0x20,OLED_CMD); /*Set Memory Addressing Mode */ 
	OLED_WR_Byte(0x02,OLED_CMD); /*Page Addressing Mode*/ 
	OLED_WR_Byte(0xdb,OLED_CMD); /*set vcomh*/ 
	OLED_WR_Byte(0x30,OLED_CMD); 
	OLED_WR_Byte(0x8d,OLED_CMD); /*set charge pump enable*/ 
	OLED_WR_Byte(0x52,OLED_CMD); 
	OLED_WR_Byte(0x08,OLED_CMD); /*set lower column address*/ 
	OLED_WR_Byte(0x10,OLED_CMD); /*set higher column address*/
	OLED_Clear();
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
}

//开启OLED显示 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
	OLED_WR_Byte(0xAF,OLED_CMD);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
	OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
	OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}


//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空	
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12
//mode:0,反色显示;1,正常显示
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode)
{
	uint8_t i,m,temp,size2,chr1;
	uint8_t x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //调用0806字体
//		else if(size1==12)
//        {temp=asc2_1206[chr1][i];} //调用1206字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
//mode:0,反色显示;1,正常显示
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}

uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode)
{
	uint8_t t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}