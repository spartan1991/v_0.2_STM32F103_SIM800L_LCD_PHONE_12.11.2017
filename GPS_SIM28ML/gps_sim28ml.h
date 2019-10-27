#include <stdio.h>
#include "models.h"

void GPS_SIM28ML_Init(void);
//GPSData_TypeDef* gpsDataParser(char gpsBuf[], GPSData_TypeDef *gpsModel);
void gpsDataParser2(char buf[], char workBuf[][10], uint32_t copySize);
