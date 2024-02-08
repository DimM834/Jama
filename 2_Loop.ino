void loop() {

  //processAPcheck();
  bot.tick();   // тикаем в луп - опрос идёт раз в 3.5 с по встроенному таймеру

  //---//////////////опрос MQTT сервера
  if ( _LOOP_MILLIS <  (millis() - MQTT_HOLD_MILLIS))
  {
    MQTT_HOLD_MILLIS =  millis() ;
    client.loop(); // опрос MQTT сервера
  }
  //---////////////// отправка данных
  if ( _SEND_MILLIS < (millis() - SEND_HOLD_MILLIS )) //  отправка раз в _SEND_MILLIS
  {
    SEND_HOLD_MILLIS =  millis();
    Tm_Hig (); //опрос датчика температуры  и присвоение переменным
    Serial.println("Tm_Hig");
Logika();
    Send_Telegramm(); // проверка изменения температуры
    // Отправка сообщений
    sendMQTT_sensor ();
   
    //---//////////////проверка кнопки
  }
  if ( _BUTTON_MILLIS < (millis() - BUTTON_HOLD_MILLIS )) //  проверка  раз 100 мс
  {
    BUTTON_HOLD_MILLIS = millis();
    Button( );
  }
  
}
