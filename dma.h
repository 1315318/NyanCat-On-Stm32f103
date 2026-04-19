#ifndef DMA_H
#define DMA_H

struct DMA_CHANNEL
{
    volatile unsigned int CCR;
    volatile unsigned int CNDTR;
    volatile unsigned int CPAR;
    volatile unsigned int CMAR;
    volatile unsigned int RESERVED; //保留
};

struct DMA
{
    volatile unsigned int ISR;
    volatile unsigned int IFCR;
    struct   DMA_CHANNEL  dma_channel[7]; //DMA1有7个通道
};

//定义DMA初地址
#define DMA ((volatile struct DMA*) 0x40020000) 

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

//定义存储器数据宽度
#define MSIZE_8BIT  0 //8位
#define MSIZE_16BIT 1 //16位
#define MSIZE_32BIT 2 //32位

//定义外设数据宽度
#define PSIZE_8BIT  0 //8位
#define PSIZE_16BIT 1 //16位
#define PSIZE_32BIT 2 //32位

//定义存储器地址增量模式开关
#define MINC_OFF 0 //不执行存储器地址增量操作
#define MINC_ON  1 //执行存储器地址增量操作

//定义循环模式开关
#define CIRC_OFF 0 //不执行循环操作
#define CIRC_ON  1 //执行循环操作

//定义数据传输方向
#define DIR_PSI 0 //从外设读
#define DIR_MSI 1 //从存储器读

void enr_dma(void)
{
    SET_BIT((RCC->AHBENR), (1 << 0));
}

void init_dma(int channel_num, int priority_level, int msize, int psize, int minc, int circ, int dir)
{
    CLEAN_BIT((DMA->dma_channel[channel_num].CCR), (0x3 << 12));
    SET_BIT((DMA->dma_channel[channel_num].CCR), (priority_level << 12)); //设置通道优先级
    CLEAN_BIT((DMA->dma_channel[channel_num].CCR), (0x3 << 10)); 
    SET_BIT((DMA->dma_channel[channel_num].CCR), (msize << 10));          //设置存储器数据宽度
    CLEAN_BIT((DMA->dma_channel[channel_num].CCR), (0x3 << 8));
    SET_BIT((DMA->dma_channel[channel_num].CCR), (psize << 8));           //设置外设数据宽度
    SET_BIT((DMA->dma_channel[channel_num].CCR), (minc << 7));            //设置存储器地址增量模式
    SET_BIT((DMA->dma_channel[channel_num].CCR), (dir << 4));             //设置数据传输方向
    SET_BIT((DMA->dma_channel[channel_num].CCR), (circ << 5));            //设置循环模式
}

//根据宏定义，传入的target_address是一个结构体指针，传参时需要做强制类型转换(unsigned int)
void set_dma(int channel_num, int data_size, unsigned int target_address, unsigned int data_address) 
{
    DMA->dma_channel[channel_num].CNDTR = data_size;        //数据传输数量，范围为0～65535
    DMA->dma_channel[channel_num].CPAR  = target_address;   //外设数据寄存器的基地址，作为数据传输的源或目标 
    DMA->dma_channel[channel_num].CMAR  = data_address;     //存储器地址，作为数据传输的源或目标
    SET_BIT((DMA->dma_channel[channel_num].CCR), (1 << 0)); //通道使能
}

#endif