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

//定义DMA通道
#define DMA_CH1 0
#define DMA_CH2 1
#define DMA_CH3 2
#define DMA_CH4 3
#define DMA_CH5 4
#define DMA_CH6 5
#define DMA_CH7 6

//定义通道优先级
#define DMA_LOW    0 //低
#define DMA_MEDIUM 1 //中
#define DMA_HIGH   2 //高
#define DMA_ULTRA  3 //最高

//定义外设数据宽度
#define PSIZE_8BIT  0 //8位
#define PSIZE_16BIT 1 //16位
#define PSIZE_32BIT 2 //32位

//定义循环模式开关
#define CIRC_OFF 0 //不执行循环操作
#define CIRE_ON  1 //执行循环操作

void enr_dma(void)
{
    SET_BIT((RCC->AHBENR),(1 << 0));
}

void init_dma(int channel_num,int priority_level,int psize,int circ)
{
    CLEAN_BIT((DMA->DMA_CHANNEL[channel_num]),(0x3 << 12));
    SET_BIT((DMA->DMA_CHANNEL[channel_num]),(priority_level << 12)); //设置通道优先级
    CLEAN_BIT((DMA->DMA_CHANNEL[channel_num]),(0x3 << 8));
    SET_BIT((DMA->DMA_CHANNEL[channel_num]),(psize << 8));           //设置外设数据宽度
    SET_BIT((DMA->DMA_CHANNEL[channel_num]),(cire << 5));            //设置循环模式
}

#endif