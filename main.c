/*PA13 (JTMS / SWDIO)：SWD 模式的数据线
PA14 (JTCK / SWCLK)：SWD 模式的时钟线
PA15 (JTDI)：JTAG 专用
PB3 (JTDO)：JTAG 专用
PB4 (NJTRST)：JTAG 专用*/

#include "nyancat_video.h" //图像数组
#include "nyancat_audio.h" //音频数组

//定义位操作
#define SET_BIT(REG,BIT)    ((REG) |= (BIT))  //BIT为0的位不变，为1的位设置为1
#define CLEAN_BIT(REG,BIT)  ((REG) &= ~(BIT)) //BIT为0的位不变，为1的位清空为0
#define TOGGLE_BIT(REG,BIT) ((REG) ^= (BIT))  //BIT为0的位不变，为1的位切换状态
#define READ_BIT(REG,BIT)   ((REG) & (BIT))   //检查REG中对应的BIT设置为1的位的值

//定义GPIO端口寄存器
struct GPIOx
{
    volatile unsigned int CRL;
    volatile unsigned int CRH;
    volatile unsigned int IDR;
    volatile unsigned int ODR;
    volatile unsigned int BSRR;
    volatile unsigned int BRR;
    volatile unsigned int LCKR;
};

//定义GPIO端口初地址
#define GPIOA ((volatile struct GPIOx*)0x40010800)
#define GPIOB ((volatile struct GPIOx*)0x40010C00)
#define GPIOC ((volatile struct GPIOx*)0x40011000)

//定义BSRR操作
#define BSRR_SET(REG,BIT)   ((REG) = (BIT))
#define BSRR_CLEAN(REG,BIT) ((REG) = ((BIT) << 16))

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
#define RCC ((volatile struct RCC*)0x40021000)

//定义SysTick寄存器
struct SysTick
{
    volatile unsigned int CAS;  //控制及状态寄存器
    volatile unsigned int RV;   //重装载数值寄存器
    volatile unsigned int CV;   //当前数值寄存器
    volatile unsigned int CALV; //校准数值寄存器
};

//定义SysTick初地址
#define SysTick ((volatile struct SysTick*)0xE000E010)

//定义Flash寄存器
struct Flash
{
    volatile unsigned int ACR;
    volatile unsigned int KEYR;
    volatile unsigned int OPTKEYR;
    volatile unsigned int SR;
    volatile unsigned int CR;
    volatile unsigned int AR;
    volatile unsigned int RESERVED; //保留
    volatile unsigned int OBR;
    volatile unsigned int WRPR;
};

//定义Flash初地址
#define Flash ((volatile struct Flash*)0x40022000)

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

void SystemInit(void) 
{
    SET_BIT((RCC->CR),(1 << 16));              //HSE使能
    while(READ_BIT((RCC->CR),(1 << 17)) == 0); //等待HSE就绪
    SET_BIT((RCC->CFGR),(1 << 16));            //设置HSE作为PLL输入时钟
    CLEAN_BIT((RCC->CFGR),(0xF << 18));
    SET_BIT((RCC->CFGR),(7 << 18));            //设置PLL倍频系数，当前值：9倍
    SET_BIT((RCC->CR),(1 << 24 ));             //PLL使能
    while(READ_BIT((RCC->CR),(1 << 25)) == 0); //等待PLL锁定
    SET_BIT((RCC->CR),(1 << 19));              //CSS使能
    CLEAN_BIT((Flash->ACR),(7));
    SET_BIT((Flash->ACR),(2));                 //设置Flash延时
    CLEAN_BIT((RCC->CFGR),(3));
    SET_BIT((RCC->CFGR),(2));                  //切换系统时钟为PLL倍频后的HSE
    while(((RCC->CFGR >> 2) & 3) != 2);        //确认系统时钟切换状态
}

//定义输出模式宏
#define GPIO_MODE_PP   0 //通用推挽输出模式
#define GPIO_MODE_OL   1 //通用开漏输出模式
#define GPIO_MODE_AFPP 2 //复用推挽输出模式

//gpio_type为引脚类型，pin_num为引脚号，范围0～15，mode为输出模式
void init_gpio(volatile struct GPIOx* gpio_type,int pin_num,int mode)
{
    if (gpio_type == GPIOA)
    {
        if (pin_num != 13 && pin_num != 14 && pin_num != 15)
        {
            SET_BIT((RCC->APB2ENR),(1 << 2)); 
            if (mode == GPIO_MODE_PP)   //通用推挽输出,50MHZ
            {
                if (pin_num >=0 && pin_num <= 7)
                {
                    CLEAN_BIT((GPIOA->CRL),(0xF << (pin_num * 4)));
                    SET_BIT((GPIOA->CRL),(3 << (pin_num * 4)));
                }
                if (pin_num >= 8 && pin_num <= 15)
                {
                    CLEAN_BIT((GPIOA->CRH),(0xF << ((pin_num - 8) * 4)));
                    SET_BIT((GPIOA->CRH),(3 << ((pin_num - 8) * 4)));
                }
            }
            if (mode == GPIO_MODE_OL)   //通用开漏输出,50MHZ
            {
                if (pin_num >=0 && pin_num <= 7)
                {
                    CLEAN_BIT((GPIOA->CRL),(0xF << (pin_num * 4)));
                    SET_BIT((GPIOA->CRL),(7 << (pin_num * 4)));
                }
                if (pin_num >= 8 && pin_num <= 15)
                {
                    CLEAN_BIT((GPIOA->CRH),(0xF << ((pin_num - 8) * 4)));
                    SET_BIT((GPIOA->CRH),(7 << ((pin_num - 8) * 4)));
                }
            }
            if (mode == GPIO_MODE_AFPP) //复用推挽输出模式,50MHZ
            {
                if (pin_num >=0 && pin_num <= 7)
                {
                    CLEAN_BIT((GPIOA->CRL),(0xF << (pin_num * 4)));
                    SET_BIT((GPIOA->CRL),(0xB << (pin_num * 4)));
                }
                if (pin_num >= 8 && pin_num <= 15)
                {
                    CLEAN_BIT((GPIOA->CRH),(0xF << ((pin_num - 8) * 4)));
                    SET_BIT((GPIOA->CRH),(0xB << ((pin_num - 8) * 4)));
                }
            }
        }
    }
    if (gpio_type == GPIOB)
    {
        if (pin_num != 3 && pin_num != 4)
        {
            SET_BIT((RCC->APB2ENR),(1 << 3)); 
            if (mode == GPIO_MODE_PP)  //通用推挽输出,50MHZ
            {
                if (pin_num >=0 && pin_num <= 7)
                {
                    CLEAN_BIT((GPIOB->CRL),(0xF << (pin_num * 4)));
                    SET_BIT((GPIOB->CRL),(3 << (pin_num * 4)));
                }
                if (pin_num >= 8 && pin_num <= 15)
                {
                    CLEAN_BIT((GPIOB->CRH),(0xF << ((pin_num - 8) * 4)));
                    SET_BIT((GPIOB->CRH),(3 << ((pin_num - 8) * 4)));
                }
            }
            if (mode == GPIO_MODE_OL)  //通用开漏输出,50MHZ
            {
                if (pin_num >=0 && pin_num <= 7)
                {
                    CLEAN_BIT((GPIOB->CRL),(0xF << (pin_num * 4)));
                    SET_BIT((GPIOB->CRL),(7 << (pin_num * 4)));
                }
                if (pin_num >= 8 && pin_num <= 15)
                {
                    CLEAN_BIT((GPIOB->CRH),(0xF << ((pin_num - 8) * 4)));
                    SET_BIT((GPIOB->CRH),(7 << ((pin_num - 8) * 4)));
                }
            }
            if (mode == GPIO_MODE_AFPP) //复用推挽输出模式,50MHZ
            {
                if (pin_num >=0 && pin_num <= 7)
                {
                    CLEAN_BIT((GPIOB->CRL),(0xF << (pin_num * 4)));
                    SET_BIT((GPIOB->CRL),(0xB << (pin_num * 4)));
                }
                if (pin_num >= 8 && pin_num <= 15)
                {
                    CLEAN_BIT((GPIOB->CRH),(0xF << ((pin_num - 8) * 4)));
                    SET_BIT((GPIOB->CRH),(0xB << ((pin_num - 8) * 4)));
                }
            }
        }
    }
    if (gpio_type == GPIOC)
    {
        SET_BIT((RCC->APB2ENR),(1 << 4)); 
        if (mode == GPIO_MODE_PP)   //通用推挽输出,50MHZ
        {
            if (pin_num >=0 && pin_num <= 7)
            {
                CLEAN_BIT((GPIOC->CRL),(0xF << (pin_num * 4)));
                SET_BIT((GPIOC->CRL),(3 << (pin_num * 4)));
            }
            if (pin_num >= 8 && pin_num <= 15)
            {
                CLEAN_BIT((GPIOC->CRH),(0xF << ((pin_num - 8) * 4)));
                SET_BIT((GPIOC->CRH),(3 << ((pin_num - 8) * 4)));
            }
        }
        if (mode == GPIO_MODE_OL)   //通用开漏输出,50MHZ
        {
            if (pin_num >=0 && pin_num <= 7)
            {
                CLEAN_BIT((GPIOC->CRL),(0xF << (pin_num * 4)));
                SET_BIT((GPIOC->CRL),(7 << (pin_num * 4)));
            }
            if (pin_num >= 8 && pin_num <= 15)
            {
                CLEAN_BIT((GPIOC->CRH),(0xF << ((pin_num - 8) * 4)));
                SET_BIT((GPIOC->CRH),(7 << ((pin_num - 8) * 4)));
            }
        }
        if (mode == GPIO_MODE_AFPP) //复用推挽输出模式,50MHZ
        {
            if (pin_num >=0 && pin_num <= 7)
            {
                CLEAN_BIT((GPIOC->CRL),(0xF << (pin_num * 4)));
                SET_BIT((GPIOC->CRL),(0xB << (pin_num * 4)));
            }
            if (pin_num >= 8 && pin_num <= 15)
            {
                CLEAN_BIT((GPIOC->CRH),(0xF << ((pin_num - 8) * 4)));
                SET_BIT((GPIOC->CRH),(0xB << ((pin_num - 8) * 4)));
            }
        }
    }
}

//定义电平宏
#define LOW  0 
#define HIGH 1

//gpio_type为引脚类型，pin_num为引脚号，范围0～15
void set_gpio(volatile struct GPIOx* gpio_type,int pin_num,int level)
{
    if (gpio_type == GPIOA)
    {
        if (level == LOW)
        {
            BSRR_CLEAN((GPIOA->BSRR),(1 << pin_num));
        }
        if (level == HIGH)
        {
            BSRR_SET((GPIOA->BSRR),(1 << pin_num));
        }        
    }
    if (gpio_type == GPIOB)
    {
        if (level == LOW)
        {
            BSRR_CLEAN((GPIOB->BSRR),(1 << pin_num));
        }
        if (level == HIGH)
        {
            BSRR_SET((GPIOB->BSRR),(1 << pin_num));
        }        
    }
    if (gpio_type == GPIOC)
    {
        if (level == LOW)
        {
            BSRR_CLEAN((GPIOC->BSRR),(1 << pin_num));
        }
        if (level == HIGH)
        {
            BSRR_SET((GPIOC->BSRR),(1 << pin_num));
        }        
    }
}

void systick_timing(int delay_time) //读CAS寄存器第16位判断倒计时是否结束
{
    SysTick->CAS = 0;            //CAS寄存器所有位置零
    SysTick->RV = 0;             //重载数值寄存器所有位置零
    SysTick->RV = delay_time;    //写入倒计时值
    SET_BIT((SysTick->CAS),(1)); //开启计时器
}

void delay_us(int delay_time)
{
    systick_timing(delay_time);
    while(READ_BIT((SysTick->CAS),(1 << 16)) == 0);
}

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

//输出极性宏套用前面电平宏定义

void set_pwm(volatile struct TIMx* tim_type,int channel_num,int mode,int frequency,int duty_cycle)
{
    if (frequency == 0);
    {
        return;
    }
    int arr_num = 1000000 / frequency;
    int ccr_num = duty_cycle * arr_num / 100;
    if (tim_type == TIM2)
    {
        TIM2->ARR = arr_num - 1;             //写入输出频率值，时钟频率1MHZ
        if (channel_num == CH1)
        { 
            TIM2->CCR1 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM2->CCER),(1 << 1));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM2->CCER),(1 << 1));
            }
            SET_BIT((TIM2->CCER),(1));       //输出使能
        }
        if (channel_num == CH2)
        {
            TIM2->CCR2 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM2->CCER),(1 << 5));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM2->CCER),(1 << 5));
            }
            SET_BIT((TIM2->CCER),(1 << 4));  //输出使能
        }
        if (channel_num == CH3)
        {
            TIM2->CCR3 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM2->CCER),(1 << 9));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM2->CCER),(1 << 9));
            }
            SET_BIT((TIM2->CCER),(1 << 8));  //输出使能
        }
        if (channel_num == CH4)
        {
            TIM2->CCR4 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM2->CCER),(1 << 13));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM2->CCER),(1 << 13));
            }
            SET_BIT((TIM2->CCER),(1 << 12)); //输出使能
        }
    }
    if (tim_type == TIM3)
    {
        TIM3->ARR = arr_num - 1;             //写入输出频率值，时钟频率1MHZ
        if (channel_num == CH1)
        { 
            TIM3->CCR1 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM3->CCER),(1 << 1));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM3->CCER),(1 << 1));
            }
            SET_BIT((TIM3->CCER),(1));       //输出使能
        }
        if (channel_num == CH2)
        {
            TIM3->CCR2 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM3->CCER),(1 << 5));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM3->CCER),(1 << 5));
            }
            SET_BIT((TIM3->CCER),(1 << 4));  //输出使能
        }
        if (channel_num == CH3)
        {
            TIM3->CCR3 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM3->CCER),(1 << 9));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM3->CCER),(1 << 9));
            }
            SET_BIT((TIM3->CCER),(1 << 8));  //输出使能
        }
        if (channel_num == CH4)
        {
            TIM3->CCR4 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM3->CCER),(1 << 13));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM3->CCER),(1 << 13));
            }
            SET_BIT((TIM3->CCER),(1 << 12)); //输出使能
        }
    }
    if (tim_type == TIM4)
    {
        TIM4->ARR = arr_num - 1;             //写入输出频率值，时钟频率1MHZ
        if (channel_num == CH1)
        { 
            TIM4->CCR1 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM4->CCER),(1 << 1));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM4->CCER),(1 << 1));
            }
            SET_BIT((TIM4->CCER),(1));       //输出使能
        }
        if (channel_num == CH2)
        {
            TIM4->CCR2 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM4->CCER),(1 << 5));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM4->CCER),(1 << 5));
            }
            SET_BIT((TIM4->CCER),(1 << 4));  //输出使能
        }
        if (channel_num == CH3)
        {
            TIM4->CCR3 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM4->CCER),(1 << 9));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM4->CCER),(1 << 9));
            }
            SET_BIT((TIM4->CCER),(1 << 8));  //输出使能
        }
        if (channel_num == CH4)
        {
            TIM4->CCR4 = ccr_num;
            if (mode == LOW)
            {
                CLEAN_BIT((TIM4->CCER),(1 << 13));
            }
            if (mode == HIGH)
            {
                SET_BIT((TIM4->CCER),(1 << 13));
            }
            SET_BIT((TIM4->CCER),(1 << 12)); //输出使能
        }
    }   
}

//定义设置高低电平宏
#define SCK_UP BSRR_SET((GPIOA_ADDRESS->BSRR),(1 << 5))
#define SCK_DOWN BSRR_CLEAN((GPIOA_ADDRESS->BSRR),(1 << 5))
#define SDA_UP BSRR_SET((GPIOA_ADDRESS->BSRR),(1 << 10))
#define SDA_DOWN BSRR_CLEAN((GPIOA_ADDRESS->BSRR),(1 << 10))

//定义ack应答信号检测宏
#define ACK_DETECTION READ_BIT((GPIOA_ADDRESS->IDR),(1 << 10))
#define ACK_ON  0
#define ACK_OFF 1

//定义延时宏对应不同延时需求
#define delay_normal 5
#define delay_init 100000

void i2c_start(void)
{
    SDA_UP;                 
    SCK_UP;                 
    delay_us(delay_normal);
    SDA_DOWN;
    delay_us(delay_normal);
    SCK_DOWN;
    delay_us(delay_normal);
}

void i2c_over(void)
{
    SDA_DOWN; 
    delay_us(delay_normal);
    SCK_UP; 
    delay_us(delay_normal);
    SDA_UP;
    delay_us(delay_normal);
}

void i2c_send (unsigned char send_num)
{
    for(int i = 7; i >= 0; i--) {
        SCK_DOWN;
        delay_us(delay_normal);
        if (READ_BIT((send_num),(1 << i))) 
        {
            SDA_UP; 
        }
        else 
        {
            SDA_DOWN;
        }
        delay_us(delay_normal);
        SCK_UP;   
        delay_us(delay_normal);
    }
    SCK_DOWN;
    delay_us(delay_normal);
    SDA_UP; 
    delay_us(delay_normal);
    SCK_UP;   
    delay_us(delay_normal); //ACK应答周期
    SCK_DOWN;
    delay_us(delay_normal);
}

void cmd_write(unsigned char cmd)
{
    i2c_start();
    unsigned char slave_address = 0x78;
    i2c_send(slave_address);
    unsigned char control_byte = 0x00;
    i2c_send(control_byte);
    i2c_send(cmd);
    i2c_over();
}

void double_cmd_write(unsigned char cmd_1,unsigned char cmd_2)
{
    i2c_start();
    unsigned char slave_address = 0x78;
    i2c_send(slave_address);
    unsigned char control_byte = 0x00;
    i2c_send(control_byte);
    i2c_send(cmd_1);
    i2c_send(cmd_2);
    i2c_over();
}

void data_write(unsigned char data)
{
    i2c_start();
    unsigned int slave_address = 0x78;
    i2c_send(slave_address);
    unsigned int control_byte = 0x40;
    i2c_send(control_byte);
    i2c_send(data);
    i2c_over();
}

void oled_clean(void)
{
    double_cmd_write(0x21,0x00); //Column Start 0
    cmd_write(0x7F);             //Column End 127
    double_cmd_write(0x22,0x00); //Page Start 0
    cmd_write(0x07);             //Page End 7   
    i2c_start();
    i2c_send(0x78);       
    i2c_send(0x40);
    for(int i = 0; i < 1024; i++)
    {
        i2c_send(0x00); 
    }
    i2c_over();
}

void oled_init(void)
{
    delay_us(delay_init);
    cmd_write(0xAE);             //关闭屏幕
    double_cmd_write(0xA8,0x3F); //Set MUX Ratio
    double_cmd_write(0xD3,0x00); //Set Display Offset
    cmd_write(0x40);             //Set Display Start Line
    double_cmd_write(0x20,0x00); //设置垂直寻址
    cmd_write(0xA1);             //Set Segment re-map 
    cmd_write(0xC8);             //Set COM Output Scan Direction 
    double_cmd_write(0xDA,0x12); //Set COM Pins hardware cofiguration
    double_cmd_write(0x81,0x7F); //Set Contrast Control
    cmd_write(0xA4);             //Disable Entire Display on 
    cmd_write(0xA6);             //Set Normal Display
    double_cmd_write(0xD5,0x80); //Set Osc Frequency
    double_cmd_write(0xD9,0xF1); //Set Pre-charge Period
    double_cmd_write(0xDB,0x30); //Set VCOMH Deselete Level
    double_cmd_write(0x8D,0x14); //Enable Charge Pump Regulato
    oled_clean();
    cmd_write(0xAF);             //Display On
}

void oled_display(const unsigned char *display_num)
{
    double_cmd_write(0x21,0x00); //Column Start 0
    cmd_write(0x7F);             //Column End 127
    double_cmd_write(0x22,0x00); //Page Start 0
    cmd_write(0x07);             //Page End 7   
    i2c_start();
    i2c_send(0x78);       
    i2c_send(0x40);
    for(int i = 0; i < 1024; i++)
    {
        i2c_send(display_num[i]); 
    }
    i2c_over();
}

//骗过链接器
void __libc_init_array(void) {}

//PC13
#define RCC_APB2ENR (*(volatile unsigned int*)0x40021018) 
#define GPIOC_CRH   (*(volatile unsigned int*)0x40011004)
#define GPIOC_ODR   (*(volatile unsigned int*)0x4001100C)

//切换延时
#define delay_change 1000000

//动画延时
#define delay_animation 80000

int main(void) 
{
    //PC13
    SET_BIT((RCC_APB2ENR),(1 << 4));
    CLEAN_BIT((GPIOC_CRH),(0xF << 20));
    SET_BIT((GPIOC_CRH),(0x3 << 20));
    //GPIO端口初始化
    init_gpio(GPIOA,5,GPIO_MODE_OL);
    init_gpio(GPIOA,10,GPIO_MODE_OL);
    set_gpio(GPIOA,5,HIGH);
    set_gpio(GPIOA,10,HIGH);
    oled_init();
    oled_display(nyan_cat);
    delay_us(delay_change);
    while (1)
    {
        oled_display(nyancat1);
        delay_us(delay_animation);
        oled_display(nyancat2);
        delay_us(delay_animation);
        oled_display(nyancat3);
        delay_us(delay_animation);
        oled_display(nyancat4);
        delay_us(delay_animation);
        oled_display(nyancat5);
        delay_us(delay_animation);
        oled_display(nyancat6);
        delay_us(delay_animation);
        oled_display(nyancat7);
        delay_us(delay_animation);
        oled_display(nyancat8);
        delay_us(delay_animation);
        oled_display(nyancat9);
        delay_us(delay_animation);
        oled_display(nyancat10);
        delay_us(delay_animation);
        oled_display(nyancat11);
        delay_us(delay_animation);
        oled_display(nyancat12);
        delay_us(delay_animation);
        oled_display(nyancat13);
        delay_us(delay_animation);
        oled_display(nyancat14);
        delay_us(delay_animation);
        oled_display(nyancat15);
        delay_us(delay_animation);
        oled_display(nyancat16);
        delay_us(delay_animation);
        oled_display(nyancat17);
        delay_us(delay_animation);
        oled_display(nyancat18);
        delay_us(delay_animation);
        oled_display(nyancat19);
        delay_us(delay_animation);
        oled_display(nyancat20);
        delay_us(delay_animation);
        oled_display(nyancat21);
        delay_us(delay_animation);
        oled_display(nyancat22);
        delay_us(delay_animation);
        oled_display(nyancat23);
        delay_us(delay_animation);
        oled_display(nyancat24);
        delay_us(delay_animation);
    }
}