#ifndef GPIO_H
#define GPIO_H

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

//定义输出模式宏
#define GPIO_MODE_PP   0 //通用推挽输出模式
#define GPIO_MODE_OL   1 //通用开漏输出模式
#define GPIO_MODE_AFPP 2 //复用推挽输出模式

void enr_gpio(volatile struct GPIOx* gpio_type)
{
    if (gpio_type == GPIOA)
    {
        SET_BIT((RCC->APB2ENR),(1 << 2)); 
    }
    if (gpio_type == GPIOB)
    {
        SET_BIT((RCC->APB2ENR),(1 << 3)); 
    }
    if (gpio_type == GPIOC)
    {
        SET_BIT((RCC->APB2ENR),(1 << 4)); 
    }
}

//gpio_type为引脚类型，pin_num为引脚号，范围0～15，mode为输出模式
void init_gpio(volatile struct GPIOx* gpio_type,int pin_num,int mode)
{
    if (pin_num >=0 && pin_num <= 7)
    {
        CLEAN_BIT((gpio_type->CRL),(0xF << (pin_num * 4)));
        if (mode == GPIO_MODE_PP)   //通用推挽输出,50MHZ
        {
            SET_BIT((gpio_type->CRL),(0x3 << (pin_num * 4)));
        }
        if (mode == GPIO_MODE_OL)   //通用开漏输出,50MHZ
        {
            SET_BIT((gpio_type->CRL),(0x7 << (pin_num * 4)));
        }
        if (mode == GPIO_MODE_AFPP) //复用推挽输出模式,50MHZ
        {
            SET_BIT((gpio_type->CRL),(0xB << (pin_num * 4)));
        }
    }
    if (pin_num >= 8 && pin_num <= 15)
    {
        CLEAN_BIT((gpio_type->CRH),(0xF << ((pin_num - 8) * 4)));
        if (mode == GPIO_MODE_PP)   //通用推挽输出,50MHZ
        {
            SET_BIT((gpio_type->CRH),(0x3 << ((pin_num - 8) * 4)));
        }
        if (mode == GPIO_MODE_OL)   //通用开漏输出,50MHZ
        {
            SET_BIT((gpio_type->CRH),(0x7 << ((pin_num - 8) * 4)));
        }
        if (mode == GPIO_MODE_AFPP) //复用推挽输出模式,50MHZ
        {
            SET_BIT((gpio_type->CRH),(0xB << ((pin_num - 8) * 4)));
        }
    }
}

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

#endif 