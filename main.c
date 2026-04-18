/*PA13 (JTMS / SWDIO)：SWD 模式的数据线
PA14 (JTCK / SWCLK)：SWD 模式的时钟线
PA15 (JTDI)：JTAG 专用
PB3 (JTDO)：JTAG 专用
PB4 (NJTRST)：JTAG 专用*/

#include "nyancat_video.h" //图像数组
#include "nyancat_audio.h" //音频数组
#include "base_define.h"   //位操作及高低电平宏的定义
#include "flash.h"         //Flash寄存器及初地址相关定义
#include "rcc.h"           //RCC寄存器及初地址相关定义和HSE时钟初始化函数
#include "gpio.h"          //GPIO寄存器及初地址相关定义和GPIO端口初始化与设置函数
#include "systick.h"       //SysTick寄存器及初地址相关定义和延时函数函数
#include "timx.h"          //TIMx寄存器及初地址相关定义和pwm初始化与设置函数
#include "i2c.h"           //I2C协议的实现和OLED屏幕初始化与显示函数

//PC13
#define RCC_APB2ENR (*(volatile unsigned int*)0x40021018) 
#define GPIOC_CRH   (*(volatile unsigned int*)0x40011004)
#define GPIOC_ODR   (*(volatile unsigned int*)0x4001100C)

#define delay_change    1000 //切换延时
#define delay_animation 10   //动画延时

int main(void) 
{
    //PC13
    SET_BIT((RCC_APB2ENR),(1 << 4));
    CLEAN_BIT((GPIOC_CRH),(0xF << 20));
    SET_BIT((GPIOC_CRH),(0x3 << 20));
    //GPIO端口初始化
    enr_gpio(GPIOA);
    init_gpio(GPIOA,5,GPIO_MODE_OL);
    init_gpio(GPIOA,10,GPIO_MODE_OL);
    set_gpio(GPIOA,5,HIGH);
    set_gpio(GPIOA,10,HIGH);
    //OLED初始化
    oled_init();
    oled_display(nyan_cat);
    delay_ms(delay_change);
    //主循环
    while (1)
    {
        oled_display(nyancat1);
        delay_ms(delay_animation);
        oled_display(nyancat2);
        delay_ms(delay_animation);
        oled_display(nyancat3);
        delay_ms(delay_animation);
        oled_display(nyancat4);
        delay_ms(delay_animation);
        oled_display(nyancat5);
        delay_ms(delay_animation);
        oled_display(nyancat6);
        delay_ms(delay_animation);
        oled_display(nyancat7);
        delay_ms(delay_animation);
        oled_display(nyancat8);
        delay_ms(delay_animation);
        oled_display(nyancat9);
        delay_ms(delay_animation);
        oled_display(nyancat10);
        delay_ms(delay_animation);
        oled_display(nyancat11);
        delay_ms(delay_animation);
        oled_display(nyancat12);
        delay_ms(delay_animation);
        oled_display(nyancat13);
        delay_ms(delay_animation);
        oled_display(nyancat14);
        delay_ms(delay_animation);
        oled_display(nyancat15);
        delay_ms(delay_animation);
        oled_display(nyancat16);
        delay_ms(delay_animation);
        oled_display(nyancat17);
        delay_ms(delay_animation);
        oled_display(nyancat18);
        delay_ms(delay_animation);
        oled_display(nyancat19);
        delay_ms(delay_animation);
        oled_display(nyancat20);
        delay_ms(delay_animation);
        oled_display(nyancat21);
        delay_ms(delay_animation);
        oled_display(nyancat22);
        delay_ms(delay_animation);
        oled_display(nyancat23);
        delay_ms(delay_animation);
        oled_display(nyancat24);
        delay_ms(delay_animation);
    }
}