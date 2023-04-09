//
// Created by HCl on 2022/5/11.
//

#include "main.h"

#ifndef MT6701_MT6701_H
#define MT6701_MT6701_H

#define SlaveAddress    0X0C        //MT6701 地址
#define ReadAddress1    0X03        //数据高位寄存器地址
#define ReadAddress2    0X04        //数据低位寄存器地址

#define Loss_of_Track       4
#define Button_Detected     3
#define Field_Weak          2
#define Field_Strong        1
#define Normal              0

double ReadAngleIIC(void);
double ReadDataSSI(uint8_t mode);
void Delay_us(uint32_t nus);
void Delay_ms(uint16_t nms);

#endif //MT6701_MT6701_H
