//void processURL() {
//  // формируем отправку
//
//  Serial.print(" WiFi.status=");
//  Serial.print( WiFi.status());
//
//  if ( WiFi.status() == WL_CONNECTED  )// проверка на частоту отправки
//  {
//
//    String url = "http://mykirov.ru/cgi-bin/esp/esp1.pl?SAVE=1";
//    url += "&AUTO=";
//    url += AUTO;
//    url += "&T1=" ;
//    url += TEMPERATURE[0];
//    url += "&H1=" ;
//    url += HUMIDITY[0];
//    url += "&T2=" ;
//    url += TEMPERATURE[1];
//    url += "&H2=" ;
//    url += HUMIDITY[1];
//    url += "&K1=" ;
//    url += RELAY1_STATUS;
//    url += "&K2=" ;
//    url += RELAY2_STATUS;
//    //-------------------//
//    sendURL(url);
//  }
//}
//
//// ----------------------------------- sendURL -----------------------------------
//void sendURL(String url) {
//  
/////---  ///////////////////////////////  
/////--- НЕТ ВЫЗОВА  ////////////////////
/////---  ///////////////////////////////
//
//  HTTPClient http;
//  http.begin(url);
//  int httpCode = http.GET();
//  Serial.println(url);
//  Serial.print(" httpCode: ");
//  Serial.println(httpCode);
/////--- ОБРАТИТЬ ВНИМАНИ !!! /////////
/////--- не получаем управление с сайта
/////--- всё управление идёт с  MQTT    
/////---   ////////////////////////////
// /* if (httpCode > 0) {
//    if (httpCode == HTTP_CODE_OK) {
//
//      String payload = http.getString(); // возврат значения
//      Serial.println(payload);               // <<<<<<<<<<<  ToDo
//      //--------------------------------***************
//     
// // передача строки на разбор
//      int start_Poz ;
//      int end_Poz ;
//      String parsing_Str;
//      end_Poz = payload.indexOf(':');
//      start_Poz = end_Poz - 2;
//      parsing_Str = payload.substring(start_Poz , end_Poz);
//      start_Poz = end_Poz + 1 ;
//      if (parsing_Str == "OK")// выполняем разбор
//      {
//        //  AUTO
//
//        //start_Poz = payload.indexOf('=',start_Poz)+1;
//        end_Poz = payload.indexOf(':', start_Poz);
//        parsing_Str = payload.substring(start_Poz , end_Poz);
//        AUTO = parsing_Str.toInt() ;
//        start_Poz = end_Poz + 1 ;
//        Serial.print("AUTO=");
//        Serial.println(AUTO);
//        //
//        //  TEMPERATURE_MIN
//        //start_Poz = payload.indexOf('=',start_Poz)+1;
//        end_Poz = payload.indexOf(':', start_Poz);
//        parsing_Str = payload.substring(start_Poz , end_Poz);
//        TEMPERATURE_1_MIN = parsing_Str.toInt() * 10 ;
//        start_Poz = end_Poz + 1 ;
//        Serial.print("TEMPERATURE_MIN=");
//        Serial.println(TEMPERATURE_MIN);
//        //
//        //  TEMPERATURE_MAX
//        //start_Poz = payload.indexOf('=',start_Poz)+1;
//        end_Poz = payload.indexOf(':', start_Poz);
//        parsing_Str = payload.substring(start_Poz , end_Poz);
//        TEMPERATURE_1_MAX = parsing_Str.toInt() * 10 ;
//        start_Poz = end_Poz + 1 ;
//        Serial.print("TEMPERATURE_MAX=");
//        Serial.println(TEMPERATURE_MAX);
//        //
//        //  K1
//        //start_Poz = payload.indexOf('=',start_Poz)+1;
//        end_Poz = payload.indexOf(':', start_Poz);
//        parsing_Str = payload.substring(start_Poz , end_Poz);
//        K1 = parsing_Str.toInt() ;
//        start_Poz = end_Poz + 1 ;
//        Serial.print("K1=");
//        Serial.println(K1);
//        //
//        //  K2 - последнее значение
//        //start_Poz = payload.indexOf('=',start_Poz)+1;
//        parsing_Str = payload.substring(start_Poz);
//        K2 = parsing_Str.toInt() ;
//        Serial.print("K2=");
//        Serial.println(K2);
//      }
//      else
//      { // ошибка
//        Serial.println("ParsingError= ");
//        Serial.println(payload.substring(0, end_Poz));
//      }
//
//      //----******************------
//    }
//    
//  }
//  else {
//    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//  }
//*/
//  http.end(); //Close connection
//
//
//}
