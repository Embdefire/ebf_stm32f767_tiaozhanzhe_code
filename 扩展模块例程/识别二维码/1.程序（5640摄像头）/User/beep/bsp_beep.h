#ifndef __BEEP_H
#define	__BEEP_H


#include "stm32f7xx.h"


/* 定义蜂鸣器连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的蜂鸣器引脚 */
#define BEEP_GPIO_PORT    	    GPIOI			              /* GPIO端口 */
#define BEEP_GPIO_CLK_ENABLE()  __GPIOI_CLK_ENABLE()  /* GPIO端口时钟 */
#define BEEP_GPIO_PIN		  	GPIO_PIN_11			          /* 连接到蜂鸣器的GPIO */


/* 带参宏，可以像内联函数一样使用 */
#define BEEP(a)	HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,a)
					
/* 直接操作寄存器的方法控制IO */
#define	beep_digitalHi(p,i)			{p->BSRR = i;}			        //设置为高电平		
#define beep_digitalLo(p,i)			{p->BSRR = (uint32_t)i << 16;;}  //输出低电平
#define beep_digitalToggle(p,i)	    {p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define BEEP_TOGGLE		beep_digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON	        beep_digitalHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF		beep_digitalLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

void BEEP_GPIO_Config(void);
void BEEP_Handler(uint8_t times);				
#endif /* __BEEP_H */
