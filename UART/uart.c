#include "stm32f10x_usart.h"
#include "stdint.h"
#include "uart.h"
	
/**************************UART1**************************/
//Функция отправляет байт в UART1
void send_byte_to_UART1(char data){
	
  while(!(USART1->SR & USART_SR_TC));
  USART1->DR=data;
}

//Функция отправляет строку в UART1
void send_str_to_UART1(char *string){
	
  uint8_t i = 0;
	
  while(string[i]){	
	send_byte_to_UART1(string[i]);
    i++;
  }
  send_byte_to_UART1('\r');
  send_byte_to_UART1('\n');
}

//Функция принимает байт с UART1
char read_byte_with_UART1(void){
	
	char* uart1_byte;
	
  if(USART1->SR & USART_SR_RXNE){
		*uart1_byte = USART1->DR;
	}
  return *uart1_byte;
}

//Функция принимает строку с UART1
void read_string_with_UART1(char buffer1[]){ 
	
	uint8_t i = 0;
	
	if (USART1->SR & USART_SR_RXNE){ // ... не пришло ли что-то в UART3 ?
		if((USART1->DR>=' ')&&(USART1->DR<='')){  
      buffer1[i] = USART1->DR; //Считываем то что пришло, в элемент массива buffer[i]
      i++;
		}
  }
}


/**************************UART3**************************/
//Функция отправляет байт в UART3
void send_byte_to_UART3(char data){
	
  while(!(USART3->SR & USART_SR_TC));
  USART3->DR=data;
}

//Функция отправляет строку в UART3
void send_str_to_UART3(char *string){
	
  uint8_t i = 0;
	
  while(string[i]){
	send_byte_to_UART3(string[i]);
    i++;
  }
  send_byte_to_UART3('\r');
  send_byte_to_UART3('\n');
}

//Функция принимает байт с UART3
char read_byte_with_UART3(void)
{
	char* uart3_byte;
	
  if(USART3->SR & USART_SR_RXNE){
		*uart3_byte = USART3->DR;
	}
  return *uart3_byte;
}

//Функция принимает строку с UART3
void read_string_with_UART3(char buffer[]){ 
	
	uint8_t i = 0;
	
	if (USART3->SR & USART_SR_RXNE){ // ... не пришло ли что-то в UART3 ? 
		if((USART3->DR>=' ')&&(USART3->DR<='')){   
      buffer[i] = USART3->DR; //Считываем то что пришло, в элемент массива buffer[i]
      i++;
		}
  }
}
