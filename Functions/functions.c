#include "functions.h"

uint8_t keyBoardStatus;

//Функция опроса клавиатуры
void KeysScan(void){
	
		keyBoardStatus = 0;

	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)==0) keyBoardStatus = 1; // LEFT
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)==0) keyBoardStatus = 2; // RIGHT
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)==0) keyBoardStatus = 3; // UP
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)==0) keyBoardStatus = 4; // DOWN
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)==0) keyBoardStatus = 5; // CENTER
}
