void Button( )
{
// проверка нажатия кнопки
if (digitalRead(_BUTTON_PIN)==HIGH ) //Когда не нажата кнопка
{
  if(BUTTON_STATUS == true)
    { // Отпускаем
      Serial.println("BUTTON_STATUS= false");
      BUTTON_STATUS= false;
      // выключить свет
      Relay_ON_OFF (0);
//      // сообщить в топик "esp01/sensor/b1"
//      client.publish("esp01/sensor/b1","HET", true);
//      client.publish("esp01/relay/rel1","0", true);
    }
  }
else  //Когда  нажата кнопка
{
   if(BUTTON_STATUS == false)
    { // Нажимаем
      BUTTON_STATUS= true;
      // включить свет
      Relay_ON_OFF (1);
      Serial.println("BUTTON_STATUS= true");
//      // сообщить в топик "esp01/sensor/b1"
//      client.publish("esp01/sensor/b1","DA", true);
//      client.publish("esp01/relay/rel1","1", true);
    }
  }
}

void Tm_Hig ()
{
// измеряем влажность 1 раз в минуту
// меняем последовательно порт опроса - выбираем поочереди датчики
float t;
float h;
float tempC ; 

DS18B20.requestTemperatures();
 //  датчик DHT11
for (int i=0; i<6; i++)
{
h = dht11.readHumidity();
t = dht11.readTemperature();
 if (!(isnan(h) || isnan(t) ) )
 {
    HUMIDITY[0] = int(h);
    TEMPERATURE[0] = int(t*10);
    Serial.print("t ="); 
    Serial.print(TEMPERATURE[0]);
    break; 
  } 
  delay (200);
  Serial.print("i ="); 
  Serial.println(i); 
}  
Serial.print("dht11: T ="); 
Serial.print(TEMPERATURE[0]); 
Serial.print("; H ="); 
Serial.println(HUMIDITY[0]); 
/////
 tempC = DS18B20.getTempCByIndex(0);
 if (tempC != 85.0 and tempC != (-127.0) )
 {
  TEMPERATURE[1]= int(tempC*10);
 }
 else TEMPERATURE[1]= 88;
///
Serial.print("DS18B20: ="); 
Serial.print(tempC); 
Serial.print("  T ="); 
Serial.println(TEMPERATURE[1]); 

//  ///!Заглушка !!! Вставить проверку на 1000 мс с момента запроса измерения!!!
//  delay (700);
//  tempC = DS18B20.getTempCByIndex(0);
//  if (tempC != 85.0 and tempC != (-127.0) ) TEMPERATURE[1] = int(tempC * 10);
//  ///
//  Serial.print("DS18B20: =");
//  Serial.print(tempC);
//  Serial.print("  T =");
//  Serial.println(TEMPERATURE[1]);

  /*
    TEMPERATURE[0] = 12;
    HUMIDITY[0] = 91;
    TEMPERATURE[1] = -50;
    HUMIDITY[1] = 90;
  */
}


//-----------------------------------------------------------
void Relay_ON_OFF (byte on_off )
{
  // ON_OFF = 1 - включить реле № 1.
  // ON_OFF = 0 -выключить реле № 1.
  


  switch (on_off )
  {

    case 0 :
      digitalWrite(_RELAY1, LOW);   //Realy Close
      RELAY1_STATUS = false ;
      break;
    case 1 :
      digitalWrite(_RELAY1, HIGH);   //Realy Power
      RELAY1_STATUS = true ;
      break;
  }

}
