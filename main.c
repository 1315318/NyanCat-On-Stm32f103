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

void delay(int n) {
    while(n--) { __asm("nop"); } // 简单的软件延时
}

// 骗过链接器：告诉它 SystemInit 在这，虽然它什么都不干
void SystemInit(void) {}
// 骗过链接器：告诉它 C 库初始化在这
void __libc_init_array(void) {}

int main(void) {
    //开启所有端口的RCC_APB2ENR
    RCC->APB2ENR |= (0x7F << 2);

    /* 目标 B：配置 PC13 为推挽输出模式 (50MHz) */
    // PC13 对应 CRH 寄存器的 [23:20] 这四位
    // 0x3 代表输出模式 50MHz, 0x0 代表通用推挽
    GPIOC->CRH &= ~(0xF << 20); // 先清空这 4 位
    GPIOC->CRH |= (0x3 << 20);  // 填入二进制 0011 (即 0x3)
    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |= (0x3 << 4);

    /* 目标 C：进入死循环，让灯闪烁 */
    while(1) {
        // 输出低电平 (Bit 13 写 0) -> LED 亮
        GPIOC->ODR &= ~(1 << 13);
        GPIOA->ODR &= ~(1 << 1);
        delay(100000);

        // 输出高电平 (Bit 13 写 1) -> LED 灭
        GPIOC->ODR |= (1 << 13);
        GPIOA->ODR |= (1 << 1);
        delay(100000);
    }
}