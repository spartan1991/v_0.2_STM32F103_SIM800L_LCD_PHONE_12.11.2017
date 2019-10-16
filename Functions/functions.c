#include "functions.h"
#include "visualization.h"
#include "stm32f10x.h"       
#include "stm32f10x_rcc.h"   
#include "stm32f10x_gpio.h"  
#include "stm32f10x_dma.h"   
#include "stm32f10x_usart.h"
#include "uart.h"
#include "stm32f10x_conf.h"


extern uint32_t TransitBuffer[16];
extern uint32_t CleanedDataBuffer[16];

uint8_t val;

//Функция опроса клавиатуры
void KeysScan(void)
{
		val = 0;

	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)==0) val = 1; // LEFT
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)==0) val = 2; // RIGHT
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)==0) val = 3; // UP
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)==0) val = 4; // DOWN
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)==0) val = 5; // CENTER
}

void DataBufferClear (void)
{
	if(val==5)
	{
		int i;
		for(i=0; i<16; i++)
		{
			TransitBuffer[i] = 0;
		}
	}
}

void BufferFiltering (void)
{
	for(int i=0; i<16; i++)
	{
		if((TransitBuffer[i]!='\r')&&(TransitBuffer[i]!='\n'))
		{
			CleanedDataBuffer[i] = TransitBuffer[i];
		}
		else CleanedDataBuffer[i] = ' ';
	}
}	
