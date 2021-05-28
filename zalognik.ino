#include "libraries.h"

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(9600);
  
  DEBUG_LN(F("Start"));
  
  //Акселерометр
  Wire.begin();
  mpu.initialize();
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

  // Иницилизация LCD
  lcd.begin();
  lcd.backlight();

  // Определение параметра подавления дребезга контактов на клавиатуре
  keypad.begin();

  //провода деактивации
  for (uint8_t i = 0; i < WIRE_PINS_COUNT; ++i)
  {
    wires[i].SetPin(WIRE_PINS[i]);
  }

  showHello();  //Запуск меню приветствия.
  
  //Плеер
  delay(1000);
  
  audio.begin(Serial);
  delay(2000);
  audio.setTimeOut(500);
  audioConnected = 1;
  audio.volume(AUDIO_VOLUME);
  audio.EQ(DFPLAYER_EQ_NORMAL);
  audio.outputDevice(DFPLAYER_DEVICE_SD);
  audio.enableDAC();
  pinMode(PLAY, INPUT_PULLUP);

  PlaySound(TREK1);
  
  //Реле
  pinMode(RELAY_GAME_OVER, OUTPUT);
  digitalWrite(RELAY_GAME_OVER, ON);

  //Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  if (EEPROM.read(0) != 255 && EEPROM.read(0) > 0)
  {
    DEBUG_LN(F("Memory start"));
    int cellEeprom = 0;
    for (uint8_t i = 0; i < adress; ++i)
    {
      if (i == 0 || i == 1 || i == (adress - 1)) {
        setupGame[i] = EEPROMReadlong(cellEeprom);
        cellEeprom += 3;
      }
      else
      {
        setupGame[i] = EEPROM.read(cellEeprom);
      }
      DEBUG_LN(setupGame[i]);
      ++cellEeprom;
      delay(1);
    }
    
    DEBUG_LN(F("Memory end"));
    globalState = 11;
    ShowSave();    //Меню сохраненные настройки
  }
  else
  {
    showTextBombTime();   //Меню установки времени игры
  }
}

void loop()
{
  switch (globalState)
  {
    case 0: SetupBombTime(); break;           //Установка времени игры
    case 1: SetupPassword(); break;           //Ввод пароля
    case 2: SetupAttempts(); break;           //Количество попыток ввода пароля
    case 3: SetupIncorrectToogle(); break;    //Сколько минут отнять
    case 4: SetupStopTime(); break;           //Заморозка таймера
    case 5: SetupSlomoTime(); break;          //Во сколько раз медленее будет отсчет.
    case 6: SetupTimeEfect(); break;          //Сколько минут будет действовать этот эффект.
    case 7: SetupGame(); break;               //Выбор режима игры.
    case 8: SetupSensitivity(); break;        //Чуствительность акселерометра.
    case 9: SetupSensitivityTime(); break;    //Во сколько раз должен ускориться отсчет при срабатывании модуля.
    case 10: SetupSensitivityEfect(); break;  //Отрезок времени для ускорения.
    case 11: SetupSave(); break;              //Запрос восстановление данных
    case 12: SetupAnyPress(); break;          //Ожидания старта
    case 13: timerGame(); break;              //Начало игры
    case 14: GameOver(); break;               //Конец игры Поражение
    case 15: GameWin(); break;                //Конец игры Победа
    case 16: ReleGameOver(); break;           //Реле Game Over
  }
  buzzer();
  delay(1);
}
