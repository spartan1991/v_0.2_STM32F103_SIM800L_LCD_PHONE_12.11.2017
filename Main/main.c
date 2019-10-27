#include "cpu_init.h"
#include "functions.h"
#include "models.h"

int main(void){
	
	extern char gpsData[20][10];
	
	System_Init();
	
	while(1){
		
		LcdClear();


		LcdGotoXYFont(0, 0); 
		LcdFStr(FONT_1X,(uint8_t*)refreshGPSbuffer());
		
		LcdUpdate();	
	}
}
