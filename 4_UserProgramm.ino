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
      ////Serial.println("BUTTON_STATUS= false");
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
      ////Serial.print("t =");
      ////Serial.print(TEMPERATURE[0]);
      break;
    }
    delay(200);
    ////Serial.print("i =");
    ////Serial.println(i);
  }
  ////Serial.print("dht11: T =");
  ////Serial.print(TEMPERATURE[0]);
  ////Serial.print("; H =");
  ////Serial.println(HUMIDITY[0]);
  /////
  tempC = DS18B20.getTempCByIndex(0);
  if (tempC != 85.0 and tempC != (-127.0)) {
    TEMPERATURE[1] = int(tempC * 10);
  } else TEMPERATURE[1] = 88;
  ///
  ////Serial.print("DS18B20: =");
  ////Serial.print(tempC);
  ////Serial.print("  T =");
  ////Serial.println(TEMPERATURE[1]);

  //  ///!Заглушка !!! Вставить проверку на 1000 мс с момента запроса измерения!!!
  //  delay (700);
  //  tempC = DS18B20.getTempCByIndex(0);
  //  if (tempC != 85.0 and tempC != (-127.0) ) TEMPERATURE[1] = int(tempC * 10);
  //  ///
  //  ////Serial.print("DS18B20: =");
  //  ////Serial.print(tempC);
  //  ////Serial.print("  T =");
  //  ////Serial.println(TEMPERATURE[1]);

  /*
    TEMPERATURE[0] = 12;
    HUMIDITY[0] = 91;
    TEMPERATURE[1] = -50;
    HUMIDITY[1] = 90;
  */
}
//---////////////////////////////////////////
void Logika() {
  if (FLAG_EXTERNAL) {
    return;  //внешнее управление
  }
  if (RELAY2_STATUS)  // реле включено
  {
    if (TEMPERATURE[1] >= TEMPERATURE_MAX) {
      Relay_ON_OFF(4);
      bot.sendMessage("Выключить обогреватель");
    }
  } else  //реле выключено
  {
    if (TEMPERATURE[1] <= TEMPERATURE_MIN) {
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
/////////////////////////////////////////
