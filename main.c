/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "system_stm32f10x.h"

#include "mcu_init.h"
#include "config.h"
#include "device.h"
#include "spi2.h"
#include "socket.h"
#include "w5500.h"
#include "util.h"
#include "dhcp.h"
#include "string.h"
#include <stdio.h>

extern uint8 txsize[];
extern uint8 rxsize[];

uint8 buffer[2048];/*定义一个2KB的缓存*/

int main(void)
{
  RCC_Configuration(); /* 配置单片机系统时钟*/
  NVIC_Configuration();/* 配置嵌套中断向量*/
	Systick_Init(72);/* 初始化Systick工作时钟*/
  GPIO_Configuration();/* 配置GPIO*/ 
	Timer_Configuration();/*定时器初始化*/
  USART1_Init(); /*初始化串口通信:115200@8-n-1*/
  // at24c16_init();/*初始化eeprom*/
  printf("W5500 EVB initialization over.\r\n");
  
  Reset_W5500();/*硬重启W5500*/
  WIZ_SPI_Init();/*初始化SPI接口*/
  printf("W5500 initialized!\r\n");  
  
  set_default(); 	
  init_dhcp_client();

  while(1)
  {
		DHCP_run();
	}
}





#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
