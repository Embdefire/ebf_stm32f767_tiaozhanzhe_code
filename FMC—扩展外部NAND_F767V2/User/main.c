/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2017-xx-xx
  * @brief   GPIO���--ʹ�ù̼������LED��
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
#include "./usart/bsp_debug_usart.h"
#include "./sdram/bsp_sdram.h" 
//#include "./delay/core_delay.h" 
#include "./malloc/malloc.h"
#include "./nand/ftl.h"
#include "./nand/bsp_nand.h"
#include "./key/bsp_key.h" 
#include "./nand/nandtest.h"


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
uint8_t CNT = 0;
int main(void)
{   
	uint8_t *buf;
	uint8_t *backbuf;	
	
//	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	
	/* ϵͳʱ�ӳ�ʼ����400MHz */
	SystemClock_Config();
  Key_GPIO_Config();
	LED_GPIO_Config();
	/* ���ô���1Ϊ��115200 8-N-1 */
	DEBUG_USART_Config();
	printf("\r\n ��ӭʹ��Ұ��  STM32 F767 �����塣\r\n");		 

		
	/*��ʼ��SDRAMģ��*/
	SDRAM_Init();
	/*��ʼ���ⲿ�ڴ��*/
	my_mem_init(SRAMEX);		
	/*����������ʾ���ڶ�дSDRAM����*/
	LED_BLUE;

  while(FTL_Init())			    //���NAND FLASH,����ʼ��FTL
	{
		printf("NAND Error!");
//		Delay(500);				 
		printf("Please Check");
//		Delay(500);				 
//		LED1_TOGGLE;//�����˸
	}
	backbuf=mymalloc(SRAMEX,NAND_ECC_SECTOR_SIZE);	//����һ�������Ļ���
	buf=mymalloc(SRAMIN,NAND_ECC_SECTOR_SIZE);		//����һ�������Ļ���
	sprintf((char*)buf,"NAND Size:%dMB",(nand_dev.block_totalnum/1024)*(nand_dev.page_mainsize/1024)*nand_dev.block_pagenum);
	printf((char*)buf);	//��ʾNAND����  
	FTL_ReadSectors(backbuf,2,NAND_ECC_SECTOR_SIZE,1);//Ԥ�ȶ�ȡ����0����������,��ֹ��д�����ļ�ϵͳ��.
	

  CNT = 55;

//test_writepage(9,0,256,CNT);
//test_readpage(9,0,256);

	while(1)
	{
    CNT++;
    if(CNT >= 255)
      CNT = 1;
    if(Key_Scan(KEY1_GPIO_PORT, KEY1_PIN) == KEY_ON)
    {
      /* д֮ǰ��Ҫ���� */
      NAND_EraseBlock(1);
      test_writepage(1*64,0,215,CNT);
      printf("write: %d\n", CNT);
    }
    
    if(Key_Scan(KEY2_GPIO_PORT, KEY2_PIN) == KEY_ON)
    {
      test_readpage(1*64,0,215);                                  //0x40
    }
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
  
	/* ��HAL_RCC_ClockConfig��������ͬʱ��ʼ������ϵͳ��ʱ��systick������Ϊ1ms�ж�һ�� */
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }  
}

/*********************************************END OF FILE**********************/

