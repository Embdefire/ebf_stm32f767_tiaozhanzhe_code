/*
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ����uCOS-III��LwIP��ֲ
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
*                                            �������ļ�
*********************************************************************************************************
*/
#include  <stdio.h>
#include  <bsp.h>
#include  <app_cfg.h>
#include  <os_app_hooks.h>
#include  <stm32f7xx_hal.h>
#include "./led/bsp_led.h" 
#include "lwip/init.h"
#include "lwip/timeouts.h"
#include "main.h"

/*
*********************************************************************************************************
*                                         ������ƿ�TCB
*********************************************************************************************************
*/

static  OS_TCB       AppTaskStartTCB;
/*
*********************************************************************************************************
*                                            �����ջ
*********************************************************************************************************
*/
static  CPU_STK      AppTaskStartStk[APP_TASK_START_STK_SIZE];
/*
*********************************************************************************************************
*                                            ����ԭ��
*********************************************************************************************************
*/

static  void  AppTaskStart (void  *p_arg);
static void MPU_Config(void);
/*
*********************************************************************************************************
* ������ : main
* ����   : ��׼��C�������
* �β�   : ��
* ����ֵ : ��
* ע��   : 1) HAL��ʼ��:
*             a) ����FlashԤȡ��ָ������ݸ��ٻ��档
*             b) ����Systick�������жϡ�HAL_InitTick()�����Ѿ���ϵͳ��д��
*                ϵͳ���Լ���Systick��ʼ�������������ڶ���������֮���ʼ����
*********************************************************************************************************
*/
int main(void)
{
    OS_ERR   err;
    MPU_Config();

    HAL_Init();                                                             //HAL��ʼ��,��ע�� 1
	
	SCB_EnableICache();														//ʹ��ָ���
    
    SCB_EnableDCache();														//ʹ�����ݻ��� 
	
    BSP_SystemClkCfg();                                                     //��ʼ��CPUƵ��Ϊ 216Mhz

    CPU_Init();                                                             //��ʼ�� CPU �����ʱ��������ж�ʱ���������������

    Mem_Init();                                                             //��ʼ���ڴ������������ڴ�غ��ڴ�ر�

    CPU_IntDis();                                                           //��ֹ�����ж�

    OSInit(&err);                                                           //��ʼ��uC/OS-IIIϵͳ
    App_OS_SetAllHooks();

	/* ������ʼ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Start",                            //��������
                 (OS_TASK_PTR ) AppTaskStart,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&AppTaskStartStk[0],                         //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 0u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������

    OSStart(&err);                                                          //�����������������uC/OS-III���ƣ�

}

/*
*********************************************************************************************************
* ������ ��AppTaskStart
* ����   : ����һ�����������ڶ�����ϵͳ�����󣬱����ʼ���δ������(�� BSP_Init ��ʵ��)��
* �β�   : p_arg   ��OSTaskCreate()�ڴ���������ʱ���ݹ������βΡ�
* ����ֵ : ��
* ע��   : 1) ��һ�д��� (void)p_arg; ��Ϊ�˷�ֹ������������Ϊ�β�p_arg��û���õ�
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR  err;

   (void)p_arg;

    BSP_Init();                                    //�弶��ʼ��

#if OS_CFG_STAT_TASK_EN > 0u                       //���ʹ�ܣ�Ĭ��ʹ�ܣ���ͳ������
    OSStatTaskCPUUsageInit(&err);                  //����û��Ӧ������ֻ�п�����������ʱ CPU �ģ����
#endif                                             //���������� OS_Stat_IdleCtrMax ��ֵ��Ϊ������� CPU
                                                   //ʹ����ʹ�ã���
#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                   //��λ�����㣩��ǰ�����ж�ʱ��
#endif
	
    while (DEF_TRUE)                               //�����壬ͨ����д��һ����ѭ��
    {                                          
        LED4_TOGGLE;                               // LED��ÿ��500ms��˸һ��
        OSTimeDly ( 500,OS_OPT_TIME_DLY,&err);
    }
}
/**
  * @brief  ��MPU��������Ϊ SRAM1 �е���̫���������豸
  * @note   ��ַ�� 0x20020000, ��ΪSRAM1�ڴ�ӿ���AXI
  *         ���õ������СΪ 256B (Rx �� Tx ����������С) 
  *       
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* ��ֹMPU */
  HAL_MPU_Disable();
  
  /* �� SRAM ������MPU������Ϊ��̫�����������豸 */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20020000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /*ʹ��MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}
