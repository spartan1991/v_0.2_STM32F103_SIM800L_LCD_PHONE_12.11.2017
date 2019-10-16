#include "stdint.h"

void send_byte_to_UART1(char data);     //Функция отправляет байт в UART1
void send_str_to_UART1(char * string);  //Функция отправляет строку в UART1
char read_byte_with_UART1(void);     //Функция принимает байт с UART1
void read_string_with_UART1(char buffer[]);  //Функция принимает строку с UART1

void send_byte_to_UART3(char data);     //Функция отправляет байт в UART3
void send_str_to_UART3(char * string);  //Функция отправляет строку в UART3
char read_byte_with_UART3(void);     //Функция принимает байт с UART3
void read_string_with_UART3(char buffer[]);  //Функция принимает строку с UART3
