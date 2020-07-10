/**
  ******************************************************************************
  * @file    bsp_sdram.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   LCDӦ�ú����ӿڣ�֧��ARGB888/RGB888/RGB565/ARGB4444/ARGB1555 (����������ʾ)
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32F767 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "./lcd/bsp_lcd.h"
#include "./QR_Decoder/qr_decoder_user.h"

/**
 * @brief  ��һ��QRɨ�贰��
 * @param  None
 * @retval None
 */ 
void LCD_Open_QR_Window(void)
{
	//ѡ��ڶ���
	LCD_SelectLayer(1);
	//����һ��͸�����Σ�������ʾ��һ��Ķ�ά��
	LCD_SetTextColor (TRANSPARENCY);
	LCD_FillRect((LCD_GetXSize()-Frame_width)/2,(LCD_GetYSize()-Frame_width)/2,Frame_width,Frame_width);      	
    //����һ����ɫ���ο�
	LCD_SetColors(LCD_COLOR_RED,TRANSPARENCY);
	LCD_DrawRect((LCD_GetXSize()-Frame_width)/2,(LCD_GetYSize()-Frame_width)/2,Frame_width,Frame_width);
	//�����ĸ�ֱ�Ƿ��ŵ���ɫ
    LCD_SetColors(LCD_COLOR_GREEN,TRANSPARENCY);
	//�������Ͻ�ֱ�Ƿ���
	//ˮƽ����
    LCD_FillRect((LCD_GetXSize()-Frame_width)/2,(LCD_GetYSize()-Frame_width)/2,Frame_line_length,Frame_line_size);
	//��ֱ����
	LCD_FillRect((LCD_GetXSize()-Frame_width)/2,(LCD_GetYSize()-Frame_width)/2,Frame_line_size,Frame_line_length);
	//�������Ͻ�ֱ�Ƿ���
	//ˮƽ����
	LCD_FillRect((LCD_GetXSize()+Frame_width)/2-Frame_line_length-(Frame_line_size-1),(LCD_GetYSize()-Frame_width)/2,Frame_line_length,Frame_line_size);
	//��ֱ����
	LCD_FillRect((LCD_GetXSize()+Frame_width)/2-(Frame_line_size-1),(LCD_GetYSize()-Frame_width)/2,Frame_line_size,Frame_line_length);
	//�������½�ֱ�Ƿ���
	//ˮƽ����
    LCD_FillRect((LCD_GetXSize()-Frame_width)/2,(LCD_GetYSize()+Frame_width)/2-(Frame_line_size-1),Frame_line_length,Frame_line_size);
	//��ֱ����
	LCD_FillRect((LCD_GetXSize()-Frame_width)/2,(LCD_GetYSize()+Frame_width)/2-Frame_line_length-(Frame_line_size-1),Frame_line_size,Frame_line_length);
	//�������½�ֱ�Ƿ���
	//ˮƽ����
    LCD_FillRect((LCD_GetXSize()+Frame_width)/2-Frame_line_length-(Frame_line_size-1),(LCD_GetYSize()+Frame_width)/2-(Frame_line_size-1),Frame_line_length+Frame_line_size,Frame_line_size);
    //��ֱ����
    LCD_FillRect((LCD_GetXSize()+Frame_width)/2-(Frame_line_size-1),(LCD_GetYSize()+Frame_width)/2-Frame_line_length-(Frame_line_size-1),Frame_line_size,Frame_line_length);	
}
/**
 * @brief  ��ɨ�����ѭ����ʾɨ������.
 * @param  None
 * @retval None
 */
void LCD_Line_Scan(void)
{
	static uint16_t pos=100;
	/* ѡ��LCD�ڶ��� */
    LCD_SelectLayer(1);
	//����ͼ����ɫΪ͸��
    LCD_SetTextColor(TRANSPARENCY);
	//��һ��͸����ɫ�ľ��Σ��������һ�λ��Ƶľ���
	LCD_FillRect((LCD_PIXEL_WIDTH-Frame_width+20*Frame_line_size)/2, pos,Frame_width-20*Frame_line_size,Frame_line_size);
	//�ı�����λ��
	pos=pos+Frame_line_size; 
	//�ж������Ƿ�Խ��
    if(pos>=((LCD_PIXEL_HEIGHT+Frame_width)/2-5*Frame_line_size))
    {
      pos = (LCD_PIXEL_HEIGHT-Frame_width)/2+5*Frame_line_size;
    }	
	//����ͼ����ɫΪ��ɫ
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_FillRect((LCD_PIXEL_WIDTH-Frame_width+20*Frame_line_size)/2, pos,Frame_width-20*Frame_line_size,Frame_line_size);
}

