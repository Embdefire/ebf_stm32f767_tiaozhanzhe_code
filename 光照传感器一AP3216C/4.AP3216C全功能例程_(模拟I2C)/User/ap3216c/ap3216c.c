#include "./AP3216C/AP3216C.h"
#include "./usart/bsp_debug_usart.h"
#include "./i2c/bsp_i2c.h"
#include "stm32f7xx_hal.h"


#define AP3216C_ERROR 		I2C_ERROR
#define AP3216C_INFO 		I2C_INFO
/**
  * @brief   д���ݵ�AP3216C�Ĵ���
  * @param   reg_add:�Ĵ�����ַ
	* @param	 reg_data:Ҫд�������
  * @retval  
  */
void AP3216C_WriteReg(uint8_t reg_add,uint8_t reg_dat)
{
	Sensor_write(reg_add, reg_dat);
}

/**
  * @brief   ��AP3216C�Ĵ�����ȡ����
  * @param   reg_add:�Ĵ�����ַ
	* @param	 Read���洢���ݵĻ�����
	* @param	 num��Ҫ��ȡ��������
  * @retval  
  */
void AP3216C_ReadData(uint8_t reg_add,unsigned char* Read,uint8_t num)
{
	Sensor_Read(reg_add, Read, num);
}

/**
  * @brief   ��λAP3216C������
  * @param   
  * @retval  
  */
void AP3216C_Reset(void)
{
  AP3216C_WriteReg(AP3216C_SYS_CONFIG, AP3216C_MODE_SW_RESET);
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
  
  /* ������ǿ�ȵķ�Χ */
  AP3216C_ReadData(AP3216C_ALS_CONFIG, &resolution, 1);
  if((resolution >> 4) == AP3216C_ALS_RANGE_20661)
  {
    DR = 0.36;
  }
  else if((resolution >> 4) == AP3216C_ALS_RANGE_5162)
  {
    DR = 0.089;
  }
  else if((resolution >> 4) == AP3216C_ALS_RANGE_1291)
  {
    DR = 0.022;
  }
  else if((resolution >> 4) == AP3216C_ALS_RANGE_323)
  {
    DR = 0.0056;
  }
  
  low_threshold = (uint16_t)low_threshold / DR;
  high_threshold = (uint16_t)high_threshold / DR;
  
  AP3216C_WriteReg(AP3216C_ALS_LOW_THRESHOLD7_0, (low_threshold & 0xff));
  AP3216C_WriteReg(AP3216C_ALS_LOW_THRESHOLD15_8, low_threshold >> 8);
  AP3216C_WriteReg(AP3216C_ALS_HIGH_THRESHOLD7_0, (high_threshold & 0xff));
  AP3216C_WriteReg(AP3216C_ALS_HIGH_THRESHOLD15_8, high_threshold >> 8);
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
static void AP3216C_INT_Config(void)
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
  AP3216C_Reset();//��λ
  HAL_Delay(10);//��λ��һ��Ҫ��ʱ10ms�����ϣ���������
  AP3216C_WriteReg(AP3216C_SYS_CONFIG, AP3216C_MODE_ALS_AND_PS);//�������й���
//  Delay(100);
  AP3216C_Set_ALS_Threshold(10, 1000);//����������10������1000�����޴����ж�
  AP3216C_Set_PS_Threshold(200, 400);//�ӽ�ֵ����200������400�����޴����ж�
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
  float ALS_Data = 0.0;
  
  AP3216C_ReadData(AP3216C_ALS_DATA_LOW, buf, 2);
  ALS_RAW = (buf[1] << 8) | buf[0];
    
  AP3216C_ReadData(AP3216C_ALS_CONFIG, &temp, 1);
  if((temp >> 4) == AP3216C_ALS_RANGE_20661)
  {
    ALS_Data = ALS_RAW * 0.36;
  }
  else if((temp >> 4) == AP3216C_ALS_RANGE_5162)
  {
    ALS_Data = ALS_RAW * 0.089;
  }
  else if((temp >> 4) == AP3216C_ALS_RANGE_1291)
  {
    ALS_Data = ALS_RAW * 0.022;
  }
  else if((temp >> 4) == AP3216C_ALS_RANGE_323)
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
  uint16_t PS_Data;
  uint16_t proximity = 0;
  
  AP3216C_ReadData(AP3216C_PS_DATA_LOW, buf, 2);
  PS_Data = (buf[1] << 8) + buf[0];
  
  if(1 == ((PS_Data >> 6) & 0x01 || (PS_Data >> 14) & 0x01))
  {
    return PS_Data = 55555;//����̫ǿʱ�ӽ���������Ч������55555
  }
  else
  {
    proximity = (PS_Data & 0x000f) + (((PS_Data >> 8) & 0x3f) << 4);
    proximity |= PS_Data & 0x8000;//���λ��ʾ�����λ�ã�0��ʾԶ�룬1��ʾ�ӽ�
    
    return proximity;
  }
//  return PS_Data;
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
  IR_Data = (buf[1] << 8) + buf[0];
  IR_Data = (IR_Data & 0x0003) + ((IR_Data >> 8) & 0xFF);
  
  return IR_Data;
}
