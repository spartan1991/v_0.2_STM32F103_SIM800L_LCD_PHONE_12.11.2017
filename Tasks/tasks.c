#include "tasks.h"  
#include "cpu_init.h"
#include "gps_sim28ml.h"
#include "gsm_sim800l.h"
#include "functions.h"
#include "visualization.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "n3310.h"

uint8_t commandor = 3;
extern uint8_t val;
extern uint32_t TransitBuffer[16];
extern uint32_t CleanedDataBuffer[16];
extern int i;

//Таск конфигурации системы (выполняется один раз, после чего, сам себя удаляет)
void vTask_SystemInit(void *pvParameters){
	
  System_config();
	UART1_config();
	UART3_config();
  DMA_config();
	GPS_SIM28ML_config();
  SIM800L_config();
  LcdInit();
  vTaskDelay(10);
		
  vTaskDelete(NULL);
}

//Таск сканирует устройства ввода (датчики, клавиатура...)
void vTask_ScanInputDevices(void *pvParameters){
	
	while(1){	
	  KeysScan();
	  vTaskDelay(5);
	}
}

//Таск визуализации (выводит на дисплей диалоговые окна)
void vTask_Visualization(void *pvParameters){
	
	while(1){
		if(val!=0){
		  LcdClear();
	  }
		switch(commandor){
			case 2: {
			   LcdClear(); BufferFiltering(); 
			   LcdGotoXYFont(0, 0); LcdFStr(FONT_1X,(uint8_t*)TransitBuffer); 
				 LcdGotoXYFont(0, 3); LcdFStr(FONT_1X,(uint8_t*)CleanedDataBuffer);
			   DataBufferClear(); break;
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
	  vTaskDelay(100);
	}
}
