/**
  ******************************************************************************
  * @file    bsp_advance_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �߼����ƶ�ʱ�������������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F767 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim/bsp_lptim.h"

LPTIM_HandleTypeDef  LPTIM_Handle;
/**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void LPTIM_GPIO_Config(void) 
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*������ʱ����ص�GPIO����ʱ��*/
	LPTIM1_OUT_GPIO_CLK_ENABLE();

	/* ��ʱ���������ų�ʼ�� */															   
	GPIO_InitStructure.Pin = LPTIM1_OUT_PIN;	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;    
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW; 	
	GPIO_InitStructure.Alternate = LPTIM1_OUT_AF;
	HAL_GPIO_Init(LPTIM1_OUT_GPIO_PORT, &GPIO_InitStructure);	
}

/**
  * @brief  ����TIM���PWM
  * @param  ��
  * @retval ��
  */
static void LPTIM_Mode_Config(void)
{
	RCC_PeriphCLKInitTypeDef        RCC_PeriphCLKInitStruct;	
	uint32_t PeriodValue;
	uint32_t PulseValue;

	/* ѡ��LSEʱ����ΪLPTIMʱ��Դ */
	RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
	RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;  
	HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);	
	// ����LPTIM1ʱ��
	__LPTIM1_CLK_ENABLE(); 
	/* ���嶨ʱ���ľ����ȷ����ʱ���Ĵ����Ļ���ַ*/
	LPTIM_Handle.Instance = LPTIM1;
	// �߼����ƶ�ʱ��ʱ��ԴLPTIM_CLK = LSE=32.768KHz 
	LPTIM_Handle.Init.Clock.Source    = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
	// ��ʱ��ʱ�ӷ�Ƶϵ��
	LPTIM_Handle.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;  	
	// ��ʱ������Դ���ڲ�
	LPTIM_Handle.Init.CounterSource   = LPTIM_COUNTERSOURCE_INTERNAL;
	// ����Դ���������
	LPTIM_Handle.Init.Trigger.Source  = LPTIM_TRIGSOURCE_SOFTWARE; 
	// ��ʱ���������
	LPTIM_Handle.Init.OutputPolarity  = LPTIM_OUTPUTPOLARITY_HIGH;
	// ��ʱ�����·�ʽ
	LPTIM_Handle.Init.UpdateMode      = LPTIM_UPDATE_IMMEDIATE;
	// ��ʼ����ʱ��LPTIM
	HAL_LPTIM_Init(&LPTIM_Handle);

	/*PWMģʽ����*/
	/*����ʱ����0������99����Ϊ100�Σ�Ϊһ����ʱ����PWM���ڣ�32.768KHz/100 = 327.68Hz*/
	PeriodValue = 100-1;
	/*PWM����Ϊ����һ�뼴50% */
	PulseValue = 50-1;
	HAL_LPTIM_PWM_Start(&LPTIM_Handle, PeriodValue, PulseValue);
}

/**
  * @brief  �͹��Ķ�ʱ���ڵ͹���ģʽ���PWM
  * @param  ��
  * @retval ��
  */
void LPTIM_PWM_OUT(void)
{
	LPTIM_GPIO_Config();	
	
	LPTIM_Mode_Config();
	/* ����͹���ģʽ */
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	/* �ȴ�ϵͳ������KEY2���ѣ��˳��͹���ģʽ��ֹͣ���PWM */
    HAL_LPTIM_PWM_Stop(&LPTIM_Handle);
}

/*********************************************END OF FILE**********************/
