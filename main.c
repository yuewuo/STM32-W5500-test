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
uint16 local_port=22;
uint16 len;
uint8 ip[4];

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

  setkeepalive(SOCK_TERMINAL);  // 打开keepalive功能
  printf("TCP Server Port: %d\r\n",local_port);
  printf("W5500 Init Complete!\r\n");

  while(1)
  {
    Delay_ms(500);
		DHCP_run();
    getSIPR(ip);
    printf("IP : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);  // 疯狂打印IP
    switch(getSn_SR(SOCK_TERMINAL)) {
      case SOCK_INIT:  // Socket处于初始化完成(打开)状态
        printf("SOCK_INIT\n");
        listen(SOCK_TERMINAL);  // 监听刚刚打开的本地端口，等待客户端连接
        break;
      case SOCK_ESTABLISHED:  // Socket处于连接建立状态
        printf("SOCK_ESTABLISHED\n");
        if(getSn_IR(SOCK_TERMINAL) & Sn_IR_CON)	{
          setSn_IR(SOCK_TERMINAL, Sn_IR_CON);				// Sn_IR的CON位置1，通知W5500连接已建立
        }
        len=getSn_RX_RSR(SOCK_TERMINAL);  // 读取W5500空闲接收缓存寄存器的值并赋给len，Sn_RX_RSR表示接收缓存中已接收和保存的数据大小
        if(len>0) {
          recv(SOCK_TERMINAL,buffer,len);  // W5500接收来自客户端的数据，并通过SPI发送给MCU
          printf("%s\r\n",buffer);  // 串口打印接收到的数据
          send(SOCK_TERMINAL,buffer,len);  // 接收到数据后再回给客户端，完成数据回环
        }
        break;
      case SOCK_CLOSE_WAIT:  // Socket处于等待关闭状态
        printf("SOCK_CLOSE_WAIT\n");
        close(SOCK_TERMINAL);  // 关闭Socket0
        break;
      case SOCK_CLOSED:
        printf("SOCK_CLOSED\n");
        socket(SOCK_TERMINAL, Sn_MR_TCP, local_port, Sn_MR_ND);		// 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
        break;
      case SOCK_LISTEN:
        printf("SOCK_LISTEN\n");
        break;
      default:
        printf("unknown state\n");
    }
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
