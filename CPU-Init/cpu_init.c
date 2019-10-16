#include "cpu_init.h"
#include "stm32f10x.h"       
#include "stm32f10x_rcc.h"   
#include "stm32f10x_gpio.h"  
#include "stm32f10x_dma.h"   
#include "stm32f10x_usart.h"
#include "stm32f10x_conf.h"
#include "uart.h"

#define DMA_BUFFER_SIZE	 16

uint32_t TransitBuffer[16];
uint32_t CleanedDataBuffer[16];

//Функция настройки периферии для работы системы
void System_config(void)
{
  //__enable_irq(); // Разрешить прерывания глобально
	
	//Включаем порты и UART3
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_USART1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//НЕ ЗАБЫВАТЬ!!! USART3 тактируется от шины APB1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	

  //Создаём структуры для настройки портов
  GPIO_InitTypeDef gpio_porta_lcd;  //Создаём структуру для настройки порта "A" под LCD
	GPIO_InitTypeDef gpio_porta_usart1;  //Создаём структуру для настройки порта "A" для USART1 (GPS/Wi-Fi)
  GPIO_InitTypeDef gpio_portb_usart3;  //Создаём структуру для настройки порта "B" для USART3 (GSM)
  GPIO_InitTypeDef gpio_portb_keys; //Создаём структуру для настройки порта "B" под клавиатуру
	
	//**************************************************************************************************************************//
	GPIO_InitTypeDef gpio_portc_led; //Создаём структуру для настройки порта "C" под светодиод
	//Настраиваем ноги PC13 для мигания светодиода
  gpio_portc_led.GPIO_Pin = (GPIO_Pin_13); //Подключаем пины для работы с светодиод
  gpio_portc_led.GPIO_Mode = GPIO_Mode_Out_PP;  //Задаём пинам порта ввода-вывода "С" режим работы
  gpio_portc_led.GPIO_Speed = GPIO_Speed_50MHz; //Задаём пинам порта ввода-вывода "С" скорость работы
  GPIO_Init(GPIOC, &gpio_portc_led);
	GPIOC->BSRR = GPIO_BSRR_BS13;
  //**************************************************************************************************************************//
	
  //Настраиваем ноги PA0 - РА4 для работы с LCD по программному SPI
  gpio_porta_lcd.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4); //Подключаем пины для работы с LCD
  gpio_porta_lcd.GPIO_Mode = GPIO_Mode_Out_PP;  //Задаём пинам порта ввода-вывода "А" режим работы с LCD
  gpio_porta_lcd.GPIO_Speed = GPIO_Speed_50MHz; //Задаём пинам порта ввода-вывода "А" скорость работы
  GPIO_Init(GPIOA, &gpio_porta_lcd);
	
	//Настраиваем ногу PA10 как вход UARTа1 (RxD)
  gpio_porta_usart1.GPIO_Pin   = GPIO_Pin_10;
  gpio_porta_usart1.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &gpio_porta_usart1);

  //Настраиваем ногу PA9 как выход UARTа1 (TxD)
  //Причем не просто выход, а выход с альтернативной функцией
  gpio_porta_usart1.GPIO_Pin   = GPIO_Pin_9;
  gpio_porta_usart1.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_porta_usart1.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &gpio_porta_usart1);

  //Настраиваем ногу PB11 как вход UARTа3 (RxD)
  gpio_portb_usart3.GPIO_Pin   = GPIO_Pin_11;
  gpio_portb_usart3.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &gpio_portb_usart3);

  //Настраиваем ногу PB10 как выход UARTа3 (TxD)
  //Причем не просто выход, а выход с альтернативной функцией
  gpio_portb_usart3.GPIO_Pin   = GPIO_Pin_10;
  gpio_portb_usart3.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_portb_usart3.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &gpio_portb_usart3);

  //Настраиваем ноги PB5 - РB9 для работы с клавиатурой
  gpio_portb_keys.GPIO_Pin = (GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
  gpio_portb_keys.GPIO_Mode = GPIO_Mode_IPU;
  gpio_portb_keys.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &gpio_portb_keys);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////************************************************************************************************************************************/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Функция настройки UART1
void UART1_config(void)
{
  //Заполняем структуру настройками 3-го UARTa
  USART_InitTypeDef uart1_struct;
  uart1_struct.USART_BaudRate            = 9600;
  uart1_struct.USART_WordLength          = USART_WordLength_8b;
  uart1_struct.USART_StopBits            = USART_StopBits_1;
  uart1_struct.USART_Parity              = USART_Parity_No ;
  uart1_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  uart1_struct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

  //Инициализируем UART
  USART_Init(USART1, &uart1_struct);
	
	//Активируем приём с последовательного порта по запросу DMA
  //USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

  //Включаем UART
  USART_Cmd(USART1, ENABLE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////************************************************************************************************************************************/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Функция настройки UART3
void UART3_config(void)
{
  //Заполняем структуру настройками 3-го UARTa
  USART_InitTypeDef uart3_struct;
  uart3_struct.USART_BaudRate            = 9600;
  uart3_struct.USART_WordLength          = USART_WordLength_8b;
  uart3_struct.USART_StopBits            = USART_StopBits_1;
  uart3_struct.USART_Parity              = USART_Parity_No ;
  uart3_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  uart3_struct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

  //Инициализируем UART
  USART_Init(USART3, &uart3_struct);
	
	//Активируем приём с последовательного порта по запросу DMA
  USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);

  //Включаем UART
  USART_Cmd(USART3, ENABLE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////************************************************************************************************************************************/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Функция настройки DMA1
void DMA_config(void)
{
  DMA_InitTypeDef dma;  //Создаём структуру для настройки DMA1
  //Заполняем структуру настройками DMA1
  DMA_StructInit(&dma);
  dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);     // это адрес регистра данных USART3
  dma.DMA_MemoryBaseAddr = (uint32_t)&TransitBuffer[0];     // адрес нулевого элемента массива
  dma.DMA_DIR = DMA_DIR_PeripheralSRC;                      // принимаем данные с периферии
  dma.DMA_Mode = DMA_Mode_Circular;                         // Режим DMA - циклический
  dma.DMA_BufferSize = DMA_BUFFER_SIZE;                     // буфер – 16 байт (#define DMA_BUFFER_SIZE 16)
  dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // перифериию не инкрементируем
  dma.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // память инкрементируем
  dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // шлем байтами
  dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // шлем байтами
	 
  //Инициализируем DMA1
  DMA_Init(DMA1_Channel3, &dma);	                          // Идем в таблицу с каналами – USART3_RX – 3 канал, все верно =)
	
  //Включаем прямой доступ к памяти DMA
  DMA_Cmd(DMA1_Channel3, ENABLE);	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////************************************************************************************************************************************/////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
