/*
 * Имя          :  n3310.h
 *
 * Описание     :  Это заголовочный файл для драйвера графического LCD от Nokia 3310, а также его китайских клонов.
 *                 Базируется на коде библиотек написанных Sylvain Bissonnette и Fandi Gunawan:
 *                 http://www.microsyl.com/index.php/2010/03/24/nokia-lcd-library/
 *                 http://fandigunawan.wordpress.com/2008/06/18/lcd-nokia-3310-pcd8544-driver-in-winavravr-gcc/
 *                 Основные отличия между оригиналом и клоном хорошо описаны в статье от Aheir:
 *                 http://radiokot.ru/articles/29/
 *
 * Автор        :  Xander Gresolio <xugres@gmail.com>
 * Веб-страница :  http://we.easyelectronics.ru/profile/XANDER/
 *
 * Лицензия     :  GPL v3.0
 *
 * Компилятор   :  WinAVR, GCC for AVR platform
 */

#ifndef _N3310_H_
#define _N3310_H_

// закомментируйте эту директиву, если ваш дисплей оригинальный
//#define CHINA_LCD

// Порт к которому подключен LCD (здесь пример распиновки для ATmega8)
// Библиотека использует аппаратный SPI, поэтому если хотите иной порт - придется реализовать SPI программно)

#define LCD_RST_HIGH     GPIOA->BSRR |= GPIO_BSRR_BS0    // PA0 = 1
#define LCD_RST_LOW      GPIOA->BRR |= GPIO_BRR_BR0      // PA0 = 0
#define LCD_CE_HIGH      GPIOA->BSRR |= GPIO_BSRR_BS1    // PA1 = 1
#define LCD_CE_LOW       GPIOA->BRR |= GPIO_BRR_BR1      // PA1 = 0
#define LCD_DC_HIGH      GPIOA->BSRR |= GPIO_BSRR_BS2    // PA2 = 1
#define LCD_DC_LOW       GPIOA->BRR |= GPIO_BRR_BR2      // PA2 = 0
#define LCD_Din_HIGH     GPIOA->BSRR |= GPIO_BSRR_BS3    // PA3 = 1
#define LCD_Din_LOW      GPIOA->BRR |= GPIO_BRR_BR3      // PA3 = 0
#define LCD_Clk_HIGH     GPIOA->BSRR |= GPIO_BSRR_BS4    // PA4 = 1
#define LCD_Clk_LOW      GPIOA->BRR |= GPIO_BRR_BR4      // PA4 = 0

// Разрешение дисплея в пикселях
#define LCD_X_RES                  84    // разрешение по горизонтали
#define LCD_Y_RES                  48    // разрешение по вертикали

// Настройки для рисования группы прямоугольников функцией LcdBars ( byte data[], byte numbBars, byte width, byte multiplier )
#define EMPTY_SPACE_BARS           2     // расстояние между прямоугольниками
#define BAR_X                      30    // координата x
#define BAR_Y                      47    // координата y

// Размер кэша ( 84 * 48 ) / 8 = 504 байта
#define LCD_CACHE_SIZE             ( ( LCD_X_RES * LCD_Y_RES ) / 8 )

#define FALSE                      0
#define TRUE                       1

// Для возвращаемых значений
#define OK                         0   // Безошибочная отрисовка
#define OUT_OF_BORDER              1   // Выход за границы дисплея
#define OK_WITH_WRAP               2   // Переход на начало (ситуация автоинкремента указателя курсора при выводе длинного текста)

//Для функции отображения переменных на LCD (Чёрненький В.В.)
#define DIG_BASE             10   // Основание системы счисления для перевода
#define MAX_SIZE             6    // Максимальное число вводимых символов
#define SPACE_CHAR           ' '  // Символ "пустого" места
#define NEG_CHAR             '-'  // Символ "минус"

typedef unsigned char              byte;

// Перечисления
typedef enum
{
    LCD_CMD  = 0,     // Команда
    LCD_DATA = 1      // Данные

} LcdCmdData;

typedef enum
{
    PIXEL_OFF =  0,   // Погасить пиксели дисплея
    PIXEL_ON  =  1,   // Включить пиксели дисплея
    PIXEL_XOR =  2    // Инвертировать пиксели

} LcdPixelMode;

typedef enum
{
    FONT_1X = 1,      // Обычный размер шрифта 5x7
    FONT_2X = 2       // Увеличенный размер шрифта

} LcdFontSize;

// Прототипы функций, детальную информацию смотрим внутри n3310lcd.c
void LcdInit       ( void );   // Инициализация
void LcdClear      ( void );   // Очистка буфера
void LcdUpdate     ( void );   // Копирование буфера в ОЗУ дисплея
void LcdVariable   ( unsigned int);
void LcdImage      ( const byte *imageData );   // Рисование картинки из массива в Flash ROM
void LcdContrast   ( byte contrast );   // Установка контрастности дисплея
byte LcdGotoXYFont ( byte x, byte y );   // Установка курсора в позицию x,y
byte LcdChr        ( LcdFontSize size, byte ch );   // Вывод символа в текущей позиции
byte LcdStr        ( LcdFontSize size, byte dataArray[] );   // Вывод строки сохраненной в RAM
byte LcdFStr       ( LcdFontSize size, const byte *dataPtr );   // Вывод строки сохраненной в Flash ROM
byte LcdPixel      ( byte x, byte y, LcdPixelMode mode );   // Точка
byte LcdLine       ( byte x1, byte y1, byte x2, byte y2, LcdPixelMode mode );   // Линия
byte LcdCircle     ( byte x, byte y, byte radius, LcdPixelMode mode);   // Окружность
byte LcdRect       ( byte x1, byte y1, byte x2, byte y2, LcdPixelMode mode );   // Прямоугольник
byte LcdSingleBar  ( byte baseX, byte baseY, byte height, byte width, LcdPixelMode mode );   // Один 
byte LcdBars       ( byte data[], byte numbBars, byte width, byte multiplier );   // Несколько


/*
 * Таблица для отображения символов (ASCII[0x20-0x7F] + CP1251[0xC0-0xFF] = всего 160 символов)
 */
static const byte FontLookup [][5] =
{
   { 0x00, 0x00, 0x00, 0x00, 0x00 },   //   0x20  32
   { 0x00, 0x00, 0x5F, 0x00, 0x00 },   // ! 0x21  33
   { 0x00, 0x07, 0x00, 0x07, 0x00 },   // " 0x22  34
   { 0x14, 0x7F, 0x14, 0x7F, 0x14 },   // # 0x23  35
   { 0x24, 0x2A, 0x7F, 0x2A, 0x12 },   // $ 0x24  36
   { 0x4C, 0x2C, 0x10, 0x68, 0x64 },   // % 0x25  37
   { 0x36, 0x49, 0x55, 0x22, 0x50 },   // & 0x26  38
   { 0x00, 0x05, 0x03, 0x00, 0x00 },   // ' 0x27  39
   { 0x00, 0x1C, 0x22, 0x41, 0x00 },   // ( 0x28  40
   { 0x00, 0x41, 0x22, 0x1C, 0x00 },   // ) 0x29  41
   { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // * 0x2A  42
   { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // + 0x2B  43
   { 0x00, 0x00, 0x50, 0x30, 0x00 },   // , 0x2C  44
   { 0x10, 0x10, 0x10, 0x10, 0x10 },   // - 0x2D  45
   { 0x00, 0x60, 0x60, 0x00, 0x00 },   // . 0x2E  46
   { 0x20, 0x10, 0x08, 0x04, 0x02 },   // / 0x2F  47
   { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0 0x30  48
   { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1 0x31  49
   { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2 0x32  50
   { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3 0x33  51
   { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4 0x34  52
   { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5 0x35  53
   { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6 0x36  54
   { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7 0x37  55
   { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8 0x38  56
   { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9 0x39  57
   { 0x00, 0x36, 0x36, 0x00, 0x00 },   // : 0x3A  58
   { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ; 0x3B  59
   { 0x08, 0x14, 0x22, 0x41, 0x00 },   // < 0x3C  60
   { 0x14, 0x14, 0x14, 0x14, 0x14 },   // = 0x3D  61
   { 0x00, 0x41, 0x22, 0x14, 0x08 },   // > 0x3E  62
   { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ? 0x3F  63
   { 0x32, 0x49, 0x79, 0x41, 0x3E },   // @ 0x40  64
   { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A 0x41  65
   { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B 0x42  66
   { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C 0x43  67
   { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D 0x44  68
   { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E 0x45  69
   { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F 0x46  70
   { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G 0x47  71
   { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H 0x48  72
   { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I 0x49  73
   { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J 0x4A  74
   { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K 0x4B  75
   { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L 0x4C  76
   { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M 0x4D  77
   { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N 0x4E  78
   { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O 0x4F  79
   { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P 0x50  80
   { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q 0x51  81
   { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R 0x52  82
   { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S 0x53  83
   { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T 0x54  84
   { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U 0x55  85
   { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V 0x56  86
   { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W 0x57  87
   { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X 0x58  88
   { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y 0x59  89
   { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z 0x5A  90
   { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [ 0x5B  91
   { 0x02, 0x04, 0x08, 0x10, 0x20 },   // \ 0x5C  92
   { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ] 0x5D  93
   { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^ 0x5E  94
   { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _ 0x5F  95
   { 0x00, 0x01, 0x02, 0x04, 0x00 },   // ` 0x60  96
   { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a 0x61  97
   { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b 0x62  98
   { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c 0x63  99
   { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d 0x64 100
   { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e 0x65 101
   { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f 0x66 102
   { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g 0x67 103
   { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h 0x68 104
   { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i 0x69 105
   { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j 0x6A 106
   { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k 0x6B 107
   { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l 0x6C 108
   { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m 0x6D 109
   { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n 0x6E 110
   { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o 0x6F 111
   { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p 0x70 112
   { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q 0x71 113
   { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r 0x72 114
   { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s 0x73 115
   { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t 0x74 116
   { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u 0x75 117
   { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v 0x76 118
   { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w 0x77 119
   { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x 0x78 120
   { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y 0x79 121
   { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z 0x7A 122
   { 0x00, 0x08, 0x36, 0x41, 0x00 },   // { 0x7B 123
   { 0x00, 0x00, 0x7F, 0x00, 0x00 },   // | 0x7C 124
   { 0x00, 0x41, 0x36, 0x08, 0x00 },   // } 0x7D 125
   { 0x08, 0x04, 0x08, 0x10, 0x08 },   // ~ 0x7E 126
   { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },   //  0x7F 127

   { 0x7C, 0x12, 0x11, 0x12, 0x7C },   // А 0xC0 192
   { 0x7F, 0x49, 0x49, 0x49, 0x31 },   // Б 0xC1 193
   { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // В 0xC2 194
   { 0x7F, 0x01, 0x01, 0x01, 0x01 },   // Г 0xC3 195
   { 0x60, 0x3F, 0x21, 0x3F, 0x60 },   // Д 0xC4 196
   { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // Е 0xC5 197
   { 0x77, 0x08, 0x7F, 0x08, 0x77 },   // Ж 0xC6 198
   { 0x22, 0x41, 0x49, 0x49, 0x36 },   // З 0xC7 199
   { 0x7F, 0x10, 0x08, 0x04, 0x7F },   // И 0xC8 200
   { 0x7E, 0x10, 0x09, 0x04, 0x7E },   // Й 0xC9 201
   { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // К 0xCA 202
   { 0x40, 0x3E, 0x01, 0x01, 0x7F },   // Л 0xCB 203
   { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // М 0xCC 204
   { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // Н 0xCD 205
   { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // О 0xCE 206
   { 0x7F, 0x01, 0x01, 0x01, 0x7F },   // П 0xCF 207
   { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // Р 0xD0 208
   { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // С 0xD1 209
   { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // Т 0xD2 210
   { 0x07, 0x48, 0x48, 0x48, 0x3F },   // У 0xD3 211
   { 0x0E, 0x11, 0x7F, 0x11, 0x0E },   // Ф 0xD4 212
   { 0x63, 0x14, 0x08, 0x14, 0x63 },   // Х 0xD5 213
   { 0x3F, 0x20, 0x20, 0x3F, 0x60 },   // Ц 0xD6 214
   { 0x07, 0x08, 0x08, 0x08, 0x7F },   // Ч 0xD7 215
   { 0x7F, 0x40, 0x7E, 0x40, 0x7F },   // Ш 0xD8 216
   { 0x3F, 0x20, 0x3F, 0x20, 0x7F },   // Щ 0xD9 217
   { 0x01, 0x7F, 0x48, 0x48, 0x30 },   // Ъ 0xDA 218
   { 0x7F, 0x48, 0x30, 0x00, 0x7F },   // Ы 0xDB 219
   { 0x00, 0x7F, 0x48, 0x48, 0x30 },   // Ь 0xDC 220
   { 0x22, 0x41, 0x49, 0x49, 0x3E },   // Э 0xDD 221
   { 0x7F, 0x08, 0x3E, 0x41, 0x3E },   // Ю 0xDE 222
   { 0x46, 0x29, 0x19, 0x09, 0x7F },   // Я 0xDF 223
   { 0x20, 0x54, 0x54, 0x54, 0x78 },   // а 0xE0 224
   { 0x3C, 0x4A, 0x4A, 0x4A, 0x31 },   // б 0xE1 225
   { 0x7C, 0x54, 0x54, 0x28, 0x00 },   // в 0xE2 226
   { 0x7C, 0x04, 0x04, 0x0C, 0x00 },   // г 0xE3 227
   { 0x60, 0x3C, 0x24, 0x3C, 0x60 },   // д 0xE4 228
   { 0x38, 0x54, 0x54, 0x54, 0x18 },   // е 0xE5 229
   { 0x6C, 0x10, 0x7C, 0x10, 0x6C },   // ж 0xE6 230
   { 0x00, 0x44, 0x54, 0x54, 0x28 },   // з 0xE7 231
   { 0x7C, 0x20, 0x10, 0x08, 0x7C },   // и 0xE8 232
   { 0x7C, 0x21, 0x12, 0x09, 0x7C },   // й 0xE9 233
   { 0x7C, 0x10, 0x28, 0x44, 0x00 },   // к 0xEA 234
   { 0x40, 0x38, 0x04, 0x04, 0x7C },   // л 0xEB 235
   { 0x7C, 0x08, 0x10, 0x08, 0x7C },   // м 0xEC 236
   { 0x7C, 0x10, 0x10, 0x10, 0x7C },   // н 0xED 237
   { 0x38, 0x44, 0x44, 0x44, 0x38 },   // о 0xEE 238
   { 0x7C, 0x04, 0x04, 0x04, 0x7C },   // п 0xEF 239
   { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // р 0xF0 240
   { 0x38, 0x44, 0x44, 0x44, 0x00 },   // с 0xF1 241
   { 0x04, 0x04, 0x7C, 0x04, 0x04 },   // т 0xF2 242
   { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // у 0xF3 243
   { 0x08, 0x14, 0x7C, 0x14, 0x08 },   // ф 0xF4 244
   { 0x44, 0x28, 0x10, 0x28, 0x44 },   // х 0xF5 245
   { 0x3C, 0x20, 0x20, 0x3C, 0x60 },   // ц 0xF6 246
   { 0x0C, 0x10, 0x10, 0x10, 0x7C },   // ч 0xF7 247
   { 0x7C, 0x40, 0x7C, 0x40, 0x7C },   // ш 0xF8 248
   { 0x3C, 0x20, 0x3C, 0x20, 0x7C },   // щ 0xF9 249
   { 0x04, 0x7C, 0x50, 0x50, 0x20 },   // ъ 0xFA 250
   { 0x7C, 0x50, 0x20, 0x00, 0x7C },   // ы 0xFB 251
   { 0x00, 0x7C, 0x50, 0x50, 0x20 },   // ь 0xFC 252
   { 0x28, 0x44, 0x54, 0x54, 0x38 },   // э 0xFD 253
   { 0x7C, 0x10, 0x38, 0x44, 0x38 },   // ю 0xFE 254
   { 0x48, 0x54, 0x34, 0x14, 0x7C }    // я 0xFF 255
};

#endif  /*  _N3310_H_ */