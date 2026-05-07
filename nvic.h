#ifndef INTERRUPT_H
#define INTERRUPT_H

//定义NVIC寄存器
struct NVIC
{
    volatile unsigned int  ISER[3];        //中断使能寄存器
    volatile unsigned int  RESERVED0[29];  //占位数组
    volatile unsigned int  ICER[3];        //中断除能寄存器
    volatile unsigned int  RESERVED1[29];  //占位数组
    volatile unsigned int  ISPR[3];        //中断悬起寄存器
    volatile unsigned int  RESERVED2[29];  //占位数组
    volatile unsigned int  CPR[3];         //中断解悬寄存器
    volatile unsigned int  RESERVED3[29];  //占位数组
    volatile unsigned int  IABR[3];        //中断活动状态寄存器
    volatile unsigned int  RESERVED4[61];  //占位数组
    volatile unsigned char IPR[84];        //中断优先级寄存器
    volatile unsigned int  RESERVED5[695]; //占位数组
    volatile unsigned char STIR;           //软件触发中断寄存器
}

//定义NVIC初地址
#define NVIC ((volatile struct NVIC*) 0xE000E100)

#endif