/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ����F767 ֹͣģʽ����
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
#include "./key/bsp_exti.h" 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void) 
{
	uint32_t SYSCLK_Frequency=0; 
	uint32_t HCLK_Frequency=0;
	uint32_t PCLK1_Frequency=0;
	uint32_t PCLK2_Frequency=0;
	uint32_t SYSCLK_Source=0;
	
	/* ��ʼ��ϵͳʱ��Ϊ216MHz */
	SystemClock_Config();
	/* ��ʼ��LED */
	LED_GPIO_Config();	
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	UARTx_Config();	
	/* ��ʼ������Ϊ�ж�ģʽ�������жϺ������жϷ�����	*/
	EXTI_Key_Config(); 
	
	printf("\r\n ��ӭʹ��Ұ�� STM32F767 �����塣\r\n");
    printf("\r\n ����F767 ֹͣģʽ����\r\n");
	
	printf("\r\n ʵ��˵����\r\n");

	printf("\r\n 1.�������У��̵Ʊ�ʾSTM32�������У���Ʊ�ʾ˯��״̬�����Ʊ�ʾ�մ�ֹͣ״̬������\r\n");
	printf("\r\n 2.��������һ��ʱ����Զ�����ֹͣ״̬����ֹͣ״̬�£���ʹ��KEY1��KEY2����\r\n");
	printf("\r\n 3.��ʵ��ִ������һ��ѭ����\r\n ------�����̵�(��������)->�����(ֹͣģʽ)->��KEY1��KEY2����->������(�ձ�����)-----��\r\n");
	printf("\r\n 4.��ֹͣ״̬�£�DAP�������޷���STM32���س���\r\n �ɰ�KEY1��KEY2���Ѻ����أ�\r\n �򰴸�λ��ʹоƬ���ڸ�λ״̬��Ȼ���ڵ����ϵ�����ذ�ť�����ͷŸ�λ��������������\r\n");

	while(1)
	{	
		/*********ִ������***************************/
		printf("\r\n STM32�������У����̵�\r\n");

		LED_GREEN;	
		HAL_Delay(2000);		
		/*****����ִ����ϣ�����˯�߽��͹���***********/

		printf("\r\n ����ֹͣģʽ�������,��KEY1��KEY2�����ɻ���\r\n");

		//ʹ�ú��ָʾ������˯��״̬
		LED_RED;
		//��ͣ�δ�ʱ�ӣ���ֹͨ���δ�ʱ���жϻ���
		HAL_SuspendTick();
		/*����ֹͣģʽʱ��FLASH�������״̬*/
		HAL_PWREx_EnableFlashPowerDown();
		/* ����ֹͣģʽ�����õ�ѹ������Ϊ�͹���ģʽ���ȴ��жϻ��� */
		HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON,PWR_STOPENTRY_WFI);
		//�ȴ��жϻ���  K1��K2�����ж�	
		/***�����ѣ�������ָʾ***/
		LED_BLUE;
		//����ʱ�ӼĴ�����ֵ����SystemCoreClock����
		SystemCoreClockUpdate();
		//��ȡ���Ѻ��ʱ��״̬	
		SYSCLK_Frequency = HAL_RCC_GetSysClockFreq(); 
		HCLK_Frequency   = HAL_RCC_GetHCLKFreq();
		PCLK1_Frequency  = HAL_RCC_GetPCLK1Freq();
		PCLK2_Frequency  = HAL_RCC_GetPCLK2Freq();
		SYSCLK_Source    = __HAL_RCC_GET_SYSCLK_SOURCE();
		//�������ڴ���ֱ��ʹ��HSIʱ�ӣ������Ӱ�촮�ڲ�����
		printf("\r\n�ջ��ѵ�ʱ��״̬��\r\n");
		printf(" SYSCLKƵ��:%d,\r\n HCLKƵ��:%d,\r\n PCLK1Ƶ��:%d,\r\n PCLK2Ƶ��:%d,\r\n ʱ��Դ:%d (0��ʾHSI��8��ʾPLLCLK)\n", 
		SYSCLK_Frequency,HCLK_Frequency,PCLK1_Frequency,PCLK2_Frequency,SYSCLK_Source);		  
		/* ��ֹͣģʽ���Ѻ�����ϵͳʱ��:����HSE��PLL*/
		/* ѡ��PLL��Ϊϵͳʱ��Դ(HSE��PLL��ֹͣģʽ�±�����)*/
		SYSCLKConfig_STOP();
		
		//�����Ѻ󣬻ָ��δ�ʱ��			
		HAL_ResumeTick();
		//��ȡ�������ú��ʱ��״̬
		SYSCLK_Frequency = HAL_RCC_GetSysClockFreq(); 
		HCLK_Frequency   = HAL_RCC_GetHCLKFreq();
		PCLK1_Frequency  = HAL_RCC_GetPCLK1Freq();
		PCLK2_Frequency  = HAL_RCC_GetPCLK2Freq();
		SYSCLK_Source    = __HAL_RCC_GET_SYSCLK_SOURCE();
		
		//��������ʱ��Դ��ʼ��״̬
		printf("\r\n�������ú��ʱ��״̬��\r\n");
		printf(" SYSCLKƵ��:%d,\r\n HCLKƵ��:%d,\r\n PCLK1Ƶ��:%d,\r\n PCLK2Ƶ��:%d,\r\n ʱ��Դ:%d (0��ʾHSI��8��ʾPLLCLK)\n", 
		SYSCLK_Frequency,HCLK_Frequency,PCLK1_Frequency,PCLK2_Frequency,SYSCLK_Source);

		HAL_Delay(2000);	
			
		printf("\r\n ���˳�ֹͣģʽ\r\n");
		//����ִ��whileѭ��

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
/**
  * @brief  ��ֹͣģʽ���Ѻ�����ϵͳʱ��:����HSE��PLL��ѡ��PLL��Ϊϵͳʱ��Դ��

  * @param  ��
  * @retval ��
  */
static void SYSCLKConfig_STOP(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	uint32_t pFLatency = 0;

	/* ���õ�Դ����ʱ�� */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* �����ڲ�RCC�Ĵ�����ȡ�������� */
	HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

	/* ��ֹͣģʽ���Ѻ���������ϵͳʱ��: ����HSE��PLL */
	RCC_OscInitStruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState        = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState    = RCC_PLL_ON;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	while(1) { ; }
	}

	/* �����ڲ�RCC�Ĵ�����ȡʱ������ */
	HAL_RCC_GetClockConfig(&RCC_ClkInitStruct, &pFLatency);

	/* ѡ�� PLL ��Ϊϵͳʱ��Դ, ������ HCLK��PCLK1 �� PCLK2ʱ�ӷ�Ƶϵ�� */
	RCC_ClkInitStruct.ClockType     = RCC_CLOCKTYPE_SYSCLK;
	RCC_ClkInitStruct.SYSCLKSource  = RCC_SYSCLKSOURCE_PLLCLK;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, pFLatency) != HAL_OK)
	{
	while(1) { ; }
	}
}
/*********************************************END OF FILE**********************/

