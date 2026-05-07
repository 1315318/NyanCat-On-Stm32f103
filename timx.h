#ifndef TIMX_H
#define TIMX_H

//定义通用定时器寄存器
struct TIM
{
    volatile unsigned int CR1;
    volatile unsigned int CR2;
    volatile unsigned int SMCR;
    volatile unsigned int DIER;
    volatile unsigned int SR;
    volatile unsigned int EGR;
    volatile unsigned int CCMR1;
    volatile unsigned int CCMR2;
    volatile unsigned int CCER;
    volatile unsigned int CNT;
    volatile unsigned int PSC;
    volatile unsigned int ARR;
    volatile unsigned int RESERVED; //保留
    volatile unsigned int CCR1;
    volatile unsigned int CCR2;
    volatile unsigned int CCR3;
    volatile unsigned int CCR4;
    volatile unsigned int DCR;
    volatile unsigned int DMAR;
};

//定义通用定时寄存器初地址
#define TIM2        ((volatile struct TIM*) 0x40000000)
#define TIM3        ((volatile struct TIM*) 0x40000400)
#define TIM4        ((volatile struct TIM*) 0x40000800)

//定义TIM通道
#define TIM_CH1     ((unsigned char) 1)
#define TIM_CH2     ((unsigned char) 2)
#define TIM_CH3     ((unsigned char) 3)
#define TIM_CH4     ((unsigned char) 4)

//定义自动重装载预装载开关
#define ARPE_OFF    ((unsigned char) 0)
#define APRE_ON     ((unsigned char) 1)

//定义中央对齐模式
#define EDGE_ALI    ((unsigned char) 0)   //边沿对齐模式
#define CENTER_ALI1 ((unsigned char) 1)   //中央对齐模式1
#define CENTER_ALI2 ((unsigned char) 2)   //中央对齐模式2
#define CENTER_ALI3 ((unsigned char) 4)   //中央对齐模式3

//定义计数器计数方向
#define DIR_UP      ((unsigned char) 0)   //计数器向上计数
#define DIR_DOWN    ((unsigned char) 1)   //计数器向下计数

//定义单脉冲模式开关
#define OPM_OFF     ((unsigned char) 0)   //在发生更新事件时，计数器不停止
#define OPM_DOWN    ((unsigned char) 1)   //在发生下一次更新事件(清除CEN位)时，计数器停止

//定义更新请求源
#define URS_MULTI   ((unsigned char) 0)   //多个事件产生更新中断或DMA请求
#define URS_SINGL   ((unsigned char) 1)   //只有计数器溢出/下溢才产生更新中断或DMA请求

//定义触发DMA请求开关
#define TDE_OFF     ((unsigned char) 0)   //禁止触发DMA请求
#define TDE_ON      ((unsigned char) 1)   //允许触发DMA请求

//定义更新事件的产生开关
#define UG_OFF      ((unsigned char) 0)   //无动作
#define UG_ON       ((unsigned char) 1)   //重新初始化计数器，并产生一个更新事件

//定义输出比较模式
#define OCM_FRZE    ((unsigned char) 0)   //冻结模式
#define OCM_PWM1    ((unsigned char) 0x6) //PWM模式1
#define OCM_PWM2    ((unsigned char) 0x7) //PWM模式2

//定义输出比较预装载开关
#define OCPE_OFF    ((unsigned char) 0)   //禁止TIMx_CCR1寄存器的预装载功能，可随时写入TIMx_CCR1寄存器
#define OCPE_ON     ((unsigned char) 1)   //开启TIMx_CCR1寄存器的预装载功能，读写操作仅对预装载寄存器操作

//定义init_pwm配置结构体
struct INIT_CONFIG
{
    volatile struct TIM* tim_type;
    unsigned char channel_num;
    unsigned char apre;
    unsigned char ali_type;
    unsigned char dir;
    unsigned char opm;
    unsigned char urs;
    unsigned char uids;
    unsigned char tde;
    unsigned char ug;
    unsigned char ocm;
    unsigned char ocpe;
};

//RCC对应时钟使能
void enr_tim(volatile struct TIM* tim_type)
{
    if (tim_type == TIM2)
    {
        SET_BIT((RCC->APB1ENR), (1 << 0)); //TIM2使能
    }
    if (tim_type == TIM3)
    {
        SET_BIT((RCC->APB1ENR), (1 << 1)); //TIM3使能
    }
    if (tim_type == TIM4)
    {
        SET_BIT((RCC->APB1ENR), (1 << 2)); //TIM4使能
    }
}

//静态参数初始化
void init_tim(struct INIT_CONFIG* init_config)
{
    SET_BIT((init_config->tim_type->CR1), (init_config->ali_type << 5));                                          //设置中央对齐模式模式
    SET_BIT((init_config->tim_type->CR1), (init_config->dir << 4));                                               //设置计数器计数方向
    SET_BIT((init_config->tim_type->CR1), (init_config->uids << 1));                                              //设置是否允许UEV事件
    SET_BIT((init_config->tim_type->CR1), (init_config->opm) << 3);                                               //设置是否开启单脉冲模式
    SET_BIT((init_config->tim_type->CR1), (init_config->apre << 7));                                              //设置自动重装载预装载
    SET_BIT((init_config->tim_type->CR1), (init_config->urs << 2));                                               //设置更新源
    SET_BIT((init_config->tim_type->DIER), (init_config->tde << 8));                                              //设置是否更新的DMA请求
    if (init_config->channel_num == TIM_CH1 || init_config->channel_num == TIM_CH2)
    {
        CLEAN_BIT((init_config->tim_type->CCMR1), (0x7 << ((init_config->channel_num - 1) * 8 + 4)));
        SET_BIT((init_config->tim_type->CCMR1), (init_config->ocm << ((init_config->channel_num - 1) * 8 + 4)));  //设置输出模式
        SET_BIT((init_config->tim_type->CCMR1), (init_config->ocpe << ((init_config->channel_num - 1) * 8 + 3))); //设置TIMx_CCR1寄存器预装载功能
    }
    if (init_config->channel_num == TIM_CH3 || init_config->channel_num == TIM_CH4)
    {
        CLEAN_BIT((init_config->tim_type->CCMR2), (0x7 << ((init_config->channel_num - 3) * 8 + 4)));
        SET_BIT((init_config->tim_type->CCMR2), (init_config->ocm << ((init_config->channel_num - 3) * 8 + 4)));  //设置输出模式
        SET_BIT((init_config->tim_type->CCMR2), (init_config->ocpe << ((init_config->channel_num - 3) * 8 + 3))); //设置TIMx_CCR1寄存器预装载功能
    }
    SET_BIT((init_config->tim_type->EGR), (1 << 0));                                                              //产生更新事件
}

//动态参数初始化
void set_tim(volatile struct TIM* tim_type,unsigned char channel_num, unsigned char psc, unsigned int arr, unsigned char ccr, unsigned char mode)
{
    tim_type->PSC = psc;
    tim_type->ARR = arr;  
    switch(channel_num)
    {
        case 1: tim_type->CCR1 = ccr; break;
        case 2: tim_type->CCR2 = ccr; break;
        case 3: tim_type->CCR3 = ccr; break;
        case 4: tim_type->CCR4 = ccr; break;
    }
    if (mode == HIGH)
    {
        CLEAN_BIT((tim_type->CCER), (1 << ((channel_num - 1) * 4 + 1)));
    }
    if (mode == LOW)
    {
        SET_BIT((tim_type->CCER), (1 << ((channel_num - 1) * 4 + 1)));
    }
    SET_BIT((tim_type->CCER), (1 << (channel_num - 1) * 4)); //输出使能
    SET_BIT((tim_type->CR1), (1 << 0));                      //计数器使能    
    SET_BIT((tim_type->EGR), (1 << 0));                      //产生更新事件
}

//音频数据长度
#define AUDIO_SIZE 26702

//音频播放
void audio_play(void)
{
    enr_tim(TIM2);
    enr_tim(TIM3);
    struct INIT_CONFIG tim2_config = { 
        .tim_type    = TIM2,
        .channel_num = TIM_CH1,
        .apre        = APRE_ON,
        .ali_type    = EDGE_ALI,
        .dir         = DIR_UP,
        .opm         = OPM_OFF,
        .urs         = URS_SINGL,
        .tde         = TDE_OFF,
        .ug          = UG_ON,
        .ocm         = OCM_PWM1,
        .ocpe        = OCPE_ON,
    };
    struct INIT_CONFIG tim3_config = {
        .tim_type    = TIM3,
        .channel_num = TIM_CH1,
        .apre        = APRE_ON,
        .ali_type    = EDGE_ALI,
        .dir         = DIR_UP,
        .opm         = OPM_OFF,
        .urs         = URS_SINGL,
        .tde         = TDE_ON,
        .ug          = UG_ON,
        .ocm         = OCM_FRZE,
        .ocpe        = OCPE_OFF,
    };
    init_tim(&tim2_config);
    init_tim(&tim3_config);
    set_tim(TIM2, TIM_CH1, 0, 255, 0, HIGH);                                              //TIM2作为载波，时钟72MHZ
    set_tim(TIM3, TIM_CH1, 35, 249, 0, HIGH);                                             //TIM3触发DMA请求，时钟8KHZ
    enr_dma();
    init_dma(DMA_CH3, DMA_ULTRA, MSIZE_8BIT, PSIZE_16BIT, MINC_ON, CIRC_ON, DIR_MSI);
    set_dma(DMA_CH3, AUDIO_SIZE, (unsigned int)&TIM2->CCR1, (unsigned int)nyancat_audio); //通过DMA直接将音频数据传输到CRR1
    enr_gpio(GPIOA);
    init_gpio(GPIOA, GPIO_CH0, GPIO_MODE_AFPP);
}

#endif