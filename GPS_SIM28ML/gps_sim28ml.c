#include "gps_sim28ml.h"
#include "stm32f10x_dma.h" 
#include "models.h"
#include "cpu_init.h"
#include "stdint.h"
#include "uart.h"

void GPS_SIM28ML_Init(void){
	
	send_str_to_UART1("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29");
}

//GPSData_TypeDef* gpsDataParser(char gpsBuf[], GPSData_TypeDef *gpsModel){
//	
//	sscanf(gpsBuf, "$GPRMC,%2c%2c%2c.%3c,%c,%2c,%7c,%c,%3c,%7c,%c,%5c,%5c,%2c%2c%2c", 
//				gpsModel->HH, gpsModel->MM, gpsModel->SS, gpsModel->MS, gpsModel->valid,
//				gpsModel->latDeg, gpsModel->lat, gpsModel->latSign, gpsModel->lonDeg,
//				gpsModel->lon, gpsModel->lonSign, gpsModel->speed, gpsModel->course,
//				gpsModel->DA, gpsModel->MO, gpsModel->YE);
//	
//	return gpsModel;
//}

void gpsDataParser2(char buf[], char workBuf[][10], uint32_t copySize){
	
	uint32_t i = 0, y = 0, z = 0;
	uint32_t size = copySize;
	
	for(i=0; i<size; i++){
		
		if(buf[i] != ','){
			workBuf[y][z] = buf[i];
			if(z<10)z++;
		}
		
		else{
			y++; z=0;
		}
	}
	
}
