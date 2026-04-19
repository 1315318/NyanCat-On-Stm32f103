#ifndef DMA_H
#define DMA_H

struct DMA
{
    volatile unsigned int ISR;
    volatile unsigned int IFCR;
    volatile unsigned int CCRx;
    volatile unsigned int CNDTRx;
    volatile unsigned int CPARx;
    volatile unsigned int CMARx;
};

//定义DMA初地址
#define DMA ((volatile struct DMA*)0x40020000) 

void enr_dma(void)
{
    SET_BIT((RCC->AHBENR),(1 << 0));
}

#endif