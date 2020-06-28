/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   Ethernet_UDP_Client
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
#include "lwip/init.h"
#include "app_ethernet.h"
#include "ethernetif.h"
#include "lwip/timeouts.h"
#include "./BSP/key/bsp_key.h"
#include "udp_echoclient.h"

KEY Key1,Key2;
extern __IO uint8_t EthLinkStatus;

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{ 
	/* Enable I-Cache */
	SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
	
    /* ����ϵͳʱ��Ϊ216 MHz */
    SystemClock_Config();

    /* ��ʼ��RGB�ʵ� */
    LED_GPIO_Config();

    /* ��ʼ��USART1 ����ģʽΪ 115200 8-N-1 */
    UARTx_Config();
	
    Key_GPIO_Config();
	KeyCreate(&Key1,GetPinStateOfKey1);
	KeyCreate(&Key2,GetPinStateOfKey2);
	
    /* ��ʼ��LwIPЭ��ջ*/
    lwip_init();
	
	printf("LAN8720A Ethernet Demo\n");
    printf("LwIP�汾��%s\n",LWIP_VERSION_STRING);

    /* ����ӿ����� */
    Netif_Config();
    /* �����û���������״̬ */
    User_notification(&gnetif);
#ifdef USE_DHCP
    printf("ʹ��DHCP�����ȡIP��ַ\r\n");
#else 
    printf("ʹ�þ�̬IP��ַ\r\n");
	//IP��ַ�Ͷ˿ڿ���main.h�ļ��޸ģ�����ʹ��DHCP�����Զ���ȡIP(��Ҫ·����֧��) 
	printf("����IP�Ͷ˿�: %d.%d.%d.%d:%d\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3,UDP_CLIENT_PORT);   
#endif
    
    printf("Զ��IP�Ͷ˿�: %d.%d.%d.%d:%d\n",DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2, DEST_IP_ADDR3,UDP_SERVER_PORT);

    printf("    KEY1: ����UDP����\n");
    printf("    KEY2: �Ͽ�UDP����\n");  
	while (1)
	{ 
		Key_RefreshState(&Key1);//ˢ�°���״̬
		Key_RefreshState(&Key2);//ˢ�°���״̬
		if(Key_AccessTimes(&Key1,KEY_ACCESS_READ)!=0)//���������¹�
		{
			LED2_ON;
			if (EthLinkStatus == 0)
			{
                printf("Connect to udp server\n");
				/* Connect to udp server */ 
				udp_echoclient_connect();
			}
			Key_AccessTimes(&Key1,KEY_ACCESS_WRITE_CLEAR);
		}
		if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0)//���������¹�
		{
			LED2_OFF;
			udp_echoclient_disconnect();
			Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
		}  		
        /* ����̫���������ж�ȡ���ݰ�������LwIP ���� */
        ethernetif_input(&gnetif);
        /* ���� LwIP ��ʱ */
        sys_check_timeouts();
		
#ifdef USE_DHCP
        /* ����DHCP�����ڶ�ʱ�� */
        DHCP_Periodic_Handle(&gnetif);
#endif 		
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
void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/

