#ifndef DMA_H
#define DMA_H

struct DMA_CHANNEL
{
    volatile unsigned int CCR;
    volatile unsigned int CNDTR;
    volatile unsigned int CPAR;
    volatile unsigned int CMAR;
};

struct DMA
{
    volatile unsigned int ISR;
    volatile unsigned int IFCR;
    sturct DMA_CHANNEL[7]; //DMA1有7个通道
};

//定义DMA初地址
#define DMA ((volatile struct DMA*)0x40020000) 

//定义DMA通道宏
#define DMA_CH1 0
#define DMA_CH2 1
#define DMA_CH3 2
#define DMA_CH4 3
#define DMA_CH5 4
#define DMA_CH6 5
#define DMA_CH7 6

void enr_dma(void)
{
    SET_BIT((RCC->AHBENR),(1 << 0));
}

#endif