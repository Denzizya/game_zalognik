#include "libraries.h"
#include "const.h"
#include "functions.h"
#include "menu.h"

void setup()
{
  Serial.begin(9600);

  Serial.println("Start");

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

  //Реле
  pinMode(RELAY_GAME_OVER, OUTPUT);
  digitalWrite(RELAY_GAME_OVER, LOW);

  //Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  //LED

  strip.begin();
  strip.setBrightness(50);    // яркость, от 0 до 255
  strip.show();               // отправить на ленту

  delay(3000);

  if (EEPROM.read(0) != 255 && EEPROM.read(0) > 0)
  {
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
      ++cellEeprom;
      delay(1);
    }

    globalState = 10;
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
    case 7: SetupSensitivity(); break;        //Чуствительность акселерометра.
    case 8: SetupSensitivityTime(); break;    //Во сколько раз должен ускориться отсчет при срабатывании модуля.
    case 9: SetupSensitivityEfect(); break;   //Отрезок времени для ускорения.
    case 10: SetupSave(); break;              //Запрос восстановление данных
    case 11: SetupAnyPress(); break;          //Ожидания старта
    case 12: timerGame(); break;              //Начало игры
    case 13: GameOver(); break;               //Конец игры Поражение
    case 14: GameWin(); break;                //Конец игры Победа
    case 15: ReleGameOver(); break;           //Реле Game Over
  }
  led();
  delay(1);
}
