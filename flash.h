#ifndef FLASH_H
#define FLASH_H

//定义Flash寄存器
struct Flash
{
    volatile unsigned int ACR;
    volatile unsigned int KEYR;
    volatile unsigned int OPTKEYR;
    volatile unsigned int SR;
    volatile unsigned int CR;
    volatile unsigned int AR;
    volatile unsigned int RESERVED; //保留
    volatile unsigned int OBR;
    volatile unsigned int WRPR;
};

//定义Flash初地址
#define Flash ((volatile struct Flash*)0x40022000)

#endif