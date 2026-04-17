#ifndef TIMX_H
#define TIMX_H

//定义通用定时器寄存器
struct TIMx
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
    volatile unsigned int CCR1;
    volatile unsigned int CCR2;
    volatile unsigned int CCR3;
    volatile unsigned int CCR4;
    volatile unsigned int DCR;
    volatile unsigned int DMAR;
};

//定义通用定时寄存器初地址
#define TIM2 ((volatile struct TIMx*)0x40000000)
#define TIM3 ((volatile struct TIMx*)0x40000400)
#define TIM4 ((volatile struct TIMx*)0x40000800)

//定义TIMx通道宏
#define CH1 1
#define CH2 2
#define CH3 3
#define CH4 4

void init_pwm(volatile struct TIMx* tim_type,int channel_num)
{
    if (tim_type == TIM2)
    {
        SET_BIT((RCC->APB1ENR),(1));     //TIM2使能
        TIM2->PSC = 71;                  //设置预分频器的值，当前频率：1MHZ
        CLEAN_BIT((TIM2->CR1),(3 << 5)); //设置边沿对齐模式
        CLEAN_BIT((TIM2->CR1),(1 << 4)); //设置计数器向上计数
        CLEAN_BIT((TIM2->CR1),(1 << 1)); //允许UEV事件
        SET_BIT((TIM2->CR1),(1 << 2));   //设置更新源为计数器溢出
        SET_BIT((TIM2->CR1),(1 << 7));   //开启自动重装载预装载
        SET_BIT((TIM2->EGR),(1));        //产生更新事件
        SET_BIT((TIM2->CR1),(1));        //使能计数器
        if (channel_num == CH1)
        { 
            CLEAN_BIT((TIM2->CCMR1),(7 << 4));
            SET_BIT((TIM2->CCMR1),(6 << 4));   //设置PWM模式1
            SET_BIT((TIM2->CCMR1),(1 << 3));   //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOA,0,GPIO_MODE_AFPP); //设置PA0引脚为复用推挽输出模式
        }
        if (channel_num == CH2)
        {
            CLEAN_BIT((TIM2->CCMR1),(7 << 12));
            SET_BIT((TIM2->CCMR1),(6 << 12));  //设置PWM模式1
            SET_BIT((TIM2->CCMR1),(1 << 11));  //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOA,1,GPIO_MODE_AFPP); //设置PA1引脚为复用推挽输出模式
        }
        if (channel_num == CH3)
        {
            CLEAN_BIT((TIM2->CCMR2),(7 << 4));
            SET_BIT((TIM2->CCMR2),(6 << 4));   //设置PWM模式1
            SET_BIT((TIM2->CCMR2),(1 << 3));   //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOA,2,GPIO_MODE_AFPP); //设置PA2引脚为复用推挽输出模式
        }
        if (channel_num == CH4)
        {
            CLEAN_BIT((TIM2->CCMR2),(7 << 12));
            SET_BIT((TIM2->CCMR2),(6 << 12));  //设置PWM模式1
            SET_BIT((TIM2->CCMR2),(1 << 11));  //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOA,3,GPIO_MODE_AFPP); //设置PA3引脚为复用推挽输出模式
        }   
    }
    if (tim_type == TIM3)
    {
        SET_BIT((RCC->APB1ENR),(1 << 1)); //TIM3使能
        TIM3->PSC = 71;                   //设置预分频器的值，当前频率：1MHZ
        CLEAN_BIT((TIM3->CR1),(3 << 5));  //设置边沿对齐模式
        CLEAN_BIT((TIM3->CR1),(1 << 4));  //设置计数器向上计数
        CLEAN_BIT((TIM3->CR1),(1 << 1));  //允许UEV事件
        SET_BIT((TIM3->CR1),(1 << 2));    //设置更新源为计数器溢出
        SET_BIT((TIM3->CR1),(1 << 7));    //开启自动重装载预装载
        SET_BIT((TIM3->EGR),(1));         //产生更新事件
        SET_BIT((TIM3->CR1),(1));         //使能计数器
        if (channel_num == CH1)
        { 
            CLEAN_BIT((TIM3->CCMR1),(7 << 4));
            SET_BIT((TIM3->CCMR1),(6 << 4));   //设置PWM模式1
            SET_BIT((TIM3->CCMR1),(1 << 3));   //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOA,6,GPIO_MODE_AFPP); //设置PA6引脚为复用推挽输出模式
        }
        if (channel_num == CH2)
        {
            CLEAN_BIT((TIM3->CCMR1),(7 << 12));
            SET_BIT((TIM3->CCMR1),(6 << 12));  //设置PWM模式1
            SET_BIT((TIM3->CCMR1),(1 << 11));  //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOA,7,GPIO_MODE_AFPP); //设置PA7引脚为复用推挽输出模式
        }
        if (channel_num == CH3)
        {
            CLEAN_BIT((TIM3->CCMR2),(7 << 4));
            SET_BIT((TIM3->CCMR2),(6 << 4));   //设置PWM模式1
            SET_BIT((TIM3->CCMR2),(1 << 3));   //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOB,0,GPIO_MODE_AFPP); //设置PB0引脚为复用推挽输出模式
        }
        if (channel_num == CH4)
        {
            CLEAN_BIT((TIM3->CCMR2),(7 << 12));
            SET_BIT((TIM3->CCMR2),(6 << 12));  //设置PWM模式1
            SET_BIT((TIM3->CCMR2),(1 << 11));  //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOB,1,GPIO_MODE_AFPP); //设置PB1引脚为复用推挽输出模式
        }   
    }
    if (tim_type == TIM4)
    {
        SET_BIT((RCC->APB1ENR),(1 << 2)); //TIM4使能
        TIM4->PSC = 71;                   //设置预分频器的值，当前频率：1MHZ
        CLEAN_BIT((TIM4->CR1),(3 << 5));  //设置边沿对齐模式
        CLEAN_BIT((TIM4->CR1),(1 << 4));  //设置计数器向上计数
        CLEAN_BIT((TIM4->CR1),(1 << 1));  //允许UEV事件
        SET_BIT((TIM4->CR1),(1 << 2));    //设置更新源为计数器溢出
        SET_BIT((TIM4->CR1),(1 << 7));    //开启自动重装载预装载
        SET_BIT((TIM4->EGR),(1));         //产生更新事件
        SET_BIT((TIM4->CR1),(1));         //使能计数器
        if (channel_num == CH1)
        { 
            CLEAN_BIT((TIM4->CCMR1),(7 << 4));
            SET_BIT((TIM4->CCMR1),(6 << 4));   //设置PWM模式1
            SET_BIT((TIM4->CCMR1),(1 << 3));   //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOB,6,GPIO_MODE_AFPP); //设置PB6引脚为复用推挽输出模式
        }
        if (channel_num == CH2)
        {
            CLEAN_BIT((TIM4->CCMR1),(7 << 12));
            SET_BIT((TIM4->CCMR1),(6 << 12));  //设置PWM模式1
            SET_BIT((TIM4->CCMR1),(1 << 11));  //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOB,7,GPIO_MODE_AFPP); //设置PB7引脚为复用推挽输出模式
        }
        if (channel_num == CH3)
        {
            CLEAN_BIT((TIM4->CCMR2),(7 << 4));
            SET_BIT((TIM4->CCMR2),(6 << 4));   //设置PWM模式1
            SET_BIT((TIM4->CCMR2),(1 << 3));   //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOB,8,GPIO_MODE_AFPP); //设置PB8引脚为复用推挽输出模式
        }
        if (channel_num == CH4)
        {
            CLEAN_BIT((TIM4->CCMR2),(7 << 12));
            SET_BIT((TIM4->CCMR2),(6 << 12));  //设置PWM模式1
            SET_BIT((TIM4->CCMR2),(1 << 11));  //开启TIMx_CCR1寄存器预装载功能
            init_gpio(GPIOB,9,GPIO_MODE_AFPP); //设置PB9引脚为复用推挽输出模式
        }   
    }
}

void set_pwm(volatile struct TIMx* tim_type,int channel_num,int mode,int frequency,int duty_cycle)
{
    if (frequency == 0)
    {
        return;
    }
    int arr_num = 1000000 / frequency;
    int ccr_num = duty_cycle * arr_num / 100;
    tim_type->ARR = arr_num - 1;             //写入输出频率值，时钟频率1MHZ
    if (channel_num == CH1)
    { 
        tim_type->CCR1 = ccr_num;
        if (mode == HIGH)
        {
            CLEAN_BIT((tim_type->CCER),(1 << 1));
        }
        if (mode == LOW)
        {
            SET_BIT((tim_type->CCER),(1 << 1));
        }
        SET_BIT((tim_type->CCER),(1));       //输出使能
    }
    if (channel_num == CH2)
    {
        tim_type->CCR2 = ccr_num;
        if (mode == HIGH)
        {
            CLEAN_BIT((tim_type->CCER),(1 << 5));
        }
        if (mode == LOW)
        {
            SET_BIT((tim_type->CCER),(1 << 5));
        }
        SET_BIT((tim_type->CCER),(1 << 4));  //输出使能
    }
    if (channel_num == CH3)
    {
        tim_type->CCR3 = ccr_num;
        if (mode == HIGH)
        {
            CLEAN_BIT((tim_type->CCER),(1 << 9));
        }
        if (mode == LOW)
        {
            SET_BIT((tim_type->CCER),(1 << 9));
        }
        SET_BIT((tim_type->CCER),(1 << 8));  //输出使能
    }
    if (channel_num == CH4)
    {
        tim_type->CCR4 = ccr_num;
        if (mode == HIGH)
        {
            CLEAN_BIT((tim_type->CCER),(1 << 13));
        }
        if (mode == LOW)
        {
            SET_BIT((tim_type->CCER),(1 << 13));
        }
        SET_BIT((tim_type->CCER),(1 << 12)); //输出使能
    }
    SET_BIT((tim_type->EGR),(1));            //产生更新事件
}

#endif