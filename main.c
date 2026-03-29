/*PA13 (JTMS / SWDIO)：SWD 模式的数据线
PA14 (JTCK / SWCLK)：SWD 模式的时钟线
PA15 (JTDI)：JTAG 专用
PB3 (JTDO)：JTAG 专用
PB4 (NJTRST)：JTAG 专用*/

//定义GPIO端口寄存器
struct GPIOx{
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
#define GPIOD ((volatile struct GPIOx*)0x40011400)
#define GPIOE ((volatile struct GPIOx*)0x40011800)
#define GPIOF ((volatile struct GPIOx*)0x40011C00)
#define GPIOG ((volatile struct GPIOx*)0x40012000)

//定义RCC寄存器
struct RCC{
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

//定义位操作
#define SET_BIT(REG,BIT) ((REG) |= (BIT))//BIT为0的位不变，为1的位设置为1
#define CLEAN_BIT(REG,BIT) ((REG) &= ~(BIT))//BIT为0的位不变，为1的位清空为0
#define TOGGLE_BIT(REG,BIT) ((REG) ^= (BIT))//BIT为0的位不变，为1的位切换状态
#define READ_BIT(REG,BIT) ((REG) & (BIT))//检查REG中对应的BIT设置为1的位的值

void init_gpioa(int pin_num)//pin_num为引脚号，范围0～15
{
    if (pin_num != 13 && pin_num != 14 && pin_num != 15)
    {
        SET_BIT((RCC->APB2ENR),(1 << 2)); 
        if (pin_num >=0 && pin_num <= 7)
        {
            CLEAN_BIT((GPIOA->CRL),(0xF << (pin_num * 4)));
            SET_BIT((GPIOA->CRL),(0x3 << (pin_num * 4)));
        }
        if (pin_num >= 8 && pin_num <= 15)
        {
            CLEAN_BIT((GPIOA->CRH),(0xF << ((pin_num - 8) * 4)));
            SET_BIT((GPIOA->CRH),(0x3 << ((pin_num - 8) * 4)));
        }
    }
}

void set_gpioa(int pin_num,char level)//pin_num为引脚号，范围0～15；level为电平，h高l低
{
    if (level == 'l')
    {
        CLEAN_BIT((GPIOA->ODR),(1 << pin_num));
    }
    if (level == 'h')
    {
        SET_BIT((GPIOA->ODR),(1 << pin_num));
    }
}

void delay(int n) {
    while(n--) { __asm("nop"); } // 简单的软件延时
}

// 骗过链接器
void SystemInit(void) {}
void __libc_init_array(void) {}

//PC13
#define RCC_APB2ENR (*(volatile unsigned int*)0x40021018) 
#define GPIOC_CRH (*(volatile unsigned int*)0x40011004)
#define GPIOC_ODR (*(volatile unsigned int*)0x4001100C)

int main(void) {
    SET_BIT((RCC_APB2ENR),(1 << 4));
    CLEAN_BIT((GPIOC_CRH),(0xF << 20));
    SET_BIT((GPIOC_CRH),(0x3 << 20));
    init_gpioa(10);
    while(1) {
        CLEAN_BIT((GPIOC_ODR),(1 << 13));
        char level = 'l';
        set_gpioa(10,level);
        delay(100000);
        SET_BIT((GPIOC_ODR),(1 << 13));
        level = 'h';
        set_gpioa(10,level);
        delay(100000);
    }
}