/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   I2S����Ƶ������¼��
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
#include "./TouchPad/bsp_touchpad.h"
#include "./wm8978/bsp_wm8978.h"  
#include "./Recorder/Recorder.h"

/* FatFs includes component */
#include "ff.h"
#include "sd_diskio.h"
/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
char SDPath[4]; /* SD�߼�������·�� */
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FRESULT res_sd;                /* �ļ�������� */

static void SystemClock_Config(void);
/**
	**************************************************************
	* Description : ��ʼ��WiFiģ��ʹ�����ţ�������WiFiģ��
	* Argument(s) : none.
	* Return(s)   : none.
	**************************************************************
	*/
static void WIFI_PDN_INIT(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/*ʹ������ʱ��*/	
	__HAL_RCC_GPIOG_CLK_ENABLE();
	/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStruct.Pin = GPIO_PIN_9;	
	/*�������ŵ��������Ϊ�������*/
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;      
	/*��������Ϊ����ģʽ*/
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	/*������������Ϊ���� */   
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST; 
	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);	
	/*����WiFiģ��*/
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);  
}

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* ����ϵͳʱ��Ϊ216 MHz */
    SystemClock_Config();
    /* ʹ��ָ��� */
    SCB_EnableICache();
	/*����WiFiģ��*/
	WIFI_PDN_INIT();
	/* ��ʼ��LED */
	LED_GPIO_Config();	
	LED_BLUE;	
	/* ��ʼ���������� */
	TPAD_Init();
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	UARTx_Config();	
    printf("WM8978¼���ͻطŹ���\n");
    //�����������������̷�
    FATFS_LinkDriver(&SD_Driver, SDPath);
	//���ⲿSD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SD����ʼ��
	res_sd = f_mount(&fs,"0:",1);

	if(res_sd!=FR_OK)
	{
		printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
		printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
		while(1);
	}
	else
	{
		printf("SD�����سɹ�\r\n");
	}
	/* ���WM8978оƬ���˺������Զ�����CPU��GPIO */
	if (wm8978_Init()==0)
	{
		printf("��ⲻ��WM8978оƬ!!!\n");
		while (1);	/* ͣ�� */
	}
	printf("��ʼ��WM8978�ɹ�\n");	
	
	/* ¼����طŹ��� */
	RecorderDemo();
	
	while(1)
	{		
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

