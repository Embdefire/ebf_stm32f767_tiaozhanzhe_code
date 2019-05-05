/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ʹ��HSE����HSI ����ϵͳʱ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32F767 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f7xx.h"
#include "main.h"
#include "./led/bsp_led.h"
#include "./rcc/bsp_clkconfig.h"

void Delay(__IO uint32_t nCount); 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{

    // ��������main����֮ǰ�������ļ���statup_stm32f746xx.s�Ѿ�����
    // SystemInit()������ϵͳʱ�ӳ�ʼ����16MHZ
    // SystemInit()��system_stm32f7xx.c�ж���
    // ����û����޸�ϵͳʱ�ӣ������б�д�����޸�	
    // ��������ϵͳʱ�ӣ���ʱ�����ѡ��ʹ��HSE����HSI
  
    //ϵͳʱ������Ϊ216M�������250M
   HSE_SetSysClock(25, 432, 2, 9);
        
    // ʹ��HSI������ϵͳʱ��Ϊ216M
     //HSI_SetSysClock(16, 432, 2, 9);
        
    // LED �˿ڳ�ʼ�� 
    LED_GPIO_Config();
        
    // MCO1 ���PLLCLK	
    HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_2);
        
    // MCO2 ���SYSCLK
    HAL_RCC_MCOConfig(RCC_MCO2,RCC_MCO2SOURCE_SYSCLK, RCC_MCODIV_2);

    while (1)
    {
        LED1( ON );			 // �� 
        Delay(0x0FFFFF);
        LED1( OFF );		 // ��
        Delay(0x0FFFFF);
    }
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/

