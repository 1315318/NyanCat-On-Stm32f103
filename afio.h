#ifndef AFIO_H
#define AFIO_H

//定义AFIO寄存器
struct AFIO
{
    volatile unsigned int EVCR;    //事件控制寄存器
    volatile unsigned int MAPR;    //复用重映射和调试I/O配置寄存器
    volatile unsigned int EXTICR1; //外部中断配置寄存器1
    volatile unsigned int EXTICR2; //外部中断配置寄存器2
    volatile unsigned int EXTICR3; //外部中断配置寄存器3
    volatile unsigned int EXTICR4; //外部中断配置寄存器4
}

//定义AFIO初地址
#define AFIO ((volatile struct AFIO*) 0x40010000) 

#endif