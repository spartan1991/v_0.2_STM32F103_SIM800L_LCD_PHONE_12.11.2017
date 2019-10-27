#ifndef _MODELS_H
#define _MODELS_H

#include "stdint.h"

typedef struct
{
  char HH[2];       
	char MM[2];
  char SS[2]; 
  char MS[3];       
	char valid[1];
	char latDeg[2];
  char lat[7]; 	
	char latSign[1];
	char lonDeg[3];
	char lon[7]; 	
	char lonSign[1];
	char speed[5];
	char course[5];
	char DA[2];       
	char MO[2];
  char YE[2]; 
	
}GPSData_TypeDef;

#endif  /*  _MODELS_H */
