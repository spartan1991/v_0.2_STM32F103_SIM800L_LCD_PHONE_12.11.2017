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

extern uint8_t commandor;
extern uint8_t val;
extern char uart_inbyte[2];

//*****************Global variables******************//
int8_t movx1 = 2, movy1 = 1;
int8_t movx2 = 2, movy2 = 3;
char digits[11] = {"0123456789"};   
char number[12] = {"0000000000;"};
char endcall[5] = {"ATH0"};
char answercall[4] = {"ATA"};
char incall[5] = {"RING"};
char prefix[25] = {"ATD+38"};

int8_t step = 0;
/////************************************************************************************************************************************/////
void Cursors_Up_Down(void)
{   
    if(movx1>11) movx1 = 11;  if(movx1<2) movx1 = 2;
    if(movx2>11) movx2 = 11;  if(movx2<2) movx2 = 2;
    /*
    if(movy1>5) movy1 = 0;  if(movy1<0) movy1 = 5;
    if(movy2>5) movy2 = 0;  if(movy2<0) movy2 = 5;
    */
    switch(val)
    {
        case 2: { movx1++; movx2++; step = 0;}; break;
        case 1: { movx1--; movx2--; step = 0;}; break;
    }
    LcdGotoXYFont(movx1, movy1);
    LcdChr(FONT_1X,(byte)'+');
    LcdGotoXYFont(movx2, movy2);
    LcdChr(FONT_1X,(byte)'-');
}
/////************************************************************************************************************************************/////
void Number_Editor(void)
{
    switch(val)
    {
        case 3: step++; break;
        case 4: step--; break;
	  }
		if(step<0) step = 0;
		if(step>9) step = 9;
    switch(movx1)
    {
        case 2:  { if(val==3||val==4 ) number[0] = (*digits+step);}; break;
        case 3:  { if(val==3||val==4 ) number[1] = (*digits+step);}; break;
        case 4:  { if(val==3||val==4 ) number[2] = (*digits+step);}; break;
        case 5:  { if(val==3||val==4 ) number[3] = (*digits+step);}; break;
        case 6:  { if(val==3||val==4 ) number[4] = (*digits+step);}; break;
        case 7:  { if(val==3||val==4 ) number[5] = (*digits+step);}; break;
        case 8:  { if(val==3||val==4 ) number[6] = (*digits+step);}; break;
        case 9:  { if(val==3||val==4 ) number[7] = (*digits+step);}; break;
        case 10: { if(val==3||val==4 ) number[8] = (*digits+step);}; break;
        case 11: { if(val==3||val==4 ) number[9] = (*digits+step);}; break;
    }
		LcdGotoXYFont(1, 0);
    LcdFStr(FONT_1X,(uint8_t*)"Enter number:");
		LcdGotoXYFont(2, 2);
    LcdFStr(FONT_1X,(uint8_t*)number);
		LcdGotoXYFont(2, 4);
    LcdFStr(FONT_1X,(uint8_t*)"Press [OK]");
}

void Number_Generator_Call(void)
{
	if(val==5)
	{
		char buffer[25];
		strcpy(buffer, prefix);
		send_str_to_UART3(strcat(buffer, number));
		commandor = 4;
	}
}

void LCD_Call_Window(void)
{
	LcdGotoXYFont(0, 0);
  LcdFStr(FONT_1X,(uint8_t*)"Calling: ");
  LcdGotoXYFont(0, 2);
  LcdFStr(FONT_1X,(uint8_t*)number);
	LcdGotoXYFont(0, 4);
  LcdFStr(FONT_1X,(uint8_t*)"Press [->] END");
	if(val==2) 
	{
    send_str_to_UART3(endcall);
		commandor = 3;
  }
}

void LCD_Call_Answer(void)
{
	LcdGotoXYFont(0, 0);
  LcdFStr(FONT_1X,(uint8_t*)"Incoming call: ");
  //LcdGotoXYFont(0, 2);
  //LcdFStr(FONT_1X,(uint8_t*)number);
	LcdGotoXYFont(0, 4);
  LcdFStr(FONT_1X,(uint8_t*)"Press [<-] ANS");
	if(val==1) 
	{
    send_str_to_UART3(answercall);
  }
}

/////************************************************************************************************************************************/////
void LCD_Number_Generator(void)
{
  Cursors_Up_Down();
	Number_Editor();
	Number_Generator_Call();
}
