#include "functions.h"

uint8_t keyBoardStatus;
extern uint32_t DMABuffer[DMA_BUFFER_SIZE];
extern uint32_t GPSBuffer[DMA_BUFFER_SIZE];

//Функция опроса клавиатуры
void KeysScan(void){
	
		keyBoardStatus = 0;

	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)==0) keyBoardStatus = 1; // LEFT
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)==0) keyBoardStatus = 2; // RIGHT
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)==0) keyBoardStatus = 3; // UP
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)==0) keyBoardStatus = 4; // DOWN
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)==0) keyBoardStatus = 5; // CENTER
}

void GPSbufferCopyDMAbuffer(char buf1[], char buf2[], uint32_t val){
	
	uint8_t copyFlag = 0, block = 0;
	uint32_t i=0, y=0;
	
	for(i = 0; i < val; i++){	
		if(buf2[i]=='$'&&block!=1){copyFlag = 1; block=1;}
		if(buf2[i]=='\n'){copyFlag = 0;}
		if(copyFlag){ 
			buf1[y++] = buf2[i];
		}
	}
}

void refreshGPSbuffer(void){
	
	GPSbufferCopyDMAbuffer((char*)GPSBuffer, (char*)DMABuffer, DMA_BUFFER_SIZE);
}
