//LED
void led()
{
  if ((millis() - blinkLedColor) > 1000)
  {
    if (blinkLed)
    {
      blinkLed = false;
      if (colorNow == 2) {
        if (yellow > 2)
        {
          colorNow = 4;
          yellow = 0;
        }
        yellow++;
      }
    }
    else
    {
      blinkLed = true;
    }
    blinkLedColor = millis();
  }

  if (ledNumber > NUM_LEDS)ledNumber = 0;

  if (blinkLed)
  {
    strip.setPixelColor(ledNumber, colorLed[4]);
  }
  else
  {
    strip.setPixelColor(ledNumber, colorLed[colorNow]);
  }

  strip.show();
  ledNumber++;
}

//Рандомная установка провода.
void wire_random() {
  setupGame[10] = 10;
  setupGame[11] = 10;
  setupGame[12] = 10;
  setupGame[13] = 10;

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
}

//Определение проводов
class WireSensor {
  public:
    WireSensor() : _pin{0}, _processed{0} {};
    void SetPin(uint8_t pin) {
      _pin = pin;
      pinMode(pin, INPUT_PULLUP);
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
    uint8_t Value() {
      if (digitalRead(_pin) == HIGH && !_connect)
      {
        if (_frequency > _frequency_button)
        {
          _connect = true;
          _frequency = 0;
          return true;
        }
      }
      if (digitalRead(_pin) == LOW)
      {
        _connect = false;
        _frequency = 0;
      }
      ++_frequency;
      return false;
    }
  private:
    uint8_t _pin;
    uint8_t _processed;
    uint8_t _frequency_button = 20;
    uint8_t _frequency = 0;
    bool _connect = false;
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

  for (uint8_t i = 0; i < WIRE_PINS_COUNT; ++i)
  {
    auto &w = wires[i];
    if (w.Value() && !w.Processed())
    {
      if (setupGame[10] == i)  //Номер провода который остановит игру с победой.
      {
        globalState += 2;
      }
      else if (setupGame[11] == i) //Номер провода который остановит отсчет на определеное время.
      {
        speedTime = setupGame[4] * 60000;
        colorNow = 2;
        ledNumber = 0;
      }
      else if (setupGame[12] == i) //Номер провода замедляющий отсчет
      {
        speedTime *= setupGame[5];
        colorNow = 2;
        ledNumber = 0;
      }
      else if (setupGame[13] == i) //Номер провода останавливающий игру
      {
        ++globalState;
      }
      else //Отнимаем определенное количество времени
      {
        if (setupGame[0] > setupGame[3])
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
void ReadPassword()
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
    if (setupGame[1] != pass)
    {
      //Количество попыток ввода пароля
      if (setupGame[2] > 0 && setupGame[2] < 11)
      {
        --setupGame[2];
      }
      else if (setupGame[2] == 0)
      {
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
      colorNow = 2;
      ledNumber = 0;
    }
    else
    {
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
    if ((millis() - timeAccel) > (setupGame[9] * 1000))
    {
      speedTime = 1000;
      speedAccel = false;
    }
    colorNow = 2;
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
  if (length > setupGame[7]) //Чувствотельность
  {
    speedTime = (int)(speedTime / setupGame[8]); //Скорость отсчета
    speedAccel = true;
    timeAccel = millis();
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
  CheckAccel();     //Акселерометр
  ReadPassword();   //Ввод пароля

  //Время вышло.
  if (setupGame[0] == 0)
  {
    ++globalState;
  }
}
