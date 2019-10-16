#include "gsm_sim800l.h"
#include "tasks.h"  
#include "stm32f10x.h"       
#include "stm32f10x_rcc.h"   
#include "stm32f10x_gpio.h"  
#include "stm32f10x_dma.h"   
#include "stm32f10x_usart.h"
#include "stm32f10x_conf.h"
#include "cpu_init.h"
#include "functions.h"
#include "visualization.h"
#include "stdint.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "uart.h"
#include "task.h"
#include "queue.h"
#include "n3310.h"

//Функция настройки GSM-модуля SIM800L
void SIM800L_config(void)
{
	send_str_to_UART3("ATE0");
	send_str_to_UART3("ATV0");
}

