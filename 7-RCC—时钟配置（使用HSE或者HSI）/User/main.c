/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   使用HSE或者HSI 配置系统时钟
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32F767 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f7xx.h"
#include "main.h"
#include "./led/bsp_led.h"
#include "./rcc/bsp_clkconfig.h"

void Delay(__IO uint32_t nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{

    // 程序来到main函数之前，启动文件：statup_stm32f746xx.s已经调用
    // SystemInit()函数把系统时钟初始化成16MHZ
    // SystemInit()在system_stm32f7xx.c中定义
    // 如果用户想修改系统时钟，可自行编写程序修改	
    // 重新设置系统时钟，这时候可以选择使用HSE还是HSI
  
    //系统时钟设置为216M，最高是250M
   HSE_SetSysClock(25, 432, 2, 9);
        
    // 使用HSI，配置系统时钟为216M
     //HSI_SetSysClock(16, 432, 2, 9);
        
    // LED 端口初始化 
    LED_GPIO_Config();
        
    // MCO1 输出PLLCLK	
    HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_2);
        
    // MCO2 输出SYSCLK
    HAL_RCC_MCOConfig(RCC_MCO2,RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_2);

    while (1)
    {
        LED1( ON );			 // 亮 
        Delay(0x0FFFFF);
        LED1( OFF );		 // 灭
        Delay(0x0FFFFF);
    }
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/

