void setup() {

  Serial.begin(115200);

  //---/////////////////////////////////////////////////////////////////
  pinMode(_RELAY1, OUTPUT);  //Порт на для управления Реле
  //  pinMode(_RELAY2, OUTPUT);  //Порт на для управления Реле

  pinMode(_BUTTON_PIN, INPUT);  //Порт на для сухой контакт
  Serial.println("RELAY");
  // первоначально реле выключить
  digitalWrite(_RELAY1, LOW);   //Realy Close
  //  digitalWrite(_RELAY2, LOW);   //Realy Close

  //---/////////////////////////////////////////////////////////////////

  Serial.println("1"); delay(2000);

  Serial.println("2"); delay(2000);

  Serial.println("3"); delay(2000);

  Serial.println("4"); delay(2000);

  Serial.println("5"); delay(2000);



  Serial.print("SDK version: "); Serial.println(ESP.getSdkVersion());
  Serial.print("Flash real size: "); Serial.println(ESP.getFlashChipRealSize());
  Serial.print("Firmware compiled for flash: "); Serial.println(ESP.getFlashChipSize());
  delay(10);

  readSettingsESP();


  // ------------------- WiFi ---------------
  WiFi.disconnect(true);  // not autoupdate flash ssid+passw
  WiFi.persistent(false);
  WiFi.setPhyMode(WIFI_PHY_MODE_11G); // ?????
  delay(500);

  if (checkAPinair(String(CONF_eeprom.STAssid)))
  {
    WiFi.mode(WIFI_STA); // WIFI_AP_STA
    STAinit();
  }
  // create soft AP
  if ( staInitOk == false )
  {
    WiFi.mode(WIFI_AP);
    softAPinit();
  }

  // ----------- server ---------------------
  server.on("/", handleRoot);
  server.on("/a", handleActions);
  server.on("/c", handleConfig);
  //server.on("/reboot", handleReboot);
  //server.on("/serialcheck", handleSerialCheck);
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found\n\n");
  });

  server.begin();

  Serial.println("CTAPT ");
  SEND_HOLD_MILLIS = _SEND_MILLIS;
  //------------------------------------------------------


  Serial.println("DHT");
  //  dht22.begin();
  dht11.begin();

  //
  Serial.println("DS18B20");
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
  bot.setChatID(_CHAT_ID); // передай "" (пустую строку) чтобы отключить проверку
  // подключаем функцию-обработчик
  bot.attach(newMsg);
  // показать юзер меню (\t - горизонтальное разделение кнопок, \n - вертикальное
  // bot.showMenu("Овощная \t Дата");//,_CHAT_MY_ID );
  bot.showMenuText("-Меню-", "Овощьная \t Дата ", _CHAT_MY_ID );
  bot.closeMenu( "-709850083"); // меню не корректно работает в канале
  // отправить сообщение в указанный в setChatID
  bot.sendMessage("Привет, Старт!");
  //---///////////////////////
}
