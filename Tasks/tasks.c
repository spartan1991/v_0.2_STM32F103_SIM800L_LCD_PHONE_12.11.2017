#include "stm32f10x_dma.h"
#include "cpu_init.h"
#include "gps_sim28ml.h"
#include "gsm_sim800l.h"
#include "functions.h"
#include "visualization.h"
#include "n3310.h"

uint8_t commandor = 3;
extern uint8_t keyBoardStatus;

//Таск конфигурации системы (выполняется один раз, после чего, сам себя удаляет)
void vTask_SystemInit(void *pvParameters){
	
  System_config();
	USART1_GPS_config();
	USART3_GSM_config();
  DMA1_GPS_config();
	GPS_SIM28ML_config();
  SIM800L_config();
  LcdInit();
}

//Таск сканирует устройства ввода (датчики, клавиатура...)
void vTask_ScanInputDevices(void *pvParameters){
	
	while(1){	
	  KeysScan();
	}
}

//Таск сканирует устройства ввода (датчики, клавиатура...)
void vTask_OperativeComputing(void *pvParameters){
	
	while(1){	
		
	  refreshGPSbuffer();
	}
}

//Таск визуализации (выводит на дисплей диалоговые окна)
void vTask_Visualization(void *pvParameters){
	
	while(1){
		if(keyBoardStatus!=0){
		  LcdClear();
	  }
		switch(commandor){
			case 2: {			
			  LcdClear(); LCD_GPS_Data();	break;
			}			
			case 3: {
				LcdClear(); LCD_Number_Generator(); break;
			}
			case 4: {
				LcdClear(); LCD_Call_Window(); break;
			}
			case 5: {
			  LcdClear(); LCD_Call_Answer(); break;
			}
		}
	  LcdUpdate();
	}
}
