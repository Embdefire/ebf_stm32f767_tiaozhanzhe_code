/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   AP3216C���Գ���
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
#include <stdlib.h>
#include "main.h"
#include "./i2c/i2c.h"
#include "./ap3216c/ap3216c.h"
//�����Ƿ�ʹ��LCD������ʾ������Ҫ�Ļ��������ע�͵�����
//#define USE_LCD_DISPLAY

#ifdef USE_LCD_DISPLAY
 #include "./lcd/bsp_lcd.h"
#endif

/*���������*/
uint32_t Task_Delay[NumOfTask]={0};
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  float ALS;
  uint16_t PS;
  uint16_t IR;
  uint8_t INTstatus;
	
  /* ϵͳʱ�ӳ�ʼ����216 MHz */
  SystemClock_Config();
  /* LED �˿ڳ�ʼ�� */
  LED_GPIO_Config();

  #ifdef USE_LCD_DISPLAY		
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

  /* �ڶ�����������ʾ͸�� */ 
  LCD_Clear(LCD_COLOR_TRANSPARENT);

  /* ���õ�һ�͵ڶ����͸����,��СֵΪ0�����ֵΪ255*/
  LCD_SetTransparency(0, 255);
  LCD_SetTransparency(1, 0);

  /* ѡ��LCD��һ�� */
  LCD_SelectLayer(0);
  /*����������ɫ������ı�����ɫ*/
  LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BLACK);	
  #endif
  /*��ʼ��USART1*/
  DEBUG_USART_Config(); 

  //��ʼ�� I2C
  I2cMaster_Init(); 

  printf("\r\n ��ӭʹ��Ұ�� STM32F767 �����塣\r\n");		 

  printf("\r\n ����һ��AP3216Cȫ���ܲ������� \r\n");

  //AP3216C��ʼ��
  AP3216C_Init();

  Delay(250);

  while(1)
  {
    if(Task_Delay[0]==TASK_ENABLE)
    {
      LED2_TOGGLE;
      Task_Delay[0]=1000;
    }
    if(Task_Delay[1]==0)
    {
      INTstatus = AP3216C_Get_INTStatus();
      ALS = AP3216C_ReadALS();
      PS = AP3216C_ReadPS();
      IR = AP3216C_ReadIR();
      printf("\r\n�����⣺%.2flux ����ǿ�ȣ�%d\r\n", ALS, IR);			
      
      if(PS == 55555)
        printf("IR̫ǿ��PS������Ч\r\n");
      else
        printf("�ӽ������ǣ�%d\r\n", PS & 0x3FF);
      
//      /* ��ѯ�������ж� */
//      if (AP_INT_Read() == 0)
//      printf("���жϲ���\n");
    
      if ((PS >> 15) & 1)
        printf("����ӽ�\n");
      else
        printf("����Զ��\n");
      
      if (INTstatus & 0x1)
        printf("ALS �����ж�\n");
      
      if (INTstatus >> 1 & 0x1)
        printf("PS �����ж�\n");
      
      #ifdef USE_LCD_DISPLAY	
        {
          char cStr [ 70 ];
          sprintf ( cStr, "ALS��%8.2flux",ALS);	//����������

          LCD_DisplayStringLine(7,(uint8_t* )cStr);			

          sprintf ( cStr, "PS ��%8d ",PS_RAW);	//�ӽ�ֵ����

          LCD_DisplayStringLine(8,(uint8_t* )cStr);			

          sprintf ( cStr, "IR ��%8d",IR_RAW);	//�����
          LCD_DisplayStringLine(9,(uint8_t* )cStr);			

        }
      #endif
      
      Task_Delay[1]=225; //�ɸ����Լ���������߲���Ƶ�ʣ���С�������225ms
      
    }

    //*************************************	��������������ĸ�ʽ************************************//
//		if(Task_Delay[i]==0)
//		{
//			Task(i);
//			Task_Delay[i]=;
//		}

  }
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

