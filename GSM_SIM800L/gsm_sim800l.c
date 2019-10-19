#include "gsm_sim800l.h"
#include "uart.h"

//Функция настройки GSM-модуля SIM800L
void SIM800L_config(void){
	
	send_str_to_UART3("ATE0");
	send_str_to_UART3("ATV0");
}

