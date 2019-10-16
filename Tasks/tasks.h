//Таск конфигурации системы (выполняется один раз, после чего, сам себя удаляет)
void vTask_SystemInit(void *pvParameters);

//Таск сканирует устройства ввода (датчики, клавиатура...)
void vTask_ScanInputDevices(void *pvParameters);

//Таск визуализации (выводит на дисплей диалоговые окна)
void vTask_Visualization(void *pvParameters);
