/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   �����������ӿ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F746 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./beep/bsp_beep.h"   
#include "main.h"

extern uint8_t beep_on_flag;
 /**
  * @brief  ��ʼ�����Ʒ�������IO
  * @param  ��
  * @retval ��
  */
void BEEP_GPIO_Config(void)
{		
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*�������Ʒ�������GPIO�Ķ˿�ʱ��*/
    BEEP_GPIO_CLK_ENABLE();   

    /*ѡ��Ҫ���Ʒ�������GPIO*/															   
    GPIO_InitStructure.Pin = BEEP_GPIO_PIN;	

    /*����GPIOģʽΪͨ���������*/
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;   

    GPIO_InitStructure.Pull = GPIO_PULLDOWN;

    /*����GPIO����Ϊ50MHz */   
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; 

    /*���ÿ⺯������ʼ�����Ʒ�������GPIO*/
    HAL_GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);			 

    /*  �رշ�����*/
    HAL_GPIO_WritePin(BEEP_GPIO_PORT, BEEP_GPIO_PIN,GPIO_PIN_RESET);	 
}
 /**
  * @brief  ���Ʒ����������
  * @param  times ��Ĵ���������ѡ��1��2,3��
  * @retval ��
  */
void BEEP_Handler(uint8_t times)
{
	static uint8_t beepstep=1;
	//���յ���������ı�־����
	if(beep_on_flag && (!Task_Delay[1]))
	{
		//���ݷ��������Ԥ��������жϲ�����������Ĳ���
		if(beepstep>2*times)
			beepstep =0;
		switch(beepstep)
		{
			case 1:  
					BEEP_ON;
					Task_Delay[1] = 50;
					beepstep = 2;
			break ;
			case 2:  
					BEEP_OFF;
					Task_Delay[1] = 30;
					beepstep = 3;
			break ;
			case 3:  
					BEEP_ON;
					Task_Delay[1] = 50;
					beepstep = 4;
			break ;
			case 4:  
					BEEP_OFF;
					Task_Delay[1] = 30;
					beepstep = 5;
			break ;
			case 5:  
					BEEP_ON;
					Task_Delay[1] = 50;
					beepstep = 6;
			break ;
			case 6:  
					BEEP_OFF;
					beepstep = 7;
			break ;			
			default :
					beepstep = 1;				
					beep_on_flag = 0;
		}	
	}
	
}
