// запрос с Яндекс погоды
// 43671af9-6f2a-48f0-8709-66ae45be84df
//Запросы в сутки = 30
//Запросы в месяц =1000

// ----------------------------------- processURL -----------------------------------
#if (_GET_WEB == 1)
void processURL() {
  // формируем отправку

  ////Serial.print(" WiFi.status=");
  ////Serial.print(WiFi.status());

  if (WiFi.status() == WL_CONNECTED)  // проверка на частоту отправки
  {

    // Формируем URL с параметрами
    String url = "https://api.weather.yandex.ru/v2/forecast";
    url += "?lat=" + String(lat, 5);
    url += "&lon=" + String(lon, 5);


    ////Serial.print(" url =");
    ////Serial.println(url);

    sendURL(url);
  }
}
// ----------------------------------- sendURL -----------------------------------
void sendURL(String &url) {


  HTTPClient http;
  clientSecure.setInsecure();

  http.begin(clientSecure, url);
  http.addHeader("X-Yandex-Weather-Key", access_key);
  http.setTimeout(15000);
  int httpCode = http.GET();
  ////Serial.println(url);
  ////Serial.print(" httpCode: ");
  ////Serial.println(httpCode);


  ///---   ////////////////////////////

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      ////Serial.println("✓ Данные получены");
      // Читаем ответ
      String response = http.getString();


      ////Serial.print("Размер данных: ");
      ////Serial.print(response.length());
      ////Serial.println(" байт\n");
      /*bot.sendMessage("Я.Погода = " + response, _CHAT_MY_ID); */ 
      // Проверяем целостность
      if (response.length() > 100 && response.indexOf("{") != -1) {
        // Парсим текущую погоду
        parseCurrentWeather(response);

        // Парсим следующие 6 часов
        parseNext6Hours(response);

      } else {
        ////Serial.println("✗ Некорректный ответ");
        /***/ bot.sendMessage("Я.Погода Некорректный ответ", _CHAT_MY_ID);
      }

    } else {
      ////Serial.print("✗ Ошибка: ");
      ////Serial.println(http.errorToString(httpCode));
      /***/ bot.sendMessage("Я.Погода  Ошибка: = " + http.errorToString(httpCode), _CHAT_MY_ID);
    }
    http.end();

    ////Serial.println("\n" + String(40, '='));
    ////Serial.println("=== ЗАВЕРШЕНО ===");
  }
}

// Функция для определения текущего часа из JSON
int getCurrentHourFromJSON(String &json) {
  // Ищем поле now_dt
  int timePos = json.indexOf("\"now_dt\":\"");
  if (timePos != -1) {
    int timeEnd = json.indexOf("\"", timePos + 10);
    if (timeEnd != -1) {
      String timeStr = json.substring(timePos + 10, timeEnd);
      // Формат: "2026-01-01T18:21:59.881299Z"

      // Ищем 'T' и затем ':'
      int tPos = timeStr.indexOf('T');
      if (tPos != -1) {
        int colonPos = timeStr.indexOf(':', tPos);
        if (colonPos != -1) {
          // Извлекаем часы (2 символа перед двоеточием)
          String hourStr = timeStr.substring(colonPos - 2, colonPos);
          byte  hour = byte(hourStr.toInt());

          // Корректируем для московского времени (+3 часа)
          hour += 3;
          if (hour >= 24) hour -= 24;

          return hour;
        }
      }
    }
  }

  // Если не нашли, возвращаем 0
  return 0;
}
///////////////////////////////////////////////////
// Функция для парсинга текущей погоды
void parseCurrentWeather(String &json) {
  ////Serial.println("=== ТЕКУЩАЯ ПОГОДА ===");

  // Определяем текущий час
  WEATHER_YA.currentHour = getCurrentHourFromJSON(json);
  ////Serial.print("Текущий час (МСК): ");
  ////Serial.println(WEATHER_YA.currentHour);
   WEATHER_YA.success = true;
     // ТЕМПЕРАТУРА
  String tempStr = extractValue(json, "\"temp\":");
  if (tempStr.length() > 0) {
    WEATHER_YA.temperature = int(tempStr.toFloat() * 10);

    ////Serial.print("Температура: ");
    ////Serial.print(WEATHER_YA.temperature / 10.0);
    ////Serial.println("°C");
  } else {
    WEATHER_YA.success = false;
  }

  // ВЛАЖНОСТЬ
  String humidityStr = extractValue(json, "\"humidity\":");
  if (humidityStr.length() > 0) {
    WEATHER_YA.humidity = humidityStr.toInt();

    ////Serial.print("Влажность: ");
    ////Serial.print(WEATHER_YA.humidity);
    ////Serial.println("%");
  } else {
    WEATHER_YA.success = false;
  }


  if (WEATHER_YA.success) {
    bot.sendMessage("Я.Погода  на  = " + String(WEATHER_YA.currentHour) + " ч." , _CHAT_MY_ID);
    bot.sendMessage("Температура: " + String(WEATHER_YA.temperature / 10.0) + "°C,  Влажность: " + String(WEATHER_YA.humidity) + "%", _CHAT_MY_ID);

  } else {
    bot.sendMessage("Я.Погода  на  = " + String(WEATHER_YA.currentHour) + " ч. Получена с ошибкой ", _CHAT_ID);
  }

  ////Serial.println("====================\n");
}
//////////////////////////////////////////////////////////////////////
void parseNext6Hours(String &json) {
  ////Serial.println("=== ПРОГНОЗ НА СЛЕДУЮЩИЕ 6 ЧАСОВ ===");
  ////Serial.println("=== Заглушка !!! ===");

}
///////////////////////////////////////////////////////////////////////
// Вспомогательная функция для извлечения числовых значений
String extractValue(String &json, const String &key) {
  int startPos = json.indexOf(key);
  if (startPos == -1) return "";
  
  startPos += key.length();
  
  // Ищем конец значения
  int endPos = json.indexOf(",", startPos);
  if (endPos == -1) endPos = json.indexOf("}", startPos);
  if (endPos == -1) return "";
  
  return json.substring(startPos, endPos);
}

#endif