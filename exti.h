#ifndef EXTI_H
#define EXTI_H

//定义EXTI寄存器
struct EXTI
{
    volatile unsigned int IMR;   //中断屏蔽寄存器
    volatile unsigned int EMR;   //事件屏蔽寄存器
    volatile unsigned int RTSR;  //上升沿触发选择寄存器
    volatile unsigned int FTSR;  //下降沿触发选择寄存器
    volatile unsigned int SWIER; //软件中断事件寄存器
    volatile unsigned int PR;    //挂起寄存器
}

//定义EXTI初地址
#define EXTI ((volatile struct EXIT*) 0x40010400)

#endif