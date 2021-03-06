#include "misc.h"
#include "stm32f10x.h"  
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"   
#include "stm32f10x_gpio.h"  
#include "stm32f10x_dma.h"   
#include "stm32f10x_usart.h"
#include "stm32f10x_conf.h"
#include "gps_sim28ml.h"
#include "gsm_sim800l.h"
#include "n3310.h"
#include "uart.h"

#define DMA_GPS_BUFFER_SIZE 84
#define DMA_GSM_BUFFER_SIZE 84

void System_Init(void);
uint32_t* refreshGPSbuffer(void);
uint32_t* refreshGSMbuffer(void);
