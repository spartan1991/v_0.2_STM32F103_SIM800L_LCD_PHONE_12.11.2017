#include "cpu_init.h"
#include "functions.h"

extern uint32_t GSMBuffer[DMA_GSM_BUFFER_SIZE];

int main(void){
	
	System_Init();
	
	while(1){
		
//		LcdClear();
		refreshGPSbuffer();
		refreshGSMbuffer();
		//LCD_GPS_Data();
//		LcdGotoXYFont(0, 0); LcdFStr(FONT_1X,(uint8_t*)GSMBuffer);
//		LcdUpdate();	
	}
}
