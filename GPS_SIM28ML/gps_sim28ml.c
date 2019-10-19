#include "gps_sim28ml.h"
#include "uart.h"

void GPS_SIM28ML_config(void){
	
	send_str_to_UART1("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29");
}
