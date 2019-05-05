/**
  ******************************************************************************
  * @file    ap3216.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   AP3216C �����ļ�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32F767 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
#include "./AP3216C/ap3216c.h"
#include "./usart/bsp_debug_usart.h"
#include "./i2c/i2c.h"


#define AP3216C_ERROR 		I2C_ERROR
#define AP3216C_INFO 		I2C_INFO
/**
  * @brief   д���ݵ�AP3216C�Ĵ���
  * @param   reg_add:�Ĵ�����ַ
	* @param	 reg_data:Ҫд�������
  * @retval  
  */
static void AP3216C_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
	Sensors_I2C_WriteRegister(AP3216C_ADDRESS,reg_add,1,&reg_dat);
}

/**
  * @brief   ��AP3216C�Ĵ�����ȡ����
  * @param   reg_add:�Ĵ�����ַ
	* @param	 Read���洢���ݵĻ�����
	* @param	 num��Ҫ��ȡ��������
  * @retval  
  */
static void AP3216C_ReadData(uint8_t reg_add,unsigned char* Read,uint8_t num)
{
	Sensors_I2C_ReadRegister(AP3216C_ADDRESS,reg_add,num,Read);
}

/**
  * @brief   ��λAP3216C������
  * @param   
  * @retval  
  */
void AP3216C_Reset(void)
{
  AP3216C_WriteReg(AP3216C_SYS_CONFIG, AP3216C_SW_RST_BIT);
}

/**
  * @brief   ����AP3216C�����⴫��������������ֵ
  * @param   low_threshold��������ֵ
  * @param   high_threshold��������ֵ
  * @retval  
  */
void AP3216C_Set_ALS_Threshold(uint16_t low_threshold, uint16_t high_threshold)
{
  uint8_t resolution;
  double DR;
  
  AP3216C_ReadData(AP3216C_ALS_CONFIG, &resolution, 1);
  if((resolution >> 4) == AP3216C_ALS_RESOLUTION_036LUX_BIT)
  {
    DR = 0.36;
  }
  else if((resolution >> 4) == AP3216C_ALS_RESOLUTION_0089LUX_BIT)
  {
    DR = 0.089;
  }
  else if((resolution >> 4) == AP3216C_ALS_RESOLUTION_0022LUX_BIT)
  {
    DR = 0.022;
  }
  else if((resolution >> 4) == AP3216C_ALS_RESOLUTION_00056LUX_BIT)
  {
    DR = 0.0056;
  }
  
  high_threshold = (uint16_t)high_threshold / DR;
  low_threshold = (uint16_t)low_threshold / DR;
  
  AP3216C_WriteReg(AP3216C_ALS_LOW_THRESHOLD7_0, (low_threshold & 0xFF));
  AP3216C_WriteReg(AP3216C_ALS_LOW_THRESHOLD15_8, (low_threshold >> 8));
  AP3216C_WriteReg(AP3216C_ALS_HIGH_THRESHOLD7_0, (high_threshold & 0xFF));
  AP3216C_WriteReg(AP3216C_ALS_HIGH_THRESHOLD15_8, (high_threshold >> 8));
}

/**
  * @brief   ����AP3216C�ӽ�����������������ֵ
  * @param   low_threshold��������ֵ
  * @param   high_threshold��������ֵ
  * @retval  
  */
void AP3216C_Set_PS_Threshold(uint16_t low_threshold, uint16_t high_threshold)
{
  if(low_threshold > 1020)
  {
    AP3216C_WriteReg(AP3216C_PS_LOW_THRESHOLD2_0, (low_threshold - 1020 & 0x03));
  }
  AP3216C_WriteReg(AP3216C_PS_LOW_THRESHOLD10_3, (low_threshold / 4));
  
  if(high_threshold > 1020)
  {
    AP3216C_WriteReg(AP3216C_PS_HIGH_THRESHOLD2_0, (high_threshold - 1020 & 0x03));
  }
  AP3216C_WriteReg(AP3216C_PS_HIGH_THRESHOLD10_3, (high_threshold / 4));
}

/**
  * @brief   ����AP3216C�ж�����
  * @param   
  * @param   
  * @retval  
  */
void AP3216C_INT_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 

  /*��������GPIO�ڵ�ʱ��*/
  AP_INT_GPIO_CLK_ENABLE();

  /* ѡ�񰴼�1������ */ 
  GPIO_InitStructure.Pin = AP_INT_GPIO_PIN;
  /* ��������Ϊ����ģʽ */ 
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;	    		
  /* �������Ų�����Ҳ������ */
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  /* ʹ������Ľṹ���ʼ������ */
  HAL_GPIO_Init(AP_INT_GPIO_PORT, &GPIO_InitStructure); 
}

/**
  * @brief   ��ȡAP3216C���������ж�״̬
  * @param   
  * @retval  INTstatus����0λ��ʾALS�жϣ���1λ��ʾPS�ж�
  */
uint8_t AP3216C_Get_INTStatus(void)
{
  uint8_t INTstatus;
  
  AP3216C_ReadData(AP3216C_INT_STATUS, &INTstatus, 1);
  
  return INTstatus;
}

/**
  * @brief   ��ʼ��AP3216C������
  * @param   
  * @retval  
  */
void AP3216C_Init(void)
{
  AP3216C_WriteReg(AP3216C_SYS_CONFIG, 0x00);//�ر����й���
  AP3216C_WriteReg(AP3216C_SYS_CONFIG, AP3216C_SW_RST_BIT);//��λ
  Delay(10);//��λ��һ��Ҫ��ʱ10ms�����ϣ���������
  AP3216C_WriteReg(AP3216C_SYS_CONFIG, AP3216C_ALS_PS_IR_ACTIVE_BIT);//�������й���
//  Delay(100);
  AP3216C_Set_ALS_Threshold(100, 1000);//����������100������1000�����޴����ж�
  AP3216C_Set_PS_Threshold(120, 600);//�ӽ�ֵ����120������600�����޴����ж�
  AP3216C_INT_Config();
}

/**
  * @brief   ��ȡAP3216C�Ļ����⴫��������
  * @param   
  * @retval  
  */
float AP3216C_ReadALS(void)
{
  uint8_t temp, buf[2];
  uint16_t ALS_RAW;
  float ALS_Data;
  
  AP3216C_ReadData(AP3216C_ALS_DATA_LOW, buf, 2);
  ALS_RAW = (buf[1] << 8) | buf[0];
    
  AP3216C_ReadData(AP3216C_ALS_CONFIG, &temp, 1);
  if((temp >> 4) == AP3216C_ALS_RESOLUTION_036LUX_BIT)
  {
    ALS_Data = ALS_RAW * 0.36;
  }
  else if((temp >> 4) == AP3216C_ALS_RESOLUTION_0089LUX_BIT)
  {
    ALS_Data = ALS_RAW * 0.089;
  }
  else if((temp >> 4) == AP3216C_ALS_RESOLUTION_0022LUX_BIT)
  {
    ALS_Data = ALS_RAW * 0.022;
  }
  else if((temp >> 4) == AP3216C_ALS_RESOLUTION_00056LUX_BIT)
  {
    ALS_Data = ALS_RAW * 0.0056;
  }
  return ALS_Data;
}

/**
  * @brief   ��ȡAP3216C�Ľӽ�����������
  * @param   
  * @retval  
  */
uint16_t AP3216C_ReadPS(void)
{
  uint8_t buf[2];
  uint16_t proximity;
  
  AP3216C_ReadData(AP3216C_PS_DATA_LOW, buf, 2);
  
  if((buf[0] >> 6) || (buf[1] >> 6 ))
  {
    return proximity = 0xFFFF;//����̫ǿʱ�ӽ���������Ч������0xFFFF
  }
  else
  {
    buf[0] = buf[0] & 0x0F; // PS Data LOW 4 bits
    buf[1] = buf[1] & 0x3F; // PS Data HIGH 6 bits
    proximity = (buf[1] << 4) | buf[0];
    proximity |= buf[1] >> 7;//���λ��ʾ�����λ�ã�0��ʾԶ�룬1��ʾ����
    
    return proximity;
  }
}

/**
  * @brief   ��ȡAP3216C�ĺ���⴫��������
  * @param   
  * @retval  
  */
uint16_t AP3216C_ReadIR(void)
{
  uint8_t buf[2];
  uint16_t IR_Data;
  
  AP3216C_ReadData(AP3216C_IR_DATA_LOW, buf, 2);
  buf[0] = buf[0] & 0x03; // IR Data LOW 2 bits
  IR_Data = (buf[1] << 2) | buf[0];
  
  return IR_Data;
}
