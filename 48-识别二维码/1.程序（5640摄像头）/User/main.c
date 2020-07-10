/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   DCMI��OV5640����ͷʵ��
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
#include "./i2c/bsp_i2c.h"
#include "./lcd/bsp_lcd.h"
#include "./camera/bsp_ov5640.h"
#include "./camera/ov5640_AF.h"
#include "./QR_Decoder/qr_decoder_user.h"
#include "./beep/bsp_beep.h"   

/*���������*/
uint32_t Task_Delay[NumOfTask]={0};
uint8_t dispBuf[100];
uint8_t fps=0;

uint8_t beep_on_flag = 0;//������״̬��1��ʾ�������죬0��ʾ����������
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    char  qr_type_len=0;
    short qr_data_len=0;
    char qr_type_buf[10];
    char qr_data_buf[512];
    int addr=0;
    int i=0,j=0;
    char qr_num=0;
	uint32_t tickstart = 0;
	uint32_t tickstop = 0;
	
	OV5640_IDTypeDef OV5640_Camera_ID;
	
    /* ϵͳʱ�ӳ�ʼ����216 MHz */
    SystemClock_Config();
    /* LED �˿ڳ�ʼ�� */
    LED_GPIO_Config();	
	/* �������˿ڳ�ʼ�� */
	BEEP_GPIO_Config();
	/*��ʼ��USART1*/
	DEBUG_USART_Config(); 
    /* LCD �˿ڳ�ʼ�� */ 
    LCD_Init();
    /* LCD ��һ���ʼ�� */ 
    LCD_LayerInit(0, LCD_FB_START_ADDRESS,RGB565);
	/* LCD �ڶ����ʼ�� */ 
    LCD_LayerInit(1, LCD_FB_START_ADDRESS+(LCD_GetXSize()*LCD_GetYSize()*4),ARGB8888);

    /* ѡ��LCD��һ�� */
    LCD_SelectLayer(0);

    /* ��һ����������ʾ��ɫ */ 
    LCD_Clear(LCD_COLOR_BLACK);  
	
	/* ѡ��LCD�ڶ��� */
    LCD_SelectLayer(1);
    /* �ڶ�����������ʾȫ�� */ 
    LCD_Clear(LCD_COLOR_TRANSPARENT);
    /* ���õ�һ�͵ڶ����͸����,��СֵΪ0�����ֵΪ255*/
    LCD_SetTransparency(0, 255);
    LCD_SetTransparency(1, 200);
		
    LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_TRANSPARENT);
    LCD_DisplayStringLine_EN_CH(1,(uint8_t* )" ģʽ:UXGA 800x480");
    CAMERA_DEBUG("STM32F767 DCMI ����OV5640����");

	OV5640_HW_Init();			
	//��ʼ�� I2C
	I2CMaster_Init(); 
    /* ʹ��LCD������������ */ 
    LCD_DisplayOn(); 
	/* ��ȡ����ͷоƬID��ȷ������ͷ�������� */
	OV5640_ReadID(&OV5640_Camera_ID);

	if(OV5640_Camera_ID.PIDH  == 0x56)
	{
		CAMERA_DEBUG("%x%x",OV5640_Camera_ID.PIDH ,OV5640_Camera_ID.PIDL);
	}
	else
	{
		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_TRANSPARENT);
		LCD_DisplayStringLine_EN_CH(8,(uint8_t*) "         û�м�⵽OV5640�������¼�����ӡ�");
		CAMERA_DEBUG("û�м�⵽OV5640����ͷ�������¼�����ӡ�");
		while(1);  
	}
	LCD_Open_QR_Window();
    /* ��������ͷ������ظ�ʽ */
	OV5640_RGB565Config();	
	/* ��ʼ������ͷ��������ʾͼ�� */
	OV5640_Init();
    OV5640_AUTO_FOCUS();
	//����
    fps =0;
	Task_Delay[0]=1000;
	
	while(1)
	{
		//��ά��ʶ�𣬷���ʶ������ĸ���
		qr_num = QR_decoder();
		if(qr_num)
		{
		   //ʶ��ɹ������������־
		   beep_on_flag =1;		   
		   //����������ǰ���ʶ������ĸ�����װ�õĶ�ά���飬��Щ������Ҫ
		   //����ʶ������ĸ�������������ͨ�����ڷ��͵���λ�������ն�
		   for(i=0;i < qr_num;i++)
		   {
			   qr_type_len = decoded_buf[i][addr++];//��ȡ�������ͳ���
			   
			   for(j=0;j < qr_type_len;j++)
				   qr_type_buf[j]=decoded_buf[i][addr++];//��ȡ������������
			   
			   qr_data_len  = decoded_buf[i][addr++]<<8; //��ȡ�������ݳ��ȸ�8λ
			   qr_data_len |= decoded_buf[i][addr++];    //��ȡ�������ݳ��ȵ�8λ
			   
			   for(j=0;j < qr_data_len;j++)
				   qr_data_buf[j]=decoded_buf[i][addr++];//��ȡ��������
			   
			   uart_send_buf((unsigned char *)qr_type_buf, qr_type_len);//���ڷ��ͽ�������
			   while(get_send_sta()); //�ȴ����ڷ������
			   uart_send_buf((unsigned char *)":", 1);    //���ڷ��ͷָ���
			   while(get_send_sta()); //�ȴ����ڷ������
			   uart_send_buf((unsigned char *)qr_data_buf, qr_data_len);//���ڷ��ͽ�������
			   while(get_send_sta()); //�ȴ����ڷ������
			   uart_send_buf((unsigned char *)"\r\n", 2); //���ڷ��ͷָ���
			   while(get_send_sta());//�ȴ����ڷ������
			   addr =0;//����
		   }
			   
		} 
        if(Task_Delay[0]==0)
        {
			tickstop = HAL_GetTick() - tickstart;                
            sprintf((char*)dispBuf, " ֡��:%d FPS", fps*1000/tickstop);			
            /*���֡��*/ 
			LCD_SelectLayer(1);
			LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_TRANSPARENT);
			LCD_ClearLine(2);
            LCD_DisplayStringLine_EN_CH(2,dispBuf);
            //����
            fps =0;

            Task_Delay[0]=1000; //��ֵÿ1ms���1������0�ſ������½�������
			tickstart = HAL_GetTick();            
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

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}  
}

/*********************************************END OF FILE**********************/

