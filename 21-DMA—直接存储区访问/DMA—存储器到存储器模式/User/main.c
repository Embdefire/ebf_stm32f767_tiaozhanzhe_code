/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   DMA���洢�����洢��ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F767 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f7xx.h"
#include "./led/bsp_led.h"


/* ��غ궨�壬ʹ�ô洢�����洢���������ʹ��DMA2 */
DMA_HandleTypeDef DMA_Handle;

#define DMA_STREAM               DMA2_Stream0
#define DMA_CHANNEL              DMA_CHANNEL_0
#define DMA_STREAM_CLOCK()       __DMA2_CLK_ENABLE()

#define BUFFER_SIZE              32

/* ����aSRC_Const_Buffer������ΪDMA��������Դ
  const�ؼ��ֽ�aSRC_Const_Buffer�����������Ϊ�������� */
const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
/* ����DMA����Ŀ��洢�� */
uint32_t aDST_Buffer[BUFFER_SIZE];
                                                                       
static void DMA_Config(void);                                    
static void Delay(__IO uint32_t nCount);
static void SystemClock_Config(void); 
uint8_t Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength); 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* �����űȽϽ������ */
	uint8_t TransferStatus;
	/* ϵͳʱ�ӳ�ʼ����216 MHz */
	SystemClock_Config();
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();
	/* ����RGB��ɫ��Ϊ��ɫ */
	LED_PURPLE;  

	/* ����ʱ���� */
	Delay(0xFFFFFF);  

	/* DMA�������� */
	DMA_Config(); 
  
	/* �ȴ�DMA������� */
	while(__HAL_DMA_GET_FLAG(&DMA_Handle,DMA_FLAG_TCIF0_4)==DISABLE)
	{

	}   

	/* �Ƚ�Դ�����봫������� */
	TransferStatus=Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);

	/* �ж�Դ�����봫������ݱȽϽ��*/
	if(TransferStatus==0)  
	{
		/* Դ�����봫������ݲ����ʱRGB��ɫ����ʾ��ɫ */
		LED_RED;
	}
	else
	{ 
		/* Դ�����봫����������ʱRGB��ɫ����ʾ��ɫ */
		LED_BLUE;
	}

	while (1)
	{		
	}
}

/* �򵥵���ʱ���� */
static void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

/**
  * DMA��������
  */
static void DMA_Config(void)
{
	HAL_StatusTypeDef DMA_status = HAL_ERROR; 
	/* ʹ��DMAʱ�� */
	DMA_STREAM_CLOCK();

	DMA_Handle.Instance = DMA_STREAM;
	/* DMA������ͨ��ѡ�� */
	DMA_Handle.Init .Channel = DMA_CHANNEL;  
	/* �洢�����洢��ģʽ */
	DMA_Handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
	/* ʹ���Զ��������� */
	DMA_Handle.Init.PeriphInc = DMA_PINC_ENABLE;
	/* ʹ���Զ��������� */
	DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;
	/* Դ�������ִ�С(32λ) */
	DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	/* Ŀ������Ҳ���ִ�С(32λ) */
	DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	/* һ�δ���ģʽ���洢�����洢��ģʽ����ʹ��ѭ������ */
	DMA_Handle.Init.Mode = DMA_NORMAL;
	/* DMA���������ȼ�Ϊ�� */
	DMA_Handle.Init.Priority = DMA_PRIORITY_HIGH;
	/* ����FIFOģʽ */
	DMA_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;     
	DMA_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	/* ����ģʽ */
	DMA_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
	/* ����ģʽ */
	DMA_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	/* ���DMA�������������� */
	HAL_DMA_Init(&DMA_Handle);

	DMA_status = HAL_DMA_Start(&DMA_Handle,(uint32_t)aSRC_Const_Buffer,(uint32_t)aDST_Buffer,BUFFER_SIZE);
	/* �ж�DMA״̬ */
	if (DMA_status != HAL_OK)
	{
		/* DMA������ó�����������ѭ����RGB��ɫ����˸ */
		while (1)
		{      
			LED_RED;
			Delay(0xFFFFFF);
			LED_RGBOFF;
			Delay(0xFFFFFF);
		}
	} 
}

/**
  * �ж�ָ�����ȵ���������Դ�Ƿ���ȫ��ȣ�
  * �����ȫ��ȷ���1��ֻҪ����һ�����ݲ���ȷ���0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
	/* ���ݳ��ȵݼ� */
	while(BufferLength--)
	{
		/* �ж���������Դ�Ƿ��Ӧ��� */
		if(*pBuffer != *pBuffer1)
		{
		  /* ��Ӧ����Դ����������˳�������������0 */
		  return 0;
		}
		/* ������������Դ�ĵ�ַָ�� */
		pBuffer++;
		pBuffer1++;
	}
	/* ����жϲ��Ҷ�Ӧ������� */
	return 1;  
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

