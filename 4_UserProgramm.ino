
//---/// Обязательные
void readEEPROM() {
  EEPROM.begin(150);
  EEPROM.get(_EEPROM_START, lp);
}
//---///////////////////////////////////
void writeEEPROM() {
  EEPROM.put(_EEPROM_START, lp);  // сохраняем
  EEPROM.commit();
}
//---/// Обязательные
//***************************************************//


void Button() {
  // проверка нажатия кнопки
  if (digitalRead(_BUTTON_PIN) == HIGH)  //Когда не нажата кнопка
  {
    if (BUTTON_STATUS == true) {  // Отпускаем
      ////////////Serial.println("BUTTON_STATUS= false");
      BUTTON_STATUS = false;
      // выключить свет
      Relay_ON_OFF(0);
      //sendMQTT_relay();
      BUTTON_HOLD_MILLIS = millis() - 1000;  // защита от дребезга, следующая проверка через 1 с
    }
  } else  //Когда  нажата кнопка
  {
    if (BUTTON_STATUS == false) {  // Нажимаем
      BUTTON_STATUS = true;
      // включить свет
      Relay_ON_OFF(1);
      // sendMQTT_relay();
      BUTTON_HOLD_MILLIS = millis() - 1000;  // защита от дребезга, следующая проверка через 1 с
    }
  }
}

void Tm_Hig() {
  // измеряем влажность 1 раз в минуту
  // меняем последовательно порт опроса - выбираем поочереди датчики
  float t;
  float h;
  float tempC;

  DS18B20.requestTemperatures();
  //  датчик DHT11
  for (int i = 0; i < 6; i++) {
    h = dht11.readHumidity();
    t = dht11.readTemperature();
    if (!(isnan(h) || isnan(t))) {
      HUMIDITY[0] = int(h);
      TEMPERATURE[0] = int(t * 10);
      ////////////Serial.print("t =");
      ////////////Serial.print(TEMPERATURE[0]);
      break;
    }
    delay(200);
    ////////////Serial.print("i =");
    ////////////Serial.println(i);
  }
  ////////////Serial.print("dht11: T =");
  ////////////Serial.print(TEMPERATURE[0]);
  ////////////Serial.print("; H =");
  ////////////Serial.println(HUMIDITY[0]);
  /////

  tempC = DS18B20.getTempCByIndex(0);
 
  if (tempC != 85.0 and tempC != (-127.0)) {
    TEMPERATURE[1] = int(tempC * 10);
  }  else TEMPERATURE[1] = 888;
  

  ///
  ////////////Serial.print("DS18B20: =");
  ////////////Serial.print(tempC);
  ////////////Serial.print("  T =");
  ////////////Serial.println(TEMPERATURE[1]);

  //  ///!Заглушка !!! Вставить проверку на 1000 мс с момента запроса измерения!!!
  //  delay (700);
  //  tempC = DS18B20.getTempCByIndex(0);
  //  if (tempC != 85.0 and tempC != (-127.0) ) TEMPERATURE[1] = int(tempC * 10);
  //  ///
  //  ////////////Serial.print("DS18B20: =");
  //  ////////////Serial.print(tempC);
  //  ////////////Serial.print("  T =");
  //  ////////////Serial.println(TEMPERATURE[1]);

  /*
    TEMPERATURE[0] = 12;
    HUMIDITY[0] = 91;
    TEMPERATURE[1] = -50;
    HUMIDITY[1] = 90;
  */
}
//---////////////////////////////////////////
void Logika() {
  if (FLAG_EXTERNAL or TEMPERATURE[1]==888) {
    return;  //внешнее управление или неудачное измерение
  }


  switch (STATUS_ZADVIGKA) {

    case _STATUS_OPEN:

      if (TEMPERATURE[1] < _TEMPERATURE_Z) {

        if (TEMPERATURE[1] > _TEMPERATURE_CLOSE) {
          //прикрыть
          open_slightly_zaslonka();
        } else {
          //закрыть
          close_zaslonka();
        }
      }
      break;
    case _STATUS_CLOSE:
      if (TEMPERATURE[1] > _TEMPERATURE_Z) {
        if (TEMPERATURE[1] > _TEMPERATURE_OPEN) {
          //открыть
          open_zaslonka();
        } else {
          //приоткрыть
          open_slightly_zaslonka();
        }
      }
      break;
    case _STATUS_Z:
      if (TEMPERATURE[1] > _TEMPERATURE_OPEN) {
        //открыть
        open_zaslonka();
      } else {
        if (TEMPERATURE[1] < _TEMPERATURE_CLOSE) {

          //закрыть
          close_zaslonka();
        }
      }
      break;
      // case 4:
      //   digitalWrite(_RELAY2, LOW);  //Realy Close
      //   RELAY2_STATUS = false;
      //   break;
  }

  if (RELAY2_STATUS)  // реле включено
  {
    if (TEMPERATURE[1] >= _TEMPERATURE_MAX) {
      Relay_ON_OFF(4);
      bot.sendMessage("Выключить обогреватель");
    }
  } else  //реле выключено
  {
    if (TEMPERATURE[1] <= _TEMPERATURE_MIN) {
      Relay_ON_OFF(3);
      bot.sendMessage("Включить обогреватель");
    }
  }
}


//-----------------------------------------------------------
void Relay_ON_OFF(byte on_off) {

  // ON_OFF = 0 -выключить реле № 1.
  // ON_OFF = 1 - включить реле № 1.
  // ON_OFF = 3 - включить реле № 2. Обогреватель
  // ON_OFF = 4 -выключить реле № 2. Обогреватель


  switch (on_off) {

    case 0:
      digitalWrite(_RELAY1, LOW);  //Realy Close
      RELAY1_STATUS = false;
      break;
    case 1:
      digitalWrite(_RELAY1, HIGH);  //Realy Power
      RELAY1_STATUS = true;
      break;
    case 3:
      digitalWrite(_RELAY2, HIGH);  //Realy Power
      RELAY2_STATUS = true;
      break;
    case 4:
      digitalWrite(_RELAY2, LOW);  //Realy Close
      RELAY2_STATUS = false;
      break;
  }
  String chat = _CHAT_MY_ID;
  sendTelegramm_relay(chat);
  sendMQTT_relay();
}
//---//
//---Задвижка--------------------------------------------------------------
void set_cycle_Time() {

  /**/ bot.sendMessage("Запускаем попытку №" + String(FLAG_RUN_CYCLE + 1), _CHAT_MY_ID);
  if (FLAG_RUN_CYCLE > 2) 
  {
    /**/ bot.sendMessage("Вревышено количество попыток определения интервалов.  Статус задвижки не известен!!!", _CHAT_ID) ;
    STATUS_ZADVIGKA = _STATUS_ERROR;  //положение задвижки не определено!!!
   return;                            // т.е. было уже 3 попытки получить значение.
  }

  SHORT_PRESS_DURATION = 0;
  LONG_PRESS_DURATION = 0;
  CYCLE_DURATION = 0;

  startMotor();  // Запускаем двигатель    // Изначально реле включено (двигатель включаем)

  // Проверка, нажата ли кнопка при старте
  waitForPress();

  while (MOTOR_RUNNING == true) {
    // Пока двигатель работает
    time_intervals();
    delay(50);
  }
  //stopMotor();  // Двигатель уже Останавлен

  /**/ bot.sendMessage("Первое нажатие длилось: " + String(SHORT_PRESS_DURATION) + " мс", _CHAT_MY_ID);
  /**/ bot.sendMessage("Второе нажатие длилось: " + String(LONG_PRESS_DURATION) + " мс", _CHAT_MY_ID);
  /**/ bot.sendMessage("Время полного цикла: " + String(CYCLE_DURATION) + " мс", _CHAT_MY_ID);


  if (LONG_PRESS_DURATION > (SHORT_PRESS_DURATION * 2))  // короткое нажатие должно быть больше чем в 2 раза короче , чем длинное
  {
    STATUS_ZADVIGKA = _STATUS_CLOSE;
    FLAG_RUN_CYCLE = 0;
    /**/ bot.sendMessage("Измерение выполнено.", _CHAT_MY_ID);
  } else {
    FLAG_RUN_CYCLE++;
    /**/ bot.sendMessage("Ошибка измерения. Повторный запуск", _CHAT_MY_ID);
    delay(1000);
    set_cycle_Time();
  }
}
void time_intervals() {

  int buttonState = digitalRead(_PIN_SENSOR);

  // Если кнопка нажата (низкий уровень, так как используется INPUT_PULLUP)
  if (buttonState == LOW && !BUTTON_PRESSED) {
    BUTTON_PRESSED = true;
    ////////////Serial.println("Кнопка нажата . Ждём отпускания...");
    /**/  //bot.sendMessage("Кнопка нажата . Ждём отпускания...", _CHAT_MY_ID);

    LAST_PRESS_TIME = millis();
    PRESS_COUNT++;

    // Если это первое нажатие, запоминаем время начала цикла
    if (PRESS_COUNT == 1) {
      CYCLE_START_TIME = LAST_PRESS_TIME;
    }

    ////////////Serial.print("Нажатие #");
    ////////////Serial.println(PRESS_COUNT);
    /**/  //bot.sendMessage("Нажатие # " + String(PRESS_COUNT), _CHAT_MY_ID);
  }

  // Если кнопка отпущена
  if (buttonState == HIGH && BUTTON_PRESSED) {
    ////////////Serial.println("Кнопка  отжата");
    /**/  //bot.sendMessage("Кнопка  отжата", _CHAT_MY_ID);

    BUTTON_PRESSED = false;
    LAST_RELEASE_TIME = millis();

    // Рассчитываем длительность текущего нажатия
    unsigned long pressDuration = LAST_RELEASE_TIME - LAST_PRESS_TIME;

    // Запоминаем длительность нажатия
    if (PRESS_COUNT == 1) {
      SHORT_PRESS_DURATION = pressDuration;
      ////////////Serial.print("Первое нажатие длилось: ");
      ////////////Serial.print(SHORT_PRESS_DURATION);
      ////////////Serial.println(" мс");
      /**/  //bot.sendMessage("Первое нажатие длилось: " + String(SHORT_PRESS_DURATION) + " мс", _CHAT_MY_ID);

    } else if (PRESS_COUNT == 2) {
      LONG_PRESS_DURATION = pressDuration;
      ////////////Serial.print("Второе нажатие длилось: ");
      ////////////Serial.print(LONG_PRESS_DURATION);
      ////////////Serial.println(" мс");

      /**/  //bot.sendMessage("Второе нажатие длилось: " + String(LONG_PRESS_DURATION) + " мс", _CHAT_MY_ID);
      // Рассчитываем время полного цикла
      CYCLE_DURATION = (LAST_PRESS_TIME - CYCLE_START_TIME) * 2;
      ////////////Serial.print("Время полного цикла: ");
      ////////////Serial.print(CYCLE_DURATION);
      ////////////Serial.println(" мс");
      /**/  //bot.sendMessage("Время полного цикла: " + String(CYCLE_DURATION) + " мс", _CHAT_MY_ID);

      // Определяем, какое нажатие короткое, а какое длинное
      if (SHORT_PRESS_DURATION < LONG_PRESS_DURATION) {
        ////////////Serial.println("Первое нажатие короче второго.");
        /**/  //bot.sendMessage("Первое нажатие короче второго.", _CHAT_MY_ID);

        //  мы сейчас после длинного нажатия, крышка закрыта

      } else {
        ////////////Serial.println("Первое нажатие длиннее второго.");
        /**/  //bot.sendMessage("Первое нажатие длиннее второго.", _CHAT_MY_ID);
        // необходимо 1 нажатия, мы сейчас после короткого нажатия
        waitForNextPressToStopMotor();  // Ожидаем конца длинного  нажатия

        //приводим 1 нажатие короткое , второе длинное
        /**/  //bot.sendMessage("приводим 1 нажатие короткое , второе длинное", _CHAT_MY_ID);
        unsigned long temporaryPressDuration = SHORT_PRESS_DURATION;
        SHORT_PRESS_DURATION = LONG_PRESS_DURATION;  // Длительность первого нажатия
        LONG_PRESS_DURATION = temporaryPressDuration;
      }


      // Сбрасываем счетчик нажатий для следующего цикла
      PRESS_COUNT = 0;
      stopMotor();  // Останавливаем двигатель
    }
  }
}

// Функция для включения двигателя
void startMotor() {
  digitalWrite(_PIN_RELAY_OUT, HIGH);  //  HIGH Включаем реле (двигатель запускается)
  MOTOR_RUNNING = true;                // Обновляем состояние двигателя
                                       ////////////Serial.println("Двигатель запущен.");
  /**/ bot.sendMessage("Двигатель запущен.", _CHAT_MY_ID);
}

// Функция для остановки двигателя
void stopMotor() {
  digitalWrite(_PIN_RELAY_OUT, LOW);  // LOW  Выключаем реле (двигатель останавливается)
  MOTOR_RUNNING = false;              // Обновляем состояние двигателя
                                      ////////////Serial.println("Двигатель остановлен.");
  /**/ bot.sendMessage("Двигатель остановлен.", _CHAT_MY_ID);
}


// Функция для ожидания конца следующего  нажатия и проверяет длину нажатия
byte waitForNextPressToStopMotor() {
  // 1 - короткое нажатие
  // 2 длинное нажатие
  // 4 ошибка
  //если кнопка нажата, до



  ////////////Serial.print("Ожидание конца следующего  нажатия для ");

  while (true) {
    int buttonState = digitalRead(_PIN_SENSOR);
    delay(50);  // анидребезг
    // Если кнопка нажата
    if (buttonState == LOW && !BUTTON_PRESSED) {
      BUTTON_PRESSED = true;
      ////////////Serial.println(" Кнопка нажата");
      /**/  //bot.sendMessage(" Кнопка нажата", _CHAT_MY_ID);
      LAST_PRESS_TIME = millis();
    }
    // Если кнопка отпущена
    if (buttonState == HIGH && BUTTON_PRESSED) {
      BUTTON_PRESSED = false;
      LAST_RELEASE_TIME = millis();
      ////////////Serial.println(" Кнопка отпущена");
      /**/  //bot.sendMessage(" Кнопка отпущена", _CHAT_MY_ID);
      break;
    }
  }
  unsigned long temporaryPress = LAST_RELEASE_TIME - LAST_PRESS_TIME;
  ////////////Serial.print("Нажатие длилось: ");
  ////////////Serial.print(temporaryPress);
  ////////////Serial.println(" мс");
  /**/  //bot.sendMessage("Нажатие длилось: " + String(temporaryPress) + " мс", _CHAT_MY_ID);

  // Проверяем, является ли нажатие коротким (с учётом дельты)
  if (temporaryPress >= SHORT_PRESS_DURATION - DELTA && temporaryPress <= SHORT_PRESS_DURATION + DELTA) {
    /**/                  //bot.sendMessage("  Короткое нажатие", _CHAT_MY_ID);
    return _STATUS_OPEN;  // Короткое нажатие
  }
  // Проверяем, является ли нажатие длинным (с учётом дельты)
  else if (temporaryPress >= LONG_PRESS_DURATION - DELTA && temporaryPress <= LONG_PRESS_DURATION + DELTA) {
    /**/                   //bot.sendMessage("Длинное нажатие", _CHAT_MY_ID);
    return _STATUS_CLOSE;  // Длинное нажатие
  }
  // Если нажатие не соответствует ни одному из вариантов
  else {
    /**/       //bot.sendMessage("Ошибка!!! Нажатие не распознано !!!", _CHAT_MY_ID);
    return 4;  // Ошибка
  }
}
///////////////////
void waitForPress() {
  // Проверка, нажата ли кнопка при старте
  if (digitalRead(_PIN_SENSOR) == LOW) {
    delay(50);
    ////////////Serial.println("Кнопка нажата при старте. Ждём отпускания...");
    while (digitalRead(_PIN_SENSOR) == LOW) {
      // Ждём, пока кнопка не разомкнётся
      delay(50);
    }
    ////////////Serial.println("Кнопка отпущена. Начинаем работу.");
  }
}

void waitForFirstPress() {

  ////////////Serial.println(" Ждём первого нажатия...");
  while (digitalRead(_PIN_SENSOR) == HIGH) {
    // Ждём, пока кнопка не замкнётся
    delay(50);
  }
}

// открыть задвижку
void open_zaslonka() {
  ////////////Serial.println(" Кнопка 1 нажата, открыть задвижку");
  // открыть задвижку

  if (STATUS_ZADVIGKA != _STATUS_OPEN) {
    /**/ bot.sendMessage("Открыть задвижку", _CHAT_MY_ID);
    startMotor();
    waitForPress();
    while (STATUS_ZADVIGKA != _STATUS_OPEN) {

      STATUS_ZADVIGKA = waitForNextPressToStopMotor();
      if (STATUS_ZADVIGKA == 4) {
        // ошибка
        ////////////Serial.println(" Ошибка измерения - необходима реакция");
        /**/ bot.sendMessage("Открыть задвижку - Ошибка измерения времени нажатия - необходима реакция");  //, _CHAT_MY_ID);
        set_cycle_Time();
        //************///
        break;
      }
    }
    stopMotor();
  } else {
    ////////////Serial.println("Задвижка и так  открыта !!!");
  }
}
// закрыть задвижку
void close_zaslonka() {
  ////////////Serial.println(" Кнопка 2 нажата, закрыть задвижку");
  // закрыть задвижку

  if (STATUS_ZADVIGKA != _STATUS_CLOSE) {
    /**/ bot.sendMessage("Закрыть задвижку", _CHAT_MY_ID);
    startMotor();
    if (STATUS_ZADVIGKA == _STATUS_Z) {
      waitForPress();
      STATUS_ZADVIGKA == _STATUS_CLOSE;
    }
    while (STATUS_ZADVIGKA != _STATUS_CLOSE) {

      STATUS_ZADVIGKA = waitForNextPressToStopMotor();
      if (STATUS_ZADVIGKA == 4) {
        // ошибка
        ////////////Serial.println(" Ошибка измерения - необходима реакция");
        /**/ bot.sendMessage("Закрыть задвижку - Ошибка измерения времени нажатия - необходима реакция");  //, _CHAT_MY_ID);
        set_cycle_Time();
        //************///
        break;
      }
    }
    stopMotor();
  } else {
    ////////////Serial.println("Задвижка и так  закрыта !!!");
  }
}
// задвижка приоткрыта
void open_slightly_zaslonka() {

  ////////////Serial.println(" Кнопка 3 нажата, приоткрыть задвижку");
  // задвижка приоткрыта
  if (STATUS_ZADVIGKA != _STATUS_Z) {
    /**/ bot.sendMessage("Приоткрыть задвижку", _CHAT_MY_ID);
    startMotor();
    waitForPress();
    //STATUS_ZADVIGKA == _STATUS_Z
    if (STATUS_ZADVIGKA == _STATUS_CLOSE)  // открываем и далее ждём первого нажатия
    {
      ////////////Serial.println(" Закрыто - открываем ");
      while (STATUS_ZADVIGKA != _STATUS_OPEN) {

        STATUS_ZADVIGKA = waitForNextPressToStopMotor();
        if (STATUS_ZADVIGKA == 4) {
          // ошибка
          ////////////Serial.println(" Ошибка измерения - необходима реакция");
          /**/ bot.sendMessage("Приоткрыть задвижку - Ошибка измерения времени нажатия - необходима реакция");  //, _CHAT_MY_ID);
          set_cycle_Time();
          //************///
          break;
        }
      }
    }
    if (STATUS_ZADVIGKA == _STATUS_OPEN)  // ждём первого нажатия
    {
      ////////////Serial.println(" Открыто ");
      waitForFirstPress();
      STATUS_ZADVIGKA = _STATUS_Z;
      delay(_SHIFT_ZADVIGKA);
      stopMotor();
    }
    // Проверка на то что кнопка нажата
    delay(_SHIFT_ZADVIGKA * 3);
    ////////////Serial.println(" Проверка на то что кнопка нажата");
    if (digitalRead(_PIN_SENSOR) == HIGH) {
      STATUS_ZADVIGKA = 4;  // ошибка
      ////////////Serial.println(" Ошибка измерения - необходима реакция");
      /**/  //bot.sendMessage(" Ошибка измерения времени нажатия - необходима реакция");  //, _CHAT_MY_ID);
      set_cycle_Time();
      //************///
    }

  } else {
    ////////////Serial.println("Задвижка и так  приоткрыта !!!");
  }
}

//---Задвижка--------------------------------------------------------------