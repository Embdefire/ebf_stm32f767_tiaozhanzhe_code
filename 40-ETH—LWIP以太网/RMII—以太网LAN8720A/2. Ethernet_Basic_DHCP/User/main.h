#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f7xx_hal.h"

/* ʹ��DHCP*/
#define USE_DHCP  

/*Զ��IP��ַ */
#define DEST_IP_ADDR0   (uint8_t)192
#define DEST_IP_ADDR1   (uint8_t)168
#define DEST_IP_ADDR2   (uint8_t)1
#define DEST_IP_ADDR3   (uint8_t)198
/*Զ�˶˿� */
#define DEST_PORT       (uint8_t)7
 
/*������̬IP��ַ */
#define IP_ADDR0   (uint8_t) 192
#define IP_ADDR1   (uint8_t) 168
#define IP_ADDR2   (uint8_t) 1
#define IP_ADDR3   (uint8_t) 122
   
/*��������*/
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/*���ص�ַ*/
#define GW_ADDR0   (uint8_t) 192
#define GW_ADDR1   (uint8_t) 168
#define GW_ADDR2   (uint8_t) 1
#define GW_ADDR3   (uint8_t) 1 

static void SystemClock_Config(void);
#endif /* __MAIN_H */

