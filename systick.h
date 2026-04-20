#ifndef SYSTICK_H
#define SYSTICK_H

//定义SysTick寄存器
struct SysTick
{
    volatile unsigned int CAS;  //控制及状态寄存器
    volatile unsigned int RV;   //重装载数值寄存器
    volatile unsigned int CV;   //当前数值寄存器
    volatile unsigned int CALV; //校准数值寄存器
};

//定义SysTick初地址
#define SysTick ((volatile struct SysTick*) 0xE000E010)

#define SysTick_CLK ((unsigned char) 9) //当前SysTick时钟：9MHZ

void systick_timing(unsigned int ticks) //读CAS寄存器第16位判断倒计时是否结束
{
    SysTick->CAS = 0;            //CAS寄存器所有位置零
    SysTick->RV  = 0;            //重载数值寄存器所有位置零
    SysTick->RV  = ticks;        //写入倒计时值
    SET_BIT((SysTick->CAS), (1)); //开启计时器
}

void delay_us(unsigned int delay_time)
{
    systick_timing(delay_time * SysTick_CLK);
    while(READ_BIT((SysTick->CAS), (1 << 16)) == 0);
}

void delay_ms(unsigned int delay_time)
{
    while(delay_time--)
    {
        systick_timing(1000 * SysTick_CLK);
        while(READ_BIT((SysTick->CAS), (1 << 16)) == 0);
    }
}

#endif