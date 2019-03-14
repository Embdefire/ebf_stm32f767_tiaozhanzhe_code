/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ʹ��USART1���ڣ��жϽ��ղ��ԣ���ʾ�������ݵ�buff�������м������
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

static void Show_Message(void);
volatile uint8_t Rxflag=0;
uint8_t ucTemp;
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    char ch;    
    /* ����ϵͳʱ��Ϊ216 MHz */
    SystemClock_Config();


    /* ��ʼ��RGB�ʵ� */
    LED_GPIO_Config();

    /* ��ʼ��USART1 ����ģʽΪ 115200 8-N-1 */
    UARTx_Config();
	
    /* ��ӡָ��������ʾ��Ϣ */
    Show_Message();
    while(1)
    {	
        /* ��ȡ�ַ�ָ�� */
        ch=getchar();
        printf("���յ��ַ���%c\n",ch);
				
        /* �����ַ�ָ�����RGB�ʵ���ɫ */
        switch(ch)
        {
          case '1':
            LED_RED;
          break;
          case '2':
            LED_GREEN;
          break;
          case '3':
            LED_BLUE;
          break;
          case '4':
            LED_YELLOW;
          break;
          case '5':
            LED_PURPLE;
          break;
          case '6':
            LED_CYAN;
          break;
          case '7':
            LED_WHITE;
          break;
          case '8':
            LED_RGBOFF;
          break;
          default:
            /* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
          Show_Message();
          break;      
        }   
    }	
}

/**
  * @brief  ��ӡָ��������ʾ��Ϣ
  * @param  ��
  * @retval ��
  */
static void Show_Message(void)
{
  printf("\r\n   ����һ��ͨ������ͨ��ָ�����RGB�ʵ�ʵ�� \n");
  printf("ʹ��  USART1  ����Ϊ��%d 8-N-1 \n",UARTx_BAUDRATE);
  printf("������ӵ�ָ������RGB�ʵ���ɫ��ָ���Ӧ���£�\n");
  printf("   ָ��   ------ �ʵ���ɫ \n");
  printf("     1    ------    �� \n");
  printf("     2    ------    �� \n");
  printf("     3    ------    �� \n");
  printf("     4    ------    �� \n");
  printf("     5    ------    �� \n");
  printf("     6    ------    �� \n");
  printf("     7    ------    �� \n");
  printf("     8    ------    �� \n");  
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

