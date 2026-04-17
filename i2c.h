#ifndef I2C_H
#define I2C_H

//定义设置高低电平宏
#define SCK_UP   BSRR_SET((GPIOA->BSRR),(1 << 5))
#define SCK_DOWN BSRR_CLEAN((GPIOA->BSRR),(1 << 5))
#define SDA_UP   BSRR_SET((GPIOA->BSRR),(1 << 10))
#define SDA_DOWN BSRR_CLEAN((GPIOA->BSRR),(1 << 10))

//定义ack应答信号检测宏
#define ACK_DETECTION READ_BIT((GPIOA->IDR),(1 << 10))
#define ACK_ON  0
#define ACK_OFF 1

//定义延时宏对应不同延时需求
#define delay_normal 5
#define delay_init   100000

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

#endif