#include "visualization.h"
#include "functions.h"
#include "stdint.h"
#include "string.h"
#include "stm32f10x_dma.h"   
#include "uart.h"
#include "n3310.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"

uint8_t commandor;
extern uint8_t keyBoardStatus;
extern char uart_inbyte[2];
extern uint32_t GPSBuffer[DMA_BUFFER_SIZE];

int8_t movx1 = 2, movy1 = 1;
int8_t movx2 = 2, movy2 = 3;
char digits[11] = {"0123456789"};   
char number[12] = {"0000000000;"};
char endcall[5] = {"ATH0"};
char answercall[4] = {"ATA"};
char incall[5] = {"RING"};
char prefix[25] = {"ATD+38"};

int8_t step = 0;

void Cursors_Up_Down(void){   
	
		uint8_t outOfRangeLeft = 0;
    //uint8_t outOfRangeRight = 0;
	
    if(movx1>11) movx1 = 11;  if(movx1<2){if(keyBoardStatus==1)outOfRangeLeft = 1; else movx1 = 2;}
    if(movx2>11) movx2 = 11;  if(movx2<2){if(keyBoardStatus==1)outOfRangeLeft = 1; else movx2 = 2;}
		
		if(outOfRangeLeft){commandor = 2;}
	
    switch(keyBoardStatus){
        case 2: { movx1++; movx2++; step = 0;}; break;
        case 1: { movx1--; movx2--; step = 0;}; break;
    }
    LcdGotoXYFont(movx1, movy1);
    LcdChr(FONT_1X,(byte)'`');
    LcdGotoXYFont(movx2, movy2);
    LcdChr(FONT_1X,(byte)'');
}

void Number_Editor(void){
	
    switch(keyBoardStatus){
        case 3: {
					step++; break;
				}
        case 4: {
					step--; break;
				}
	  }
		
		if(step<0) step = 0;
		if(step>9) step = 9;
		
    switch(movx1){
        case 2:  { if(keyBoardStatus==3||keyBoardStatus==4 ) number[0] = (*digits+step);}; break;
        case 3:  { if(keyBoardStatus==3||keyBoardStatus==4 ) number[1] = (*digits+step);}; break;
        case 4:  { if(keyBoardStatus==3||keyBoardStatus==4 ) number[2] = (*digits+step);}; break;
        case 5:  { if(keyBoardStatus==3||keyBoardStatus==4 ) number[3] = (*digits+step);}; break;
        case 6:  { if(keyBoardStatus==3||keyBoardStatus==4 ) number[4] = (*digits+step);}; break;
        case 7:  { if(keyBoardStatus==3||keyBoardStatus==4 ) number[5] = (*digits+step);}; break;
        case 8:  { if(keyBoardStatus==3||keyBoardStatus==4 ) number[6] = (*digits+step);}; break;
        case 9:  { if(keyBoardStatus==3||keyBoardStatus==4 ) number[7] = (*digits+step);}; break;
        case 10: { if(keyBoardStatus==3||keyBoardStatus==4 ) number[8] = (*digits+step);}; break;
        case 11: { if(keyBoardStatus==3||keyBoardStatus==4 ) number[9] = (*digits+step);}; break;
    }
		LcdGotoXYFont(1, 0);
    LcdFStr(FONT_1X,(uint8_t*)"Enter number:");
		LcdGotoXYFont(2, 2);
    LcdFStr(FONT_1X,(uint8_t*)number);
		LcdGotoXYFont(2, 4);
    LcdFStr(FONT_1X,(uint8_t*)"Press [OK]");
}

void Number_Generator_Call(void){
	
	if(keyBoardStatus==5){
		char buffer[25];
		strcpy(buffer, prefix);
		send_str_to_UART3(strcat(buffer, number));
		commandor = 4;
	}
}

void LCD_Call_Window(void){
	
	LcdGotoXYFont(0, 0);
  LcdFStr(FONT_1X,(uint8_t*)"Calling: ");
  LcdGotoXYFont(0, 2);
  LcdFStr(FONT_1X,(uint8_t*)number);
	LcdGotoXYFont(0, 4);
  LcdFStr(FONT_1X,(uint8_t*)"Press [->] END");
	
	if(keyBoardStatus==2) {
    send_str_to_UART3(endcall);
		commandor = 3;
  }
}

void LCD_Call_Answer(void){
	
	LcdGotoXYFont(0, 0);
  LcdFStr(FONT_1X,(uint8_t*)"Incoming call: ");
	LcdGotoXYFont(0, 4);
  LcdFStr(FONT_1X,(uint8_t*)"Press [<-] ANS");
	
	if(keyBoardStatus==1) {
    send_str_to_UART3(answercall);
  }
}

void LCD_Number_Generator(void){
	
  Cursors_Up_Down();
	Number_Editor();
	Number_Generator_Call();
}

void LCD_GPS_Data(void){
	
  LcdGotoXYFont(0, 0); LcdFStr(FONT_1X,(uint8_t*)GPSBuffer);
	if(keyBoardStatus==2) commandor = 3; //**TEST
}

void vTask_Visualization(void *pvParameters){
	
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
