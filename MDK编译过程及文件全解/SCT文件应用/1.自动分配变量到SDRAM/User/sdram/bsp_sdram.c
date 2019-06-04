/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   sdramӦ�ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32F767 ������  
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./sdram/bsp_sdram.h"  
#include "main.h"

/**
  * @brief  �ӳ�һ��ʱ��
  * @param  �ӳٵ�ʱ�䳤��
  * @retval None
  */
static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}

/**
  * @brief  ��ʼ������SDRAM��IO
  * @param  ��
  * @retval ��
  */
static void SDRAM_GPIO_Config(void)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ʹ��SDRAM��ص�GPIOʱ�� */

  /*��ַ�ź���*/
  FMC_A0_GPIO_CLK;  FMC_A1_GPIO_CLK;  FMC_A2_GPIO_CLK;
  FMC_A3_GPIO_CLK;FMC_A4_GPIO_CLK;FMC_A5_GPIO_CLK;
  FMC_A6_GPIO_CLK; FMC_A7_GPIO_CLK; FMC_A8_GPIO_CLK;
  FMC_A9_GPIO_CLK; FMC_A10_GPIO_CLK;FMC_A11_GPIO_CLK; 
  FMC_A12_GPIO_CLK;
  /*�����ź���*/
  FMC_D0_GPIO_CLK; FMC_D1_GPIO_CLK ; FMC_D2_GPIO_CLK ; 
  FMC_D3_GPIO_CLK ; FMC_D4_GPIO_CLK ; FMC_D5_GPIO_CLK ;
  FMC_D6_GPIO_CLK; FMC_D7_GPIO_CLK ; FMC_D8_GPIO_CLK ;
  FMC_D9_GPIO_CLK ;FMC_D10_GPIO_CLK; FMC_D11_GPIO_CLK;
  FMC_D12_GPIO_CLK; FMC_D13_GPIO_CLK; FMC_D14_GPIO_CLK;
  FMC_D15_GPIO_CLK;  
  /*�����ź���*/
  FMC_CS_GPIO_CLK ; FMC_BA0_GPIO_CLK; FMC_BA1_GPIO_CLK ;
  FMC_WE_GPIO_CLK ; FMC_RAS_GPIO_CLK ; FMC_CAS_GPIO_CLK;
  FMC_CLK_GPIO_CLK ; FMC_CKE_GPIO_CLK; FMC_UDQM_GPIO_CLK;
  FMC_LDQM_GPIO_CLK;

  
  /*-- GPIO ���� -----------------------------------------------------*/
  /* ͨ�� GPIO ���� */       
  GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;//����Ϊ���ù���
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStructure.Alternate = GPIO_AF12_FMC;
  
  /*��ַ�ź��� �����������*/
  GPIO_InitStructure.Pin = FMC_A0_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A0_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A1_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A1_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A2_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A2_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A3_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A3_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A4_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A4_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A5_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A5_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A6_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A6_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A7_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A7_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A8_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A8_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A9_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A9_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A10_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A10_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A11_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A11_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pin = FMC_A12_GPIO_PIN; 
  HAL_GPIO_Init(FMC_A12_GPIO_PORT, &GPIO_InitStructure);
  
  
  /*�����ź��� �����������*/
  GPIO_InitStructure.Pin = FMC_D0_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D0_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D1_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D1_GPIO_PORT, &GPIO_InitStructure);

    
  GPIO_InitStructure.Pin = FMC_D2_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D2_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D3_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D3_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D4_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D4_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D5_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D5_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D6_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D6_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D7_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D7_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D8_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D8_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D9_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D9_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D10_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D10_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D11_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D11_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D12_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D12_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D13_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D13_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D14_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D14_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_D15_GPIO_PIN; 
  HAL_GPIO_Init(FMC_D15_GPIO_PORT, &GPIO_InitStructure);

  
  /*�����ź���*/
  GPIO_InitStructure.Pin = FMC_CS_GPIO_PIN; 
  HAL_GPIO_Init(FMC_CS_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_BA0_GPIO_PIN; 
  HAL_GPIO_Init(FMC_BA0_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_BA1_GPIO_PIN;
  HAL_GPIO_Init(FMC_BA1_GPIO_PORT, &GPIO_InitStructure);

    
  GPIO_InitStructure.Pin = FMC_WE_GPIO_PIN; 
  HAL_GPIO_Init(FMC_WE_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_RAS_GPIO_PIN; 
  HAL_GPIO_Init(FMC_RAS_GPIO_PORT, &GPIO_InitStructure);

    
  GPIO_InitStructure.Pin = FMC_CAS_GPIO_PIN; 
  HAL_GPIO_Init(FMC_CAS_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_CLK_GPIO_PIN; 
  HAL_GPIO_Init(FMC_CLK_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_CKE_GPIO_PIN; 
  HAL_GPIO_Init(FMC_CKE_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_UDQM_GPIO_PIN; 
  HAL_GPIO_Init(FMC_UDQM_GPIO_PORT, &GPIO_InitStructure);

  
  GPIO_InitStructure.Pin = FMC_LDQM_GPIO_PIN; 
  HAL_GPIO_Init(FMC_LDQM_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  ��SDRAMоƬ���г�ʼ������
  * @param  None. 
  * @retval None.
  */
static void SDRAM_InitSequence(void)
{
  /* Step 1 --------------------------------------------------------------------*/
  /* ������������ṩ��SDRAM��ʱ�� */
	FMC_Bank5_6->SDCMR = 0x00000009;
  /* Step 2: ��ʱ100us */ 
  SDRAM_delay(1);	
	/* Step 3 --------------------------------------------------------------------*/
  /* ������������е�bankԤ��� */ 
  FMC_Bank5_6->SDCMR = 0x0000000A;    
  /* Step 4 --------------------------------------------------------------------*/
  /* ��������Զ�ˢ�� */   
  FMC_Bank5_6->SDCMR = 0x0000008B;  
  /* Step 5 --------------------------------------------------------------------*/
  /* ����sdram�Ĵ������� */
  FMC_Bank5_6->SDCMR = 0x0004600C;
  /* Step 6 --------------------------------------------------------------------*/
	FMC_Bank5_6->SDRTR |= (824<<1);
}


/**
  * @brief  ��ʼ������ʹ��SDRAM��FMC��GPIO�ӿڣ�
  *         ��������SDRAM��д����ǰ��Ҫ������
  * @param  None
  * @retval None
  */
void SDRAM_Init(void)
{
	//ʹ��HSE
  RCC->CR |= RCC_CR_HSEON;
  while(!(RCC->CR&(1<<17))){;}
	//ʹ��PLL
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR&(1<<25))){;}
	//ѡ��HSE��ΪPLLʱ��Դ
	RCC->PLLCFGR |= (1 << 22);
	//���÷�Ƶϵ�� PLLCLK = HSE/M*N/P = 25M / 25 *432 / 2 = 216M
  RCC->PLLCFGR |= (25 << 0);//PLLM
	RCC->PLLCFGR |= (432 << 6);//PLLN
	RCC->PLLCFGR |= (2 << 16);//PLLP
	RCC->PLLCFGR |= (9 << 24);//PLLQ
	/* ���� OverDrive ģʽ�Դﵽ216MƵ��  */ 
	HAL_PWREx_EnableOverDrive();

	/* ����FMC�ӿ���ص� GPIO*/
  SDRAM_GPIO_Config();
	/* ʹ�� FMC ʱ�� */
	__HAL_RCC_FMC_CLK_ENABLE();

	/** Perform the SDRAM1 memory initialization sequence
  */
	FMC_Bank5_6->SDCR[FMC_SDRAM_BANK1] = 0x000039D0;
	FMC_Bank5_6->SDCR[FMC_SDRAM_BANK2] = 0x000001D8;
	
	FMC_Bank5_6->SDTR[FMC_SDRAM_BANK1] = 0x0F1F7FFF;
	FMC_Bank5_6->SDTR[FMC_SDRAM_BANK2] = 0x01010471;
	
  /* FMC SDRAM device initialization sequence */
  SDRAM_InitSequence(); 
}

/**
  * @brief  �ԡ��֡�Ϊ��λ��sdramд������ 
  * @param  pBuffer: ָ�����ݵ�ָ�� 
  * @param  uwWriteAddress: Ҫд���SDRAM�ڲ���ַ
  * @param  uwBufferSize: Ҫд�����ݴ�С
  * @retval None.
  */
void SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwWriteAddress;

//  /* ��ֹд���� */
//  HAL_SDRAM_WriteProtection_Disable(&hsdram1);
	FMC_Bank5_6->SDCR[1] &= ~FMC_SDRAM_WRITE_PROTECTION_ENABLE;
//  /* ���SDRAM��־���ȴ���SDRAM���� */ 
//  while(HAL_SDRAM_GetState(&hsdram1) != RESET)
//  {
//  }

  /* ѭ��д������ */
  for (; uwBufferSize != 0; uwBufferSize--) 
  {
    /* �������ݵ�SDRAM */
    *(uint32_t *) (SDRAM_BANK_ADDR + write_pointer) = *pBuffer++;

    /* ��ַ����*/
    write_pointer += 4;
  }  
}

/**
  * @brief  ��SDRAM�ж�ȡ���� 
  * @param  pBuffer: ָ��洢���ݵ�buffer
  * @param  ReadAddress: Ҫ��ȡ���ݵĵ�ʮ
  * @param  uwBufferSize: Ҫ��ȡ�����ݴ�С
  * @retval None.
  */
void SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
  __IO uint32_t write_pointer = (uint32_t)uwReadAddress;
  
   
//  /* ���SDRAM��־���ȴ���SDRAM���� */  
//  while ( HAL_SDRAM_GetState(&hsdram1) != RESET)
//  {
//  }
  
  /*��ȡ���� */
  for(; uwBufferSize != 0x00; uwBufferSize--)
  {
   *pBuffer++ = *(__IO uint32_t *)(SDRAM_BANK_ADDR + write_pointer );
    
   /* ��ַ����*/
    write_pointer += 4;
  } 
}


/**
  * @brief  ����SDRAM�Ƿ����� 
  * @param  None
  * @retval ��������1���쳣����0
  */
uint8_t SDRAM_Test(void)
{
  /*д�����ݼ�����*/
  uint32_t counter=0;
  
  /* 8λ������ */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16λ������ */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  SDRAM_INFO("���ڼ��SDRAM����8λ��16λ�ķ�ʽ��дsdram...");

  /*��8λ��ʽ��д���ݣ���У��*/
  
  /* ��SDRAM����ȫ������Ϊ0 ��W9825G6KH_SIZE����8λΪ��λ�� */
  for (counter = 0x00; counter < W9825G6KH_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* ������SDRAMд������  8λ */
  for (counter = 0; counter < W9825G6KH_SIZE; counter++)
  {
    *(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<W9825G6KH_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(SDRAM_BANK_ADDR + counter);  //�Ӹõ�ַ��������
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SDRAM_ERROR("8λ���ݶ�д����");
      return 0;
    }
  }
	
  /*��16λ��ʽ��д���ݣ������*/
  
  /* ��SDRAM����ȫ������Ϊ0 */
  for (counter = 0x00; counter < W9825G6KH_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* ������SDRAMд������  16λ */
  for (counter = 0; counter < W9825G6KH_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (SDRAM_BANK_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* ��ȡ SDRAM ���ݲ����*/
  for(counter = 0; counter<W9825G6KH_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(SDRAM_BANK_ADDR + 2*counter);  //�Ӹõ�ַ��������
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SDRAM_ERROR("16λ���ݶ�д����");

      return 0;
    }
  }
  SDRAM_INFO("SDRAM��д����������"); 
  /*���������return 1 */
  return 1;
}


/*********************************************END OF FILE**********************/

