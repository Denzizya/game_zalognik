#define DEBUG_ENABLE 1
#ifdef DEBUG_ENABLE
#define DEBUG_LN(x) Serial.println(x)
#define DEBUG(x) Serial.print(x)
#else
#define DEBUG_LN(x)
#define DEBUG(x)
#endif
/*** Клавиатура ***/
#define I2C_KEYPAD_ADDR 0x20
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 3
char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
uint8_t KEYPAD_ROW_PINS[KEYPAD_ROWS] = {1, 2, 3, 4};
uint8_t KEYPAD_COL_PINS[KEYPAD_COLS] = {5, 6, 7};
Keypad_I2C keypad(makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COL_PINS,
                  KEYPAD_ROWS, KEYPAD_COLS, I2C_KEYPAD_ADDR);

/*** LCD ***/
#define I2C_LCD_ADDR 0x27
LiquidCrystal_I2C lcd(I2C_LCD_ADDR, 16, 2);

/*** Провода ***/
uint8_t WIRE_PINS[] = {2,3,4,5,6,7,8,9,10};
const uint8_t WIRE_PINS_COUNT = sizeof(WIRE_PINS) / sizeof(uint8_t);

//Акселерометр
MPU6050 mpu(0x68);
int16_t ax, ay, az;
long timeAccel;
bool speedAccel = false;

/*** RELE ***/
#define ON HIGH
#define OFF LOW
#define RELAY_GAME_OVER 11
unsigned long releEndTime;  //Метка времени сработки реле

/*** BUZZER ***/
#define BUZZER_PIN 12
//bool buzz = true;   //разрешение проигрование сигнала
unsigned long timeBuzz;

/*** LED ***/
#define PIN 13       // пин DI
#define NUM_LEDS 63   // число диодов
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
unsigned long timeLastLed = millis();  //Метка времени перезалива цвета
unsigned long blinkLedColor = millis();  //Метка времени мигания
//                   синий     зелен.    желтый    красн.    черный
long colorLed[5] = {0x0000ff, 0x00ff00, 0xff8400, 0xff0000, 0x000000};  //Цвета используемые.
uint8_t ledNumber = 0;  //С какого LED стартовать
uint8_t colorNow = 0;   //Начальный цвет (синий)
bool blinkLed = true;   //Смена цвета
uint8_t yellow = 0;     //Мигание 3 раза желтым

/*** Timer ***/
unsigned long setupTimeLastMillis;	//Метка времени
unsigned long speedTime = 1000;		  //Скорость таймера
unsigned long setupMiddleTimeMillis;//Таймер замедления

/**************************************************************/
uint8_t globalState = 0;          //Пункт меню
bool globalStateButton = false;   //Вывод меню
const uint8_t adress = 14;        //Количество параметров
long setupGame[adress];           //Сохранение параметров
uint8_t globalTimer = 0;          //Таймер установки времени

uint8_t cursorZeroStr = 0;  //Курсор нулевой строки
uint8_t cursorOneStr = 0;   //Курсор первой строки
