//������ MQTT ��������
//---------------------------
void reconnect()
{
  if ( WiFi.status() == WL_CONNECTED  )// если нет  WiFi
  {
   
 
    // ���� ��� ���������� � mqtt 10 ������� �� 3 �
    //while (!client.connected())
    for (byte  i = 0 ; i < 5; i++)
    {
      client.connect(_NUMBER_SEN, lp.mqttlogin, lp.mqttpass);
      client.setCallback(callback);
      //---/////////////////////////////////////////////////////////////////
      //    client.subscribe(_NUMBER_SEN "/sensor/t1"); // подписываемся по топик с данными для температуры
      //    client.subscribe(_NUMBER_SEN "/sensor/h1"); // подписываемся по топик с данными для влажности
      //    client.subscribe(_NUMBER_SEN "/sensor/t2"); // подписываемся по топик с данными для температуры 2
      client.subscribe(_NUMBER_SEN "/relay/#"); // подписываемся по топик с данными для реле
      //   client.subscribe(_NUMBER_SEN "/sensor/b1"); // подписываемся по топик с данными для кнопки

      //---////////////////////////////////////////////////////////////////
      if (client.connected())
      {
        ////Serial.println("connect MQTT");
        break;

      }
      delay(500);
    }
  }
}
//----------------------------------------------
//---////////////////////////////////////
void sendMQTT_sensor ()
{
  //---/////////////////////// Если пропал сигнал с mqtt
  if (!client.connected()) {
    ////Serial.println("reconnect");
    reconnect();
  }

  ////Serial.println("");
  ////Serial.println("sendMQTT");

  char msgBuffer_1[8];           // убедитесь что строки размера хватит
  char *_created_string;
  // char value_St = '0';

  _created_string = dtostrf((TEMPERATURE[0]/ 10.0), 4, 1, msgBuffer_1);
  client.publish(_NUMBER_SEN "/sensor/t1",  _created_string);

  String Humid = String(HUMIDITY[0]);
  char charVar[sizeof(Humid)];
  Humid.toCharArray(charVar, sizeof(charVar));
  client.publish(_NUMBER_SEN "/sensor/h1", charVar );

  _created_string = dtostrf((TEMPERATURE[1]/ 10.0), 4, 1, msgBuffer_1);
  client.publish(_NUMBER_SEN "/sensor/t2", _created_string);


}
/////////////////////////////////////////////////////////////
//// При изменении статуса реле отправка сообщения
void sendMQTT_relay ()
{
  //---/////////////////////// Если пропал сигнал с mqtt
  if (!client.connected()) {
    ////Serial.println("reconnect");
    // reconnect();
    return; // выход из процедуры если нет связи с WiFi
    // иначе задержка на reconnect в 10 с.
  }

  if (RELAY1_STATUS)
  {
    client.publish(_NUMBER_SEN "/relay/rel1", "1", true );
  }
  else
  { client.publish(_NUMBER_SEN "/relay/rel1", "0", true );
  }
  //---//////////////////////////////////
  if (RELAY2_STATUS)
  {
    client.publish(_NUMBER_SEN "/relay/rel2", "1", true );
  }
  else
  { client.publish(_NUMBER_SEN "/relay/rel2", "0", true );
  }
  //-------/// сообщение о нажатии кнопки
  if (BUTTON_STATUS )
  {
    ////Serial.println("BUTTON_STATUS= true");
    // сообщить в топик _NUMBER_SEN "/sensor/b1"
    client.publish(_NUMBER_SEN "/sensor/b1", "DA", true);
  }
  else
  {
    ////Serial.println("BUTTON_STATUS= false");
    // сообщить в топик _NUMBER_SEN "/sensor/b1"
    client.publish(_NUMBER_SEN "/sensor/b1", "HET", true);
  }


}
////---////////////////////////////////////
