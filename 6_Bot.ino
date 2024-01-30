
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
  if (msg.text == "/menu") {
    bot.showMenu("Овощная \t Состояние \t Обогрев", _CHAT_MY_ID);
    bot.closeMenu("-1001644002787");  // меню не корректно работает в канале
  }
  if (msg.text == "Овощная" or msg.text == "/send@Dim834_bot" or msg.text == "/send") {
    bot.sendMessage("Запрос ...", msg.chatID);
    Tm_Hig();
    String sendBot = "Овощная: Температура = ";
    sendBot += TEMPERATURE[4] / 10.0;
    sendBot += " С⁰; Влажность = ";
    sendBot += HUMIDITY[0];
    sendBot += " %";
    if (RELAY2_STATUS)  // реле включено
    {
      sendBot += " Обогреватель включен";
    } else  //реле выключено
    {
      sendBot += " Обогреватель выключен";
    }
    bot.sendMessage(sendBot, msg.chatID);

  } else if (msg.text == "Обогрев") {
    // FB_Time t = bot.getTime(3);
    // bot.sendMessage(t.dateString(), msg.chatID);
    // bot.sendMessage(t.timeString(), msg.chatID);
    // меню включить или выключить реле
    String menu1 = F("Включить \n Выключить");
    String call1 = F("Start, Stop");
    bot.inlineMenuCallback("-Обогрев-", menu1, call1);
  } else if (msg.data == "Start") {
    Relay_ON_OFF(3);
    sendTelegramm_relay(msg.chatID);
  } else if (msg.data == "Stop") {
    Relay_ON_OFF(4);
    sendTelegramm_relay(msg.chatID);
  } else if (msg.text == "Состояние" or msg.text == "/state" or msg.text == "/state@Dim834_bot") {
    sendTelegramm_relay(msg.chatID);
    String sendBot = "Сеть " + WiFi.SSID() + " уровень " + String(WiFi.RSSI()) + " мБд";
    bot.sendMessage(sendBot, msg.chatID);
  }

  else {
    bot.sendMessage("Команда не распознана !", msg.chatID);
  }
}

//---////////////////////////////



void Send_Telegramm() {

  // Температура * на 10 !!!
  if (abs(TEMPERATURE[3] - TEMPERATURE[1]) > 3) {
    TEMPERATURE[3] = TEMPERATURE[1];  // прошлое измерение
    Serial.print("Дельта = ");
    Serial.println(TEMPERATURE[3] / 10.0);

    int temperatura = int(round(TEMPERATURE[1] / 5.0) * 5);

    if (TEMPERATURE[4] != temperatura)  // прошлая отправка
    {
      TEMPERATURE[4] = temperatura;

      String sendBot = "Овощная: Температура = ";
      sendBot += TEMPERATURE[4] / 10.0;
      sendBot += " С; Влажность = ";
      sendBot += HUMIDITY[0];
      sendBot += " %";

      bot.sendMessage(sendBot);

      Serial.print("Отправка = ");
      Serial.println(sendBot);
    }
  }
  // отправка сообщений
  if (TEMPERATURE[1] < _MIN_TEMPER) {
    bot.sendMessage("Внимание!!! Очень низкая температура !!!");
  }
}

void sendTelegramm_relay(String& MyChadID) {
  String sendBot = "Свет в подполье ";
  if (RELAY1_STATUS) {
    sendBot += "включен";
  } else {
    sendBot += "выключен";
  }
  bot.sendMessage(sendBot, MyChadID);
  sendBot = "Обогреватель ";
  if (RELAY2_STATUS)  // реле включено
  {
    sendBot += "включен";
  } else  //реле выключено
  {
    sendBot += "выключен";
  }
  bot.sendMessage(sendBot, MyChadID);
  sendBot = "Контакт западни ";

  if (BUTTON_STATUS == true) {
    sendBot += "замкнут";
  } else {
    sendBot += "разомкнут";
  }
  bot.sendMessage(sendBot, MyChadID);
}
