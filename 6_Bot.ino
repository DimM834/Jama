

// обработчик сообщений
void newMsg(FB_msg& msg) {

  // обновить, если прислал известный человек (админ)
  if (msg.OTA && msg.chatID == _CHAT_MY_ID) {
    bot.update();
    return;
  }
  // рестарт по запросу
 // if (msg.text == "restart") ESP.restart();
  // выводим имя юзера и текст сообщения
  Serial.print(msg.username);
  Serial.print(", ");
  Serial.print(msg.text);
  Serial.print(", ");
  Serial.println(msg.chatID);

  // выводим всю информацию о сообщении
  //  Serial.println(msg.toString());
  //  if (msg.text == "Close") bot.closeMenu();
  if (msg.text == "Овощная" or msg.text == "/send@Dim834_bot" or msg.text == "/send")
  {
    bot.sendMessage("Запрос ...", msg.chatID);
    Tm_Hig ();
    String sendBot = "Овощная: Температура = ";
    sendBot += TEMPERATURE[4] / 10.0;
    sendBot += " С⁰; Влажность = ";
    sendBot += HUMIDITY[0];
    sendBot += " %" ;
    bot.sendMessage(sendBot, msg.chatID);

  }
  else if (msg.text == "Дата" or msg.text == "/data" or msg.text == "/data@Dim834_bot")
  {
    FB_Time t = bot.getTime(3);
    bot.sendMessage(t.dateString(), msg.chatID);
  }
  else {
    bot.sendMessage("Не понял", msg.chatID);
   

    // выведем время и дату
//    FB_Time t = bot.getTime(3);
//    /*Serial.print(t.hour);
//      Serial.print(':');
//      Serial.print(t.minute);
//      Serial.print(':');
//      Serial.print(t.second);
//      Serial.print(' ');
//      Serial.print(t.day);
//      Serial.print(':');
//      Serial.print(t.month);
//      Serial.print(':');
//      Serial.println(t.year);*/
//
//    Serial.print(t.timeString());
//    Serial.print(' ');
//    Serial.println(t.dateString());
//    //Вывести дату и время
//    bot.sendMessage(t.timeString(), msg.chatID);
//    bot.sendMessage(t.dateString(), msg.chatID);
  }


  //  else if (msg.text == "/led_on") digitalWrite(LED_BUILTIN, LOW);
  //  else if (msg.text == "/led_off") digitalWrite(LED_BUILTIN, HIGH);
}

//---////////////////////////////



void Send_Telegramm()
{

  // Температура * на 10 !!!
  if ( abs(TEMPERATURE[3] - TEMPERATURE[1]) > 3)
  {
    TEMPERATURE[3] = TEMPERATURE[1]; // прошлое измерение
    Serial.print("Дельта = ");
    Serial.println(TEMPERATURE[3] / 10.0);

    int temperatura = int(round(TEMPERATURE[1] / 5.0) * 5);

    if (TEMPERATURE[4] != temperatura) // прошлая отправка
    {
      TEMPERATURE[4] = temperatura;

      String sendBot = "Овощная: Температура = ";
      sendBot += TEMPERATURE[4] / 10.0;
      sendBot += " С; Влажность = ";
      sendBot += HUMIDITY[0];
      sendBot += " %" ;

      bot.sendMessage(sendBot);

      Serial.print("Отправка = ");
      Serial.println(sendBot);
    }
  }
  // отправка сообщений

}
