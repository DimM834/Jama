void setup() {

  ////Serial.begin(115200);

  //---/////////////////////////////////////////////////////////////////
  pinMode(_RELAY1, OUTPUT);         //Порт на для управления Реле лампа
  pinMode(_RELAY2, OUTPUT);         //Порт на для управления Реле нагрев
  pinMode(_PIN_RELAY_OUT, OUTPUT);  // Внешнее Реле двигателя

  pinMode(_BUTTON_PIN, INPUT);  //Порт на для сухой контакт
  pinMode(_PIN_SENSOR, INPUT);  // Кнопка положения задвижки подключена к пину 2 с подтяжкой к питанию

  ////Serial.println("RELAY");
  // первоначально реле выключить
  digitalWrite(_RELAY1, LOW);         //Realy Close
  digitalWrite(_RELAY2, LOW);         //Realy Close
  digitalWrite(_PIN_RELAY_OUT, LOW);  // LOW  Выключаем реле (двигатель останавливается)

  //---/////////////////////////////////////////////////////////////////

  ////Serial.print("SDK version: ");
  ////Serial.println(ESP.getSdkVersion());
  ////Serial.print("Flash real size: ");
  ////Serial.print("Firmware compiled for flash: ");
  ////Serial.println(ESP.getFlashChipSize());


  //---/////////////////////////////////
  readEEPROM();
  //---/// Код для проверки необходимости настройки сети
  if (digitalRead(_BUTTON_PIN) == LOW)  //Для настройки сети LOW     кнопка нажата, заподня открыта
  {
    //---/// Добавить - задержка 5 с и далее закрыть заподню и только в этом случае проваливаться в настройки.
    for (byte i = 0; i < 10; i++) {
      delay(500);
      if (digitalRead(_BUTTON_PIN) == HIGH) loginPortal();
    }
  }
  //---///////////////////////////////
  ////Serial.println("Данные для входа: ");
  ////Serial.println();
  ////Serial.print("SSID: ");
  ////Serial.println(lp.ssid);
  ////Serial.print("Password: ");
  ////Serial.println(lp.pass);
  ////Serial.print("mqttserv: ");
  ////Serial.println(lp.mqttserv);
  ////Serial.print("mqttlogin: ");
  ////Serial.println(lp.mqttlogin);
  ////Serial.print("mqttpass: ");
  ////Serial.println(lp.mqttpass);
  ////Serial.print("mqttport: ");
  ////Serial.println(lp.mqttport);



  //---//////////// Настройка клиента MQTT
  client.setClient(wifiClient);
  client.setServer(lp.mqttserv, lp.mqttport);
  client.setCallback(callback);
  //---//////////////////////////////
  // ------------------- WiFi ---------------
  WiFi.disconnect(true);  //
  WiFi.persistent(false);
  WiFi.setPhyMode(WIFI_PHY_MODE_11G);  // ?????
  WiFi.begin(lp.ssid, lp.pass);
  delay(2000);
  // Инициализация
  unsigned long wifiConnectStart = millis();
  while (WiFi.status() != WL_CONNECTED) {
    // if (WiFi.status() == WL_CONNECT_FAILED) {
    //   ////////Serial.println("Ошибка соединения WIFI. Проверьте данные: ");
    //   ////////Serial.println();
    //   ////////Serial.print("SSID: ");
    //   ////////Serial.println(lp.ssid);
    //   ////////Serial.print("Password: ");
    //   ////////Serial.println(lp.pass);
    //   ////////Serial.println();
    // }
    ////Serial.print(" .");
    delay(500);

    if (millis() - wifiConnectStart > 5000) {
      ////Serial.println("Ошибка соединения WiFi");
      ////Serial.println("Попробуйте отправить обновленные параметры конфигурации.");
      break;
    }
  }

  ////Serial.println("CTAPT ");

  //------------------------------------------------------

  ////Serial.println("DHT");
  //  dht22.begin();
  dht11.begin();

  //
  ////Serial.println("DS18B20");
  DS18B20.begin();
  // по умолчанию разрешение датчика – 9-битное;
  // если у вас какие-то проблемы, его имеет смысл
  // поднять до 12 бит; если увеличить задержку,
  // это даст датчику больше времени на обработку
  // температурных данных
  // Устанавливаем разрешение датчика в 12 бит (мы могли бы установить другие значения, точность уменьшится, но скорость получения данных увеличится
  // dallasSensors.setResolution(sensorAddress, 12);
  //---///////////////////////////////////////////////////////////////////////////////
  //---///////////////////////////

  // установить ID чата, чтобы принимать сообщения только из него
  bot.setChatID(_CHAT_ID);  // передай "" (пустую строку) чтобы отключить проверку
  // подключаем функцию-обработчик
  bot.attach(newMsg);
  // показать юзер меню (\t - горизонтальное разделение кнопок, \n - вертикальное
  // bot.closeMenu( "-1001644002787"); // меню не корректно работает в канале
  bot.sendMessage("Привет, Старт!");
  // показать юзер меню (\t - горизонтальное разделение кнопок, \n - вертикальное
  bot.showMenu("Овощная \t Состояние \t Обогрев \n Настройки", _CHAT_MY_ID);
  //bot.showMenuText("-Меню-", "Овощная \t Дата" , _CHAT_MY_ID );
  bot.closeMenu("-1001644002787");  // меню не корректно работает в канале
  // отправить сообщение в указанный в setChatID

  //---Задвижка--------------------------------------------------------------

   set_cycle_Time();
  // ////Serial.println("Цикл  завершился");
 
 

  //---Задвижка--------------------------------------------------------------
}
