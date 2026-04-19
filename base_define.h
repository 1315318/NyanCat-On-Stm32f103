#ifndef BASE_DEFINE_H
#define BASE_DEFINE_H

//定义位操作
#define SET_BIT(REG,BIT)    ((REG) |= (BIT))  //BIT为0的位不变，为1的位设置为1
#define CLEAN_BIT(REG,BIT)  ((REG) &= ~(BIT)) //BIT为0的位不变，为1的位清空为0
#define TOGGLE_BIT(REG,BIT) ((REG) ^= (BIT))  //BIT为0的位不变，为1的位切换状态
#define READ_BIT(REG,BIT)   ((REG) & (BIT))   //检查REG中对应的BIT设置为1的位的值

//定义电平
#define LOW  0 
#define HIGH 1

//骗过链接器
void __libc_init_array(void) {}

#endif