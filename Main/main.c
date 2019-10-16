#include "stm32f10x.h"       
#include "stm32f10x_rcc.h"   
#include "stm32f10x_gpio.h"  
#include "stm32f10x_dma.h"   
#include "stm32f10x_usart.h"
#include "stm32f10x_conf.h"
#include "cpu_init.h"
#include "stdint.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "uart.h"
#include "tasks.h"
#include "functions.h"
#include "visualization.h" 
#include "task.h"
#include "queue.h"
#include "n3310.h"

// Отдадочная затычка. Сюда можно вписать код обработки ошибок.
#define	ERROR_ACTION(CODE,POS)		do{}while(0)
	
int main(void)
{
	if(pdTRUE != xTaskCreate(vTask_SystemInit,"SystemInit",configMINIMAL_STACK_SIZE, NULL, 1, NULL)) ERROR_ACTION(TASK_NOT_CREATE,0);
	if(pdTRUE != xTaskCreate(vTask_ScanInputDevices,"vTask_ScanInputDevices",configMINIMAL_STACK_SIZE, NULL, 1, NULL)) ERROR_ACTION(TASK_NOT_CREATE,0);
	if(pdTRUE != xTaskCreate(vTask_Visualization,"vTask_Visualization",configMINIMAL_STACK_SIZE, NULL, 1, NULL)) ERROR_ACTION(TASK_NOT_CREATE,0);

// Создаем задачи. Если при создании задачи возвращенный параметр не TRUE, то обрабатываем ошибку.
// vTask_SystemInit 								- это имя функции которая будет задачей. 
// "Blinker" 								- текстовая строка для отладки. Может быть любой, но длина ограничина в конфиге ОС. Ну и память она тоже ест немного.
// configMINIMAL_STACK_SIZE - размер стека для задачи. Определяется опытным путем. В данном случае стоит системный минимум ,т.к. 
// 													Задачи используют очень мало памяти. Должно хватить. 
// NULL 										- передаваемый pvParameters в задачу. В данном случае не нужен, потому NULL. Но можно задаче при создании передевать
// 													Разные данные, для инициализации или чтобы различать две копии задачи между собой. 
// tskIDLE_PRIORITY + 1			- приоритет задачи. В данный момент выбран минимально возможный. На 1 пункт выше чем IDLE
// NULL 										- тут вместо NULL можно было вписать адрес переменной типа xTaskHandle в которую бы упал дескриптор задачи. 
//													Например так: xTaskCreate(vBlinker,"Blinker",configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xBlinkerTask); 
//													где xBlinkerTask это глобальная переменная обьявленная как xTaskHandle xBlinkerTask; глобальная - чтобы ее везде
//													было видно. Отовсюду она была доступна. Но можно и как static обьявить или еще каким образом передать хэндл.
//													И зная эту переменную и то что там дескриптор этой задачи мы могли бы из другой задачи ее грохнуть, поменять
//													приоритет или засуспендить. В общем, управлять задачей. Но в моем примере это не требуется. 
// 													Остальные аналогично.
	
// Запускаем диспетчер и понеслась. 	
  vTaskStartScheduler();
  while(1)
  {
	  ;
  }
}
