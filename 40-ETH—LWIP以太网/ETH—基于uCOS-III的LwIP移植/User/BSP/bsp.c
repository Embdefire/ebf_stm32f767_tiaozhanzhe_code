/*
  ******************************************************************************
  * @file    bsp.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ϵͳ��ʼ�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F767 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
*/


/*
*********************************************************************************************************
*                                             �������ļ�
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  "os.h"
#include  "bsp.h"
#include  "main.h"
#include "./led/bsp_led.h" 
#include  "stm32f7xx_hal.h"
#include  "stm32f7xx_hal.h"
#include "./usart/bsp_debug_usart.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "app_ethernet.h"
#include "ethernetif.h"

static  OS_TCB       AppTaskDHCPTCB;
static  CPU_STK      AppTaskDHCPStk[APP_TASK_DHCP_STK_SIZE];
struct netif gnetif;
/*
*********************************************************************************************************
* ������ : BSP_Init
* ����   : ���е�Ӳ���豸��Ӧ�÷������������߳�ʼ��
* �β�   : ��
* ����ֵ : ��
*********************************************************************************************************
*/
void  BSP_Init (void)
{
    OS_ERR  err;	
	BSP_OSTickInit();                  //��ʼ�� OS ʱ��Դ
    LED_GPIO_Config();                 //��ʼ��LED
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	DEBUG_USART_Config();
	
	printf("����uCOS-III��LwIP����ͨ�Ų���\n");
	printf("LwIP�汾��%s\n",LWIP_VERSION_STRING);
	/* ����TCPIP��ջ�߳� */
	tcpip_init( NULL, NULL );
    /* ����ӿ����� */
    Netif_Config(&gnetif);	
	/* �����û���������״̬ */
    User_notification(&gnetif);
	printf("LAN8720A��ʼ���ɹ�\n"); 
#ifdef USE_DHCP
	/* ����DHCP�ͻ��� */
	OSTaskCreate((OS_TCB     *)&AppTaskDHCPTCB,                            //������ƿ��ַ
				(CPU_CHAR   *)"DHCP",                            		   //��������
				(OS_TASK_PTR ) LwIP_DHCP_task,                             //������
				(void       *) &gnetif,                                    //���ݸ����������β�p_arg����ʵ��
				(OS_PRIO     ) APP_CFG_TASK_DHCP_PRIO,                     //��������ȼ�
				(CPU_STK    *)&AppTaskDHCPStk[0],                          //�����ջ�Ļ���ַ
				(CPU_STK_SIZE) APP_TASK_DHCP_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
				(CPU_STK_SIZE) APP_TASK_DHCP_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
				(OS_MSG_QTY  ) 0u,                                         //����ɽ��յ������Ϣ��
				(OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
				(void       *) 0,                                          //������չ��0����չ��
				(OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
				(OS_ERR     *)&err);                                       //���ش�������
#endif  //#ifdef USE_DHCP		
}
/*
*********************************************************************************************************                               
* ������ : BSP_SystemClkCfg
* ����   : ϵͳʱ�ӳ�ʼ��
* �β�   : ��
* ����ֵ : ��
*********************************************************************************************************
*/

void  BSP_SystemClkCfg (void)
{
    RCC_OscInitTypeDef  RCC_OscInit;
    RCC_ClkInitTypeDef  RCC_ClkInit;
    HAL_StatusTypeDef   hal_status;
                                                      
    RCC_OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;            
    RCC_OscInit.HSEState       = RCC_HSE_ON;
    RCC_OscInit.HSIState       = RCC_HSI_OFF;
    RCC_OscInit.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInit.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInit.PLL.PLLM       = 25u;                              //�ⲿ����Ƶ�� HSE freq     = 25MHz
    RCC_OscInit.PLL.PLLN       = 432u;                             //��Ƶʱ��Ƶ�� VCO out-freq = HSE * (PLLN / PLLM)     = 432MHz
    RCC_OscInit.PLL.PLLP       = RCC_PLLP_DIV2;                    //ϵͳʱ��Ƶ�� PLLCLK       = (VCO out-freq) / PLLP   = 216MHz
    RCC_OscInit.PLL.PLLQ       = 9;                                //����ʱ��Ƶ�� PLL_Q out freq = (VCO out-freq) / PLLQ = 48MHz

    hal_status = HAL_RCC_OscConfig(&RCC_OscInit);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                         //���������ֹͣ
            ;
        }
    }

    hal_status = HAL_PWREx_EnableOverDrive();                      //�ȴ�ʱ��Ƶ���ȶ�Ϊ 216 Mhz
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                         //���������ֹͣ
            ;
        }
    }

    RCC_ClkInit.ClockType      = RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_HCLK   |
                                 RCC_CLOCKTYPE_PCLK1  |
                                 RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInit.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInit.AHBCLKDivider  = RCC_SYSCLK_DIV1;                   //AHBʱ��Ƶ��  HCLK= AHBCLK    = PLLCLK / AHBPRES(1) = 216MHz
    RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;                     //APB1��������ʱ��Ƶ�� APB1CLK = AHBCLK  / APB1DIV(4)=  54MHz
    RCC_ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;                     //APB2��������ʱ��Ƶ�� APB2CLK = AHBCLK  / APB2DIV(2)= 108MHz

    hal_status = HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_7);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                          //���������ֹͣ
            ;
        }
    }
}


/*
*********************************************************************************************************                               
* ������ : BSP_ClkFreqGet
* ����   : ���������������ϵͳʱ��Ƶ��
* �β�   : clk_id    ϵͳʱ�ӱ�ʶ��
*                    BSP_CLK_ID_SYSCLK     ϵͳʱ��Ƶ�ʡ�
*                    BSP_CLK_ID_HCLK       CPUʱ��Ƶ�ʡ�
*                    BSP_CLK_ID_PCLK1      APB1����ʱ��Ƶ�ʡ�
*                    BSP_CLK_ID_PCLK2      APB2����ʱ��Ƶ�ʡ�
* ����ֵ : ��
*********************************************************************************************************
*/
CPU_INT32U  BSP_ClkFreqGet (BSP_CLK_ID  clk_id)
{
    CPU_INT32U  clk_freq;


    switch (clk_id) {
        case BSP_CLK_ID_SYSCLK:
             clk_freq = HAL_RCC_GetSysClockFreq();
             break;


        case BSP_CLK_ID_HCLK:
             clk_freq = HAL_RCC_GetHCLKFreq();
             break;


        case BSP_CLK_ID_PCLK1:
             clk_freq = HAL_RCC_GetPCLK1Freq();
             break;


        case BSP_CLK_ID_PCLK2:
             clk_freq = HAL_RCC_GetPCLK2Freq();
             break;


        default:
             clk_freq = 1u;                                     //û����Чʱ��Ƶ��
             break;
    }

    return (clk_freq);
}
/*
*********************************************************************************************************                               
* ������ : BSP_OSTickInit
* ����   : ��ʼ�� OS ���ʱ���ж�
* �β�   : ��
* ����ֵ : ��
*********************************************************************************************************
*/
void  BSP_OSTickInit (void)
{
    CPU_INT32U  cpu_clk_freq;


    cpu_clk_freq = BSP_ClkFreqGet(BSP_CLK_ID_HCLK);             //��ȡCPUʱ�ӣ�ʱ������Ը�ʱ�Ӽ���

    OS_CPU_SysTickInitFreq(cpu_clk_freq);                       //��ʼ��uC/OS ����ʱ��Դ (SysTick)
}

/*
*********************************************************************************************************                               
* ������ : HAL_InitTick
* ����   : ����STM32F7xx HAL ���е�HAL_InitTick��������ΪMicriumʵʱϵͳ���Լ���Systick ��ʼ����
*          �����ڶ�����������ų�ʼ���δ�ʱ��
* �β�   : TickPriority     �δ��ж����ȼ�
* ����ֵ : ��
*********************************************************************************************************
*/
HAL_StatusTypeDef  HAL_InitTick (uint32_t  TickPriority)
{
    HAL_NVIC_SetPriorityGrouping(0);

    return (HAL_OK);
}

/*
*********************************************************************************************************                               
* ������ : HAL_GetTick
* ����   : ����STM32F7xx HAL HAL_GetTick��������ΪMicriumʵʱϵͳ���Լ��ĵδ��ʱ����ֵ
* �β�   : ��
* ����ֵ : �δ��ʱ����ֵ
* ע��   ����ȷ���δ�ʱ�������Ӧ�ó�������������и��ߵ����ȼ�
*********************************************************************************************************
*/
uint32_t  HAL_GetTick(void)
{
    CPU_INT32U  os_tick_ctr;
#if (OS_VERSION >= 30000u)
    OS_ERR      os_err;


    os_tick_ctr = OSTimeGet(&os_err);
#else
    os_tick_ctr = OSTimeGet();
#endif

    return os_tick_ctr;
}

