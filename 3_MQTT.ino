////������ MQTT ��������
////---------------------------
//void reconnect()
//{
//  if ( WiFi.status() == WL_CONNECTED  )// если есть WiFi
//  {
//
//
//    // ���� ��� ���������� � mqtt 10 ������� �� 3 �
//    //while (!client.connected())
//    for (int i = 0 ; i < 10; i++)
//    {
//      client.connect("ESP02 Client", _MQTT_USER ,  _MQTT_PASS);
//      client.setCallback(callback);
//      //---/////////////////////////////////////////////////////////////////
//      client.subscribe("esp02/relay/#");      //
//      client.subscribe("esp02/status/#");     //
//      //---////////////////////////////////////////////////////////////////
//      if (client.connected())
//      {
//        Serial.println("connect MQTT");
//        break;
//
//      }
//      delay(1000);
//    }
//  }
//}
////----------------------------------------------
////---////////////////////////////////////
//void sendMQTT_sensor ()
//{
//  //---/////////////////////// Если пропал сигнал с mqtt
//  if (!client.connected()) {
//    Serial.println("reconnect");
//    reconnect();
//  }
//
//  char msgBuffer_1[8];           // ��������� ��� ������ ������� ������
//  char *_created_string;
//  // char value_St = '0';
//
//  Serial.println("");
//  Serial.println("sendMQTT");
//  _created_string = dtostrf((TEMPERATURE[0] / 10.0), 4, 1, msgBuffer_1);
//  client.publish("esp02/sensor/t1",  _created_string, true);
//
//  _created_string = dtostrf((TEMPERATURE[1] / 10.0), 4, 1, msgBuffer_1);
//  client.publish("esp02/sensor/t2",  _created_string, true);
//
//  String Humid = String(HUMIDITY[0]);
//  char charVar[sizeof(Humid)];
//  Humid.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/sensor/h1", charVar, true );
//
//  Humid = String(HUMIDITY[1]);
//  charVar[sizeof(Humid)];
//  Humid.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/sensor/h2", charVar, true );
//
//  //---///
//  // Признак достоверности данных. Если данные не вычисляются то признак укажет не это
//  if (FLAG_RELIABILITY )
//  {
//    client.publish("esp02/status/reliability", "1", true );
//  }
//  else
//  { client.publish("esp02/status/reliability", "0", true );
//  }
//
//  //---///
//
//  //////---///Заглушка только ручное управление
//  //  if (AUTO)
//  //  {
//  //    client.publish("esp02/status/auto", "1", true );
//  //  }
//  //  else
//  //  { client.publish("esp02/status/auto", "0", true );
//  //  }
//
//
//  switch (AUTO) {
//    case 0:
//      client.publish("esp02/status/auto", "0", true );
//      break;
//    case 1:
//      client.publish("esp02/status/auto", "1", true );
//      break;
//    case 2:
//      client.publish("esp02/status/auto", "2", true );
//      break;
//  }
//  //client.publish("esp02/status/auto", value_St, true );
//  ////char var = 'a';
//  //value_St = '0';
//
//  // Темп мин подвала - для включения нагрева
//  String stVal = String(int (TEMPERATURE_1_MIN / 10));
//  charVar[sizeof(stVal)];
//  stVal.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/status/t_min", charVar );
//
//  // Темп мах подвала - для включения вентилятора
//  stVal = String(int (TEMPERATURE_1_MAX / 10));
//  charVar[sizeof(stVal)];
//  stVal.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/status/t_max", charVar );
/////**********///
//// Темп мин 2 канал - для включения нагрева
//   stVal = String(int (TEMPERATURE_2_MIN / 10));
//  charVar[sizeof(stVal)];
//  stVal.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/status/t2_min", charVar );
//
//  // Темп мах 2 канал - для выключения нагрева
//  stVal = String(int (TEMPERATURE_2_MAX / 10));
//  charVar[sizeof(stVal)];
//  stVal.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/status/t2_max", charVar );
// ///********///
// // Темп мин 3 канал
//   stVal = String(int (TEMPERATURE_3_MIN / 10));
//  charVar[sizeof(stVal)];
//  stVal.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/status/t3_min", charVar );
//
//  // Темп мах 3 канал
//  stVal = String(int (TEMPERATURE_3_MAX / 10));
//  charVar[sizeof(stVal)];
//  stVal.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/status/t3_max", charVar ); 
//
//
//
//  
//}
/////////////////////////////////////////////////////////////
//// При изменении статуса реле отправка сообщения
//void sendMQTT_relay ()
//{
//  //---/////////////////////// Если пропал сигнал с mqtt
//  if (!client.connected()) {
//    Serial.println("reconnect");
//    reconnect();
//  }
//  if (RELAY1_STATUS)
//  {
//    client.publish("esp02/relay/rel1", "1", true );
//  }
//  else
//  { client.publish("esp02/relay/rel1", "0", true );
//  }
//
//  if (RELAY2_STATUS)
//  {
//    client.publish("esp02/relay/rel2", "1", true );
//  }
//  else
//  { client.publish("esp02/relay/rel2", "0", true );
//  }
//  if (RELAY3_STATUS)
//  {
//    client.publish("esp02/relay/rel3", "1", true );
//  }
//  else
//  { client.publish("esp02/relay/rel3", "0", true );
//  }
//}
////---////////////////////////////////////
//void sendMQTT_Serial ()
//{
//  //---/////////////////////// Если пропал сигнал с mqtt
//  if (!client.connected()) {
//    Serial.println("reconnect");
//    reconnect();
//  }
//  String stVal = String(BUF_SER.Work);
//  char charVar[sizeof(stVal)];
//  stVal.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/ard/value/work", charVar, true );
//
//
//  stVal = String(BUF_SER.Value_V);
//  charVar[sizeof(stVal)];
//  stVal.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/ard/value/top", charVar, true );
//
//  stVal = String(BUF_SER.Value_N);
//  charVar[sizeof(stVal)];
//  stVal.toCharArray(charVar, sizeof(charVar));
//  client.publish("esp02/ard/value/bottom", charVar, true );
//
//
//
//  ////////////////////
//
//
//
//  if (BUF_SER.Flag_Ur_V)
//  {
//    client.publish("esp02/ard/top_fl", "1", true );
//  }
//  else
//  { client.publish("esp02/ard/top_fl", "0", true );
//  }
//  ///
//  if (BUF_SER.Flag_Ur_N)
//  {
//    client.publish("esp02/ard/bottom_fl", "1", true );
//  }
//  else
//  { client.publish("esp02/ard/bottom_fl", "0", true );
//  }
//  ///
//  if (BUF_SER.Flag_Relay)
//  {
//    client.publish("esp02/ard/relay", "1", true );
//  }
//  else
//  { client.publish("esp02/ard/relay", "0", true );
//  }
//  ///
//
//}
