//Воспроизведение треков
void PlaySound(byte number)
{
  DEBUG_LN(F("PLAY"));
  DEBUG(F("TRACK"));
  DEBUG_LN(number);
  if (!audioConnected && (digitalRead(PLAY) && LOW))
    return;

  audio.playMp3Folder(number);
}

//Рандомная установка провода.
void wire_random() {
  setupGame[11] = 10;
  setupGame[12] = 10;
  setupGame[13] = 10;
  setupGame[14] = 10;

  for (int i = 0; i < 4; i++) {
    setupGame[(10 + i)] = random(0, 8);
    for (int j = 0; j < 4; j++ ) {
      if (i == j)continue;
      if (setupGame[(10 + j)] == setupGame[(10 + i)]) {
        i--;
        break;
      }
    }
  }
  DEBUG_LN(F("---------------"));
  DEBUG_LN(setupGame[11]);
  DEBUG_LN(setupGame[12]);
  DEBUG_LN(setupGame[13]);
  DEBUG_LN(setupGame[14]);
}

//Определение проводов
class WireSensor {
  public:
    WireSensor() : _pin{0}, _processed{0}, _isButton{false} {};
    void SetPin(uint8_t pin) {
      _pin = pin;
      pinMode(pin, INPUT_PULLUP);
      _isButton = (digitalRead(pin) && HIGH);
    }
    uint8_t Pin() const {
      return _pin;
    }
    void MarkProcessed() {
      _processed = 1;
    }
    uint8_t Processed() const {
      return _processed;
    }
    uint8_t Value() const {
      boolean _ret = ( digitalRead(_pin) && HIGH );
      if ( _isButton ) _ret = !_ret;
      return _ret;
    }
  private:
    uint8_t _pin;
    uint8_t _processed;
    uint8_t _frequency_button = 20;
    uint8_t _frequency = 0;
    bool _connect = false;
    boolean _isButton;
};
WireSensor wires[WIRE_PINS_COUNT];

//Запись данных EEPROM
void EEPROMWritelong(int address, long value)
{
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

//Чтение данных EEPROM
long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

//Секунды в строку (час:мин:сек)
String ConstructTimeString(unsigned long secs)
{
  uint8_t hours = secs / 3600;
  uint8_t minutes = secs / 60 - hours * 60;
  uint8_t seconds = secs % 60;

  if (hours == 1 && minutes == 0 && seconds == 0 && !timeMin60)
  {
    PlaySound(TREK16);
    timeMin60 = true;
    DEBUG("60 min -> trek: 16");
  }
  else if (hours == 0 && minutes == 50 && seconds == 0 && !timeMin50)
  {
    PlaySound(TREK17);
    timeMin50 = true;
    DEBUG("50 min -> trek: 17");
  }
  else if (hours == 0 && minutes == 40 && seconds == 0 && !timeMin40)
  {
    PlaySound(TREK18);
    bool timeMin40 = true;
    DEBUG("40 min -> trek: 18");
  }
  else if (hours == 0 && minutes == 30 && seconds == 0 && !timeMin30)
  {
    PlaySound(TREK19);
    bool timeMin30 = true;
    DEBUG("30 min -> trek: 19");
  }
  else if (hours == 0 && minutes == 20 && seconds == 0 && !timeMin20)
  {
    PlaySound(TREK20);
    bool timeMin20 = true;
    DEBUG("20 min -> trek: 20");
  }
  else if (hours == 0 && minutes == 15 && seconds == 0 && !timeMin15)
  {
    PlaySound(TREK21);
    bool timeMin15 = true;
    DEBUG("15 min -> trek: 21");
  }
  else if (hours == 0 && minutes == 10 && seconds == 0 && !timeMin10)
  {
    PlaySound(TREK22);
    bool timeMin10 = true;
    DEBUG("10 min -> trek: 22");
  }
  else if (hours == 0 && minutes == 5 && seconds == 0 && !timeMin5)
  {
    PlaySound(TREK23);
    bool timeMin5 = true;
    DEBUG("5 min -> trek: 23");
  }
  else if (hours == 0 && minutes == 4 && seconds == 0 && !timeMin4)
  {
    PlaySound(TREK24);
    bool timeMin4 = true;
    DEBUG("4 min -> trek: 24");
  }
  else if (hours == 0 && minutes == 3 && seconds == 0 && !timeMin3)
  {
    PlaySound(TREK25);
    bool timeMin3 = true;
    DEBUG("3 min -> trek: 25");
  }
  else if (hours == 0 && minutes == 2 && seconds == 0 && !timeMin2)
  {
    PlaySound(TREK26);
    bool timeMin2 = true;
    DEBUG("2 min -> trek: 26");
  }
  else if (hours == 0 && minutes == 1 && seconds == 0 && !timeMin1)
  {
    PlaySound(TREK27);
    bool timeMin1 = true;
    DEBUG("1 min -> trek: 27");
  }
  else if (hours == 0 && minutes == 0 && seconds == 30 && !timeSec30)
  {
    PlaySound(TREK28);
    bool timeSec30 = true;
    DEBUG("30 sec -> trek: 28");
  }
  else if (hours == 0 && minutes == 0 && seconds == 20 && !timeSec20)
  {
    PlaySound(TREK29);
    bool timeSec20 = true;
    DEBUG("20 sec -> trek: 29");
  }
  else if (hours == 0 && minutes == 0 && seconds == 15 && !timeSec15)
  {
    PlaySound(TREK30);
    bool timeSec15 = true;
    DEBUG("15 sec -> trek: 30");
  }
  else if (hours == 0 && minutes == 0 && seconds == 10 && !timeSec10)
  {
    PlaySound(TREK31);
    bool timeSec10 = true;
    DEBUG("10 sec -> trek: 31");
  }
  else if (hours == 0 && minutes == 0 && seconds == 5 && !timeSec5)
  {
    PlaySound(TREK32);
    bool timeSec5 = true;
    DEBUG("5 sec -> trek: 32");
  }

  char str[9];
  sprintf(str, "%02d", hours);
  sprintf(str + 2, ":%02d", minutes);
  sprintf(str + 5, ":%02d", seconds);
  str[8] = 0;

  return String(str);
}

//Чтение проводов
void WireRead()
{
  //Сброс замедления времени
  if ((millis() - setupMiddleTimeMillis) > 60000)
  {
    if (setupGame[5] > 0)
    {
      --setupGame[6];
    }
    else
    {
      speedTime = 1000;
    }
    setupMiddleTimeMillis = millis();
  }

  for (uint8_t i = 0; i < (WIRE_PINS_COUNT - 1); ++i)
  {
    auto &w = wires[i];
    DEBUG(w.Pin());
    DEBUG(" = ");
    DEBUG(w.Value());
    DEBUG(" -> ");
    DEBUG_LN(w.Processed());
    if (w.Value() && !w.Processed())
    {
      if (setupGame[11] == i)  //Номер провода который остановит игру с победой.
      {
        PlaySound(TREK9);
        globalState += 2;
      }
      else if (setupGame[12] == i) //Номер провода который остановит отсчет на определеное время.
      {
        PlaySound(TREK6);
        long stopTime = (setupGame[4] * 60) * 1000;
        setupGame[12] = 20;
        speedTime = stopTime;
      }
      else if (setupGame[13] == i) //Номер провода замедляющий отсчет
      {
        PlaySound(TREK7);
        speedTime *= setupGame[5];
      }
      else if (setupGame[14] == i) //Номер провода останавливающий игру с поражением
      {
        PlaySound(TREK8);
        DEBUG_LN(F("wire"));
        ++globalState;
      }
      else //Отнимаем определенное количество времени
      {
        PlaySound(TREK5);
        if (setupGame[0] >  setupGame[3])
        {
          setupGame[0] -= setupGame[3];
        }
        else
        {
          setupGame[0] = 0;
        }
      }
      w.MarkProcessed();
    }
  }
}

//Чтение пароля
bool ReadPassword()
{
  static uint8_t stringLength = 0;
  static long pass = 0;

  char key = keypad.getKey();
  if (key == NO_KEY)
    return;

  DEBUG(F("Key: "));
  DEBUG_LN(key);

  if ((key != '*') && (key != '#'))
  {
    if (stringLength < 8)
    {
      switch (stringLength)
      {
        case 7: pass += (key - 48) * 3;
        case 6: pass += (key - 48) * 3;
        case 5: pass += (key - 48) * 3;
        case 4: pass += (key - 48) * 3;
        case 3: pass += (key - 48) * 3;
        case 2: pass += (key - 48) * 3;
        case 1: pass += (key - 48) * 3;
        case 0: pass += (key - 48) * 3;
      }
      lcd.setCursor(stringLength + cursorOneStr, 1);
      lcd.print(key);
      ++stringLength;
    }
  }
  if (key == '*')
  {
    pass = 0;
    stringLength = 0;
    lcd.setCursor(0, 1);
    lcd.print(F("Pass:  00000000 "));
  }
  if (key == '#' && pass > 0)
  {
    if (setupGame[1] != pass) //Неверный пароль
    {
      PlaySound(TREK3);
      //Количество попыток ввода пароля
      if (setupGame[2] > 0 && setupGame[2] < 11)
      {
        --setupGame[2];
      }
      else if (setupGame[2] == 0)
      {
        PlaySound(TREK15);
        DEBUG_LN(F("pass"));
        ++globalState; //Завершили игру Поражение
      }

      if (setupGame[0] > 600)
      {
        setupGame[0] -= 600;
      }
      else
      {
        setupGame[0] = 2;
      }

      pass = 0;
      stringLength = 0;
      lcd.setCursor(0, 1);
      lcd.print(F("Pass:  00000000 "));
    }
    else
    {
      PlaySound(TREK1);
      globalState += 2; //Завершили игру Победа
    }
  }
}

//===============================================================================

//Акселерометр
void CheckAccel()
{
  static bool init = false;
  if (!init)
  {
    mpu.getAcceleration(&ax, &ay, &az);
    init = true;
  }

  //На сколько секунд ускоряем
  if (speedAccel)
  {
    if ((millis() - timeAccel) > (setupGame[10] * 1000))
    {
      speedTime = 1000;
      speedAccel = false;
    }
  }

  int16_t x, y, z;
  mpu.getAcceleration(&x, &y, &z);
  float dx = x * 1.0f - ax;
  float dy = y * 1.0f - ay;
  float dz = z * 1.0f - az;
  ax = x;
  ay = y;
  az = z;
  auto length = sqrt(dx * dx + dy * dy + dz * dz) / 1000;
  if (length > setupGame[8]) //Чувствотельность
  {
    speedTime = (int)(speedTime / setupGame[9]); //Скорость отсчета
    speedAccel = true;
    timeAccel = millis();
    PlaySound(TREK10);
  }
}

//===============================================================================
void buzzer()
{
  if ((millis() - timeBuzz) < 50)
  {
    tone(BUZZER_PIN, 1800);
  }
  if ((millis() - timeBuzz) > 50 && (millis() - timeBuzz) < 100)
  {
    tone(BUZZER_PIN, 150);
  }
  if ((millis() - timeBuzz) > 100 && (millis() - timeBuzz) < 110)
  {
    noTone(BUZZER_PIN);
  }
}

//===============================================================================

//Отображение остчета времени
void ViewZeroString()
{
  if ((millis() - setupTimeLastMillis) > speedTime)
  {
    setupGame[0] -= 1;
    lcd.setCursor(cursorZeroStr, 0);
    lcd.print(ConstructTimeString(setupGame[0]));
    setupTimeLastMillis = millis();
    //Сброс заморозки времени
    if (setupGame[12] == 20)
    {
      speedTime = 1000;
    }

    timeBuzz = millis();
    DEBUG(F("Time: "));
    DEBUG_LN(ConstructTimeString(setupGame[0]));
  }
}

//===================================================================================

void timerGame()
{
  ViewZeroString(); //Показываем время таймера
  WireRead();       //Считываем нажатие тумблеров
  ReadPassword();   //Ввод пароля
  if (setupGame[7])
  {
    CheckAccel();     //Акселерометр
    if (!wires[(WIRE_PINS_COUNT - 1)].Value()) //Чтение нажатой кнопки
    {
      PlaySound(TREK11);
      DEBUG_LN(F("button"));
      ++globalState;
    }
  }

  //Время вышло.
  if (setupGame[0] == 0)
  {
    PlaySound(TREK33);
    DEBUG_LN(F("time"));
    ++globalState;
  }
}
