/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   RTC������ʵ��
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
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_lcd.h"
#include <stdlib.h>
#include "main.h"
#include "./beep/bsp_beep.h"
#include "./RTC/bsp_rtc.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    /* ϵͳʱ�ӳ�ʼ����216 MHz */
    SystemClock_Config();
    /* LED �˿ڳ�ʼ�� */
    LED_GPIO_Config();
	/* �������˿ڳ�ʼ�� */	
	BEEP_GPIO_Config();
    /* LCD �˿ڳ�ʼ�� */ 
    LCD_Init();
    /* LCD ��һ���ʼ�� */ 
    LCD_LayerInit(0, LCD_FB_START_ADDRESS,ARGB8888);
	/* LCD �ڶ����ʼ�� */ 
    LCD_LayerInit(1, LCD_FB_START_ADDRESS+(LCD_GetXSize()*LCD_GetYSize()*4),ARGB8888);
    /* ʹ��LCD������������ */ 
    LCD_DisplayOn(); 

    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(0);

    /* ��һ����������ʾȫ�� */ 
    LCD_Clear(LCD_COLOR_BLACK);  

    /* ѡ��LCD�ڶ��� */
    LCD_SelectLayer(1);

    /* �ڶ�����������ʾȫ�� */ 
    LCD_Clear(LCD_COLOR_TRANSPARENT);

    /* ���õ�һ�͵ڶ����͸����,��СֵΪ0�����ֵΪ255*/
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 0);
	
	/* ѡ��LCD��һ�� */
    LCD_SelectLayer(0);
    /*=========================Һ����ʼ������===============================*/   
	/*
	 * ���������ù�RTCʱ��֮��������ݼĴ���0д��һ�����������
	 * ����ÿ�γ����������е�ʱ���ͨ����ⱸ�ݼĴ���0��ֵ���ж�
	 * RTC �Ƿ��Ѿ����ù���������ù��Ǿͼ������У����û�����ù�
	 * �ͳ�ʼ��RTC������RTC��ʱ�䡣
	 */
  
	/* RTC���ã�ѡ��ʱ��Դ������RTC_CLK�ķ�Ƶϵ�� */
	RTC_CLK_Config();
   
	if (HAL_RTCEx_BKUPRead(&Rtc_Handle,RTC_BKP_DRX) != 0X32F3)
	{		
		/* �������� */
		RTC_AlarmSet();
		
		/* ����ʱ������� */
		RTC_TimeAndDate_Set();
		

	}
	else
	{
		/* ����Ƿ��Դ��λ */
		if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
		{
		  printf("\r\n ������Դ��λ....\r\n");
		}
		/* ����Ƿ��ⲿ��λ */
		else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
		{
		  printf("\r\n �����ⲿ��λ....\r\n");
		}

		printf("\r\n ����Ҫ��������RTC....\r\n");

		/* ʹ�� PWR ʱ�� */
		__HAL_RCC_PWR_CLK_ENABLE();
		/* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
		HAL_PWR_EnableBkUpAccess();
		/* �ȴ� RTC APB �Ĵ���ͬ�� */
		HAL_RTC_WaitForSynchro(&Rtc_Handle);

	} 
	
	/* ��ʾʱ������� */
	RTC_TimeAndDate_Show();
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

