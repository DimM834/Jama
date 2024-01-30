/*функция WiFi.status().

  Serial.printf("Connection status: %d\n", WiFi.status());
  Эта функция не ограничивается булевыми значениями. В общей сложности она может вернуть пять разных значений, описывающих статус WiFi-соединения:

  0: WL_IDLE_STATUS – когда WiFi в процессе между сменой статусов
  1: WL_NO_SSID_AVAIL – когда заданный SSID находится вне зоны доступа
  3: WL_CONNECTED – после успешного подключения
  4: WL_CONNECT_FAILED – когда задан неправильный пароль
  6: WL_DISCONNECTED – когда ESP8266 не настроен на режим станции
*/
// ----------------------------------- libs -----------------------------------
#include <GyverPortal.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
//---////////////////////////////////
#include <PubSubClient.h>
#include <ArduinoJson.h>
//---////////////////////////////////

#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
//---////////////////////////////////
#include <datatypes.h>
#include <FastBot.h>
#include <utils.h>


// ----------------------------------- common
//#define apCheckTickTime 60*1000 
unsigned long nextapCheckTick;


#define _NUMBER_SEN "ESP01"  // номенр сенсора и заголовок топика MQTT
#define _VERSION_SEN "v2.1"  // номенр сенсора и заголовок топика MQTT

#define _EEPROM_START 0
struct LoginPass {
  char ssid[20];
  char pass[20];
  char mqttserv[20];
  char mqttlogin[20];
  char mqttpass[20];
  int mqttport;
};
LoginPass lp;

//byte mqtt_server[] = { 192, 168, 0, 129 };  // ip адрес сервера mqtt
// const char* mqtt_server = "m2.wqtt.ru";
// char buffer[10];
// #define  _MQTT_USER  "u_6UATRP"
// #define  _MQTT_PASS  "Ir4NIfsA"
// int PORT = 12051;

//---////////////////////////////////
// Портал !!!
//---//////////////////////////////////
void build() {  // Построитель формы ввода настроек
  GP.BUILD_BEGIN();
  GP.THEME(GP_LIGHT);

  GP.FORM_BEGIN("/login");
  GP.TITLE(_NUMBER_SEN);
  GP.TITLE("Версия " _VERSION_SEN);
  
  //---///
  GP.BLOCK_BEGIN(GP_THIN, "", "WiFi", GP_BLUE);
  GP.LABEL("SSID");
  GP.TEXT("lg", "Login", lp.ssid);
  GP.BREAK();
  GP.LABEL("Пароль");
  GP.PASS("ps", "Password", lp.pass);
  GP.BREAK();
  GP.BLOCK_END();
  //---///
  GP.BLOCK_BEGIN(GP_THIN, "", "MQTT", GP_BLUE);
  //GP.TITLE("MQTT");
  GP.LABEL("Адрес");
  GP.TEXT("msrv", "Server", lp.mqttserv);
  GP.BREAK();
  //
  GP.HR();
  //
  GP.LABEL("Логин");
  GP.TEXT("muser", "Login", lp.mqttlogin);
  GP.BREAK();
  GP.LABEL("Пароль");
  GP.PASS("mps", "Password", lp.mqttpass);
  GP.BREAK();
  GP.LABEL("Порт");
  GP.NUMBER("mport", "Port", lp.mqttport);
  GP.BLOCK_END();
  //---///
// GP.BLOCK_BEGIN(GP_THIN, "", "Частота опроса", GP_BLUE);
//   GP.LABEL("Минут");
//   GP.NUMBER("Sl_minutes", "Минут", lp.minutes_Sleep);
//   GP.BREAK();
//  GP.BLOCK_END();

  //---///
  GP.SUBMIT("Записать");
  GP.FORM_END();

  GP.BUILD_END();
}

//---///////////////////////////////////////////////////
void actionGP(GyverPortal& p) {
  if (p.form("/login")) {      // кнопка нажата
    p.copyStr("lg", lp.ssid);  // копируем себе
    p.copyStr("ps", lp.pass);
    p.copyStr("msrv", lp.mqttserv);
    p.copyStr("muser", lp.mqttlogin);
    p.copyStr("mps", lp.mqttpass);
    p.copyInt("mport", lp.mqttport);
//p.copyInt("Sl_minutes", lp.minutes_Sleep);
    //---/// записываем
    writeEEPROM();
    //---///
    WiFi.softAPdisconnect();  // отключаем AP
  }
}
//---///////////////////////////////////////////////////

void loginPortal() {
  ////Serial.println("Portal start");

  // запускаем точку доступа
  WiFi.mode(WIFI_AP);
  WiFi.softAP("WiFi Config");

  // запускаем портал
  GyverPortal ui;
  ui.attachBuild(build);
  ui.start();
  ui.attach(actionGP);

nextapCheckTick=millis();
  // работа портала
   while (ui.tick() or (millis() - nextapCheckTick > 600000)); // 10 минут 
  
}

