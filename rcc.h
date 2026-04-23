#ifndef RCC_H
#define RCC_H

//定义RCC寄存器
struct RCC
{
    volatile unsigned int CR;
    volatile unsigned int CFGR;
    volatile unsigned int CIR;
    volatile unsigned int APB2RSTR;
    volatile unsigned int APB1RSTR;
    volatile unsigned int AHBENR;
    volatile unsigned int APB2ENR;
    volatile unsigned int APB1ENR;
    volatile unsigned int BDCR;
    volatile unsigned int CSR;
};

//定义RCC初地址
#define RCC ((volatile struct RCC*) 0x40021000)

//系统时钟初始化
void SystemInit(void) 
{
    SET_BIT((RCC->CR), (1 << 16));              //HSE使能
    while(READ_BIT((RCC->CR), (1 << 17)) == 0); //等待HSE就绪
    SET_BIT((RCC->CFGR), (1 << 16));            //设置HSE作为PLL输入时钟
    CLEAN_BIT((RCC->CFGR), (0xF << 18));
    SET_BIT((RCC->CFGR), (7 << 18));            //设置PLL倍频系数，当前值：9倍
    SET_BIT((RCC->CR), (1 << 24 ));             //PLL使能
    while(READ_BIT((RCC->CR), (1 << 25)) == 0); //等待PLL锁定
    SET_BIT((RCC->CR), (1 << 19));              //CSS使能
    CLEAN_BIT((Flash->ACR), (7));
    SET_BIT((Flash->ACR), (2));                 //设置Flash延时
    CLEAN_BIT((RCC->CFGR), (3));
    SET_BIT((RCC->CFGR), (2));                  //切换系统时钟为PLL倍频后的HSE
    while(((RCC->CFGR >> 2) & 3) != 2);         //确认系统时钟切换状态
    CLEAN_BIT((RCC->CR), (1 << 0));             //关闭HSI
}

#endif