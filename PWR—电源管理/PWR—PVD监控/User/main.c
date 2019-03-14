/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ����F767 PVD�������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 767 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "main.h"  
#include "stm32f7xx.h"
#include "./led/bsp_led.h" 
#include "./usart/bsp_usart.h"
#include "./key/bsp_key.h" 
#include "./pvd/bsp_pvd.h"

/*
*�� ����ʵ�������
* 1.ʹ���ⲿ�ɵ���Դ�����ڳ�5V��������ӵ�ʵ��������� 5V<--->GND��������ӹ��磻
*2.��λʵ��壬��ѹ����ʱ�����ϵ�LED�ʵ�ӦΪ��ɫ
*3.���µ��ڿɵ���Դ�ĵ�ѹ����Լ������4V��ʱ��LED�ʵƻ�תΪ��ɫ��
*�������п���PVD��ص�ѹԼΪ2.8V,��5V��Դ����4V��ʱ������STM32��VDD��Դ(3.3V��Դ)�����2.8V������PVD�¼������ж��п�������ƣ�
*/

/*�� ������ע�����
*ʹ�ÿɵ���Դ��ʵ��幩�磬������Դ�߶��ε�(������������USB��)��
*����ֱ�ӽ����빩��û�е�·���������ڵ�ԴʱС�Ĳ�Ҫʹ�����ѹԶ����5V,��ѹ̫�߻��ջ�ʵ��壡��
*/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{ 
	/* ����ϵͳʱ��Ϊ216 MHz */
    SystemClock_Config();
	/* ��ʼ��LED */
	LED_GPIO_Config();	
	//���̵ƣ���ʾ��������
	LED_GREEN; 
	
	//����PVD������ѹ����ʱ��������жϷ������������
	PVD_Config();
	
	while(1)
	{				
		/*�������еĳ���*/
	}

}
/**
  * @brief  PWR PVD interrupt callback
  * @param  None 
  * @retval None
  */
void HAL_PWR_PVDCallback(void)
{
  /* ����ƣ�ʵ��Ӧ����Ӧ�������״̬���� */
  LED_RED;
}
/**
  * @brief  System Clock ����
  *         system Clock �������� : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  ��
  * @retval ��
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* ʹ��HSE������HSEΪPLL��ʱ��Դ������PLL�ĸ��ַ�Ƶ����M N P Q 
	 * PLLCLK = HSE/M*N/P = 25M / 25 *432 / 2 = 216M
	 */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* ���� OverDrive ģʽ�Դﵽ216MƵ��  */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* ѡ��PLLCLK��ΪSYSCLK�������� HCLK, PCLK1 and PCLK2 ��ʱ�ӷ�Ƶ���� 
	 * SYSCLK = PLLCLK     = 216M
	 * HCLK   = SYSCLK / 1 = 216M
	 * PCLK2  = SYSCLK / 2 = 108M
	 * PCLK1  = SYSCLK / 4 = 54M
	 */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }  
}
/*********************************************END OF FILE**********************/

