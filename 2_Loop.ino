void loop() {

  processAPcheck();
//  if ( _LOOP_MILLIS <  (millis() - START_HOLD_MILLIS))
//  {
//    START_HOLD_MILLIS =  millis() ;
    bot.tick();   // тикаем в луп - опрос идёт раз в 3.5 с по встроенному таймеру
   
    //client.loop(); // опрос MQTT сервера
    //Serial.println("LOOP");
//  }

  if ( _SEND_MILLIS < (millis() - SEND_HOLD_MILLIS )) //  отправка раз в _SEND_MILLIS
  {
    SEND_HOLD_MILLIS =  millis();

    Tm_Hig (); //опрос датчика температуры  и присвоение переменным
    // //Serial.println("Tm_Hig");
    
    Send_Telegramm(); // проверка изменения температуры
   
    // //Serial.println("Logic");

    //---///////////////////////
  //  processURL(); // формируем отправку? отправляем и получаем ответ, разбираем ответ.

    //---//////////////////////////////////
    // Serial.println("processURL");
    // Logic(); // включение выключение реле GET не управляет
    // Serial.println("Logic");
   // ParcelAnalysis () ;
    // Serial.println("ParcelAnalysis");


//    // Отправка сообщений
//    sendMQTT_sensor ();
//    sendMQTT_Serial ();

//    WHILE20++;
//    if (WHILE20 > 20)
//    { // выполнить доп действия
//      WHILE20 = 0;
//      //---/// убрать принудительное включение, т.к. вентилятор установлен в подвале, где нетотрицательных температур
//      if ( RELAY1_STATUS )Relay_ON_OFF (0); 
//      if (!FLAG_RELIABILITY )
//      {
//        dht22.begin(); // если на момент проверки данные не достоверны то перезапуск датчика
//      }
//    }
//    Serial.print("STOP - WHILE20= ");
//    Serial.println (WHILE20);

  }
Button( );
  processReboot();
  server.handleClient(); // ОБРАБОТКА клиента
}
