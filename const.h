#define DEBUG_ENABLE
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
uint8_t WIRE_PINS[] = {2,3,4,5,6,7,8,9,10,11,12};
const uint8_t WIRE_PINS_COUNT = sizeof(WIRE_PINS) / sizeof(uint8_t);

//Акселерометр
MPU6050 mpu(0x68);
int16_t ax, ay, az;
long timeAccel;
bool speedAccel = false;

//Плеер
DFRobotDFPlayerMini audio{};
#define AUDIO_VOLUME 30 // min = 0, max = 30
#define PLAY A1 //воспроизведение трека.
bool audioConnected = true;

/*** RELE ***/
#define ON HIGH
#define OFF LOW
#define RELAY_GAME_OVER A2
unsigned long releEndTime;  //Метка времени сработки реле

/*** BUZZER ***/
#define BUZZER_PIN 13
unsigned long timeBuzz;

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

int start_button = 3;

unsigned long timeDownButton;

/***PLAY TIME***/
bool timeMin60 = false;
bool timeMin50 = false;
bool timeMin40 = false;
bool timeMin30 = false;
bool timeMin20 = false;
bool timeMin15 = false;
bool timeMin10 = false;
bool timeMin5 = false;
bool timeMin4 = false;
bool timeMin3 = false;
bool timeMin2 = false;
bool timeMin1 = false;

bool timeSec30 = false;
bool timeSec20 = false;
bool timeSec15 = false;
bool timeSec10 = false;
bool timeSec5 = false;
