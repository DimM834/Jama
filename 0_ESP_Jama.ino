#include <datatypes.h>
#include <FastBot.h>
#include <utils.h>

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
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <EEPROM.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

//---////////////////////////////////
//#include <PubSubClient.h>
//#include <ArduinoJson.h>
//---////////////////////////////////
// ----------------------------------- web
ESP8266WebServer server(80);
String html_header = "<!DOCTYPE HTML>\
 <html>\
 <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\
 <head>\
   <title>ESP8266 Settings</title>\
   <style>\
     body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
   </style>\
 </head>";


// ----------------------------------- common
#define apCheckTickTime 60*1000 
unsigned long nextapCheckTick;
bool rebootReq = false;


// ----------------------------------- wifi
IPAddress ipSTA;
IPAddress ipAP;
bool staInitOk;
String checkClientNetStr;
String ipAPstr;
String ipSTAstr;

// ----------------------------------- eeprom

#define EEPROM_START 0
boolean setEEPROM = false;

typedef struct {
  char STAssid[17];
  char STApass[17];
} CONF_eeprom_t __attribute__ ((packed));

CONF_eeprom_t CONF_eeprom
{
  "asd123",
  "espespesp",
};
//------------------------------------------------------------------------- dht11
// все значения температуры умножаем на 10, чтобы работать с десятыми
int TEMPERATURE[4] = {0,0,0,0};
int HUMIDITY[1] = {0};
#define _BOT_TOKEN "1463566344:AAH8-z-k5hqmSfj0VleMuzAliI8WcEDtHqg"
#define _CHAT_ID "1331422127,-709850083" // ,-1001701973117" // чат температура, чат подвал 
#define _CHAT_MY_ID "1331422127" //  только чат с ботом мой ID,

//---////////////////////////////////
////byte mqtt_server[] = { 192, 168, 0, 105 };  // ip адрес сервера mqtt
//const char* mqtt_server = "soldier.cloudmqtt.com";
//char buffer[10];
//#define  _MQTT_USER  "esp01_dom"
//#define  _MQTT_PASS  "mrY3d%aSq7k"
//int PORT = 10996;  
//---////////////////////////////////

//---------------------------------------------------------------------------
#define _RELAY1  D0 
//#define _RELAY2  D5 
//#define _RELAY3  D8 
//#define _HUMIDITY_MAX 60

//#define _DHT22_PIN  D8  
#define _DHT11_PIN  D4 
#define _18B20_PIN  D7
#define _BUTTON_PIN  D6
 
boolean RELAY1_STATUS = false  ;
//boolean RELAY2_STATUS = false  ;
boolean BUTTON_STATUS= false  ;
 //#define _LOOP_MILLIS 1000 //раз в секунду опрос
//unsigned long START_HOLD_MILLIS =0;
#define _SEND_MILLIS  180000   //3*60*1000 // время между отправками
unsigned long SEND_HOLD_MILLIS =0;

FastBot bot(_BOT_TOKEN);

//Датчик DHT11 подключен к цифровому пину _DHT11_PIN- Подвал 
DHT dht11(_DHT11_PIN, DHT11);                 // Объявление переменной класса dht11

// создаем экземпляр класса oneWire; с его помощью
// можно коммуницировать с любыми девайсами, работающими
// через интерфейс 1-Wire, а не только с температурными датчиками
// от компании Maxim/Dallas:
OneWire oneWire(_18B20_PIN);
// передаем объект oneWire объекту DS18B20:
DallasTemperature DS18B20(&oneWire);



//---------------------------------------------------------------------------------
// **********************************************************************
//---------------------------------------------------------------------------------
//---///////////////////////////////

//void callback(char* topic, byte* payload, unsigned int length)
//{
// 
//  char json[length + 1];
//  strncpy (json, (char*)payload, length);
//  json[length] = '\0';
//
//  String message = String(json);
//  Serial.print("topic= ");
//  Serial.println(topic);
//
//  Serial.print("message= ");
//  Serial.println(message);
//
//  if (strcmp(topic, "esp02/relay/rel1") == 0)
//  { // ���� ������ ��������� ��� relay1
//    K1 = message.toInt();
//    Serial.print("K1 = ");
//    Serial.println(K1 );
//    Logic(); // сразу отрабатывать переключения
//  }
//}

//---/////////////////

//WiFiClient wifiClient;
//PubSubClient client(mqtt_server, PORT, callback, wifiClient);
//---///////////////////////////////////////////////////////////////////////////

//---///////////////////////////////////////////////////////////////////////////



// ----------------------------------- processReboot -----------------------------------
void processReboot() {
  if (rebootReq == true)
  {
    delay(1000);
    ESP.reset();
  }
}


// ----------------------------------- checkAPinair -----------------------------------
boolean checkAPinair( String name) {
  name.toUpperCase();

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  //WiFi.mode(WIFI_STA);
  //WiFi.disconnect();
  //Serial.println(name);

  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
  {
    Serial.println("no networks found");
    return false;
  }
  else
  {
    String nnn;
    Serial.print(n); Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      nnn = WiFi.SSID(i);
      nnn.toUpperCase();
      Serial.print("   wifi "); Serial.print(i); Serial.println(nnn);

      if (nnn == name)
      {
        Serial.print(name);    Serial.print(" network found, RSSI:"); Serial.println(WiFi.RSSI(i));
        return true;
      }
    }
  }
  Serial.print(name);    Serial.println(" network NOT found");
  return false;

}


// ----------------------------------- processAPcheck -----------------------------------
void processAPcheck() {

  if (nextapCheckTick <= millis())
  {
    nextapCheckTick =  millis() + apCheckTickTime;

    if (WiFi.status() != WL_CONNECTED)
    {

      Serial.print("processAPcheck ssid : ");  Serial.println(CONF_eeprom.STAssid);

      if (checkAPinair(String(CONF_eeprom.STAssid)))
      {
        WiFi.mode(WIFI_STA); // WIFI_AP_STA
        STAinit();
        Serial.print(millis() / 1000); Serial.println(": AP found, AP_STA mode");
      }
      /*
            if ( staInitOk == false )
            {
              WiFi.mode(WIFI_AP);
              softAPinit();
              Serial.print(millis() / 1000); Serial.println(": soft WIFI_AP mode");
            }
      */
    }
  }
}

// ----------------------------------- STAinit -----------------------------------
void STAinit() {

  delay(1000);
  //WiFi.config(IPAddress(192, 168, 1, 73), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0), IPAddress(192, 168, 1, 1));
  //delay(10);
  WiFi.begin(CONF_eeprom.STAssid, CONF_eeprom.STApass);

  Serial.print("Wait for connection to WIFI_STA ");
  byte ind = 0;
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    if (ind % 2 == 0) Serial.printf ( "%d", 20 - ind / 2)
    ;
    else  Serial.print(".")
    ;
    ind++;
    if (ind > 10) break;
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    staInitOk = false;
    Serial.print("Bad Connected to "); Serial.println(CONF_eeprom.STAssid);
  }
  else
  {
    staInitOk = true;
    Serial.print("Success Connected to "); Serial.println(CONF_eeprom.STAssid);
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
  }

}

// ----------------------------------- softAPinit -----------------------------------
void softAPinit() {

  WiFi.softAP("ESPsoftAP_01", "espespesp"); // Параметры поднимаемой точки доступа
  //  WiFi.config(ip, gateway, subnet);
  Serial.print("AP SSID: ESPsoftAP_01");

  ipAP = WiFi.softAPIP();
  ipAPstr = Print_IP(ipAP);
  //  ipAPstr = String(ipAP[0]) + '.' + String(ipAP[1]) + '.' + String(ipAP[2]) + '.' + String(ipAP[3]);
  //  checkClientNetStr = String(ipAP[0]) + '.' + String(ipAP[1]) + '.' + String(ipAP[2]) + '.' ;

  Serial.print("AP IP address: "); Serial.println(ipAPstr);

}


// ----------------------------------- wifiEvent -----------------------------------
void WiFiEvent(WiFiEvent_t event) {

  //  Serial.printf("[WiFi-event] event: %d\n", event);

  switch (event) {
    case WIFI_EVENT_STAMODE_GOT_IP:
      Serial.println("WiFi = WIFI_EVENT_STAMODE_GOT_IP");
      if (staInitOk == false)
      {
        staInitOk = true;
      }

      ipSTA = WiFi.localIP();
      ipSTAstr = Print_IP(ipSTA);
      //ipSTAstr = String(ipSTA[0]) + '.' + String(ipSTA[1]) + '.' + String(ipSTA[2]) + '.' + String(ipSTA[3]);
      Serial.print(millis() / 1000); Serial.print(": STA IP address: "); Serial.println(ipSTAstr);
      break;

    case WIFI_EVENT_STAMODE_DISCONNECTED:
      //Serial.println("WiFi lost connection = WIFI_EVENT_STAMODE_DISCONNECTED");
      if (staInitOk == true)
      {
        staInitOk = false;
      }
      break;


      /*WIFI_EVENT_STAMODE_CONNECTED = 0,
        WIFI_EVENT_STAMODE_DISCONNECTED,1
        WIFI_EVENT_STAMODE_AUTHMODE_CHANGE,2
        WIFI_EVENT_STAMODE_GOT_IP,3
        WIFI_EVENT_STAMODE_DHCP_TIMEOUT,4
        WIFI_EVENT_SOFTAPMODE_STACONNECTED,5
        WIFI_EVENT_SOFTAPMODE_STADISCONNECTED,6
        WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED,7
        WIFI_EVENT_MAX*/
  }
}


// ----------------------------------- handleRoot -----------------------------------
void handleRoot() {


  String s = html_header;

  s += "Status - <a href='/a'>Actions</a> - <a href='/c'>Config</a>";
  s += "<br>";

  s += "Flash real size: ";
  s += ESP.getFlashChipRealSize();
  s += "<br>";
  s += "Firmware compiled for flash: ";
  s += ESP.getFlashChipSize();
  s += "<br>";

  if (WiFi.status() != WL_CONNECTED)
    s += "<br>WiFi client enabled and disconnected";
  else if (WiFi.status() == WL_CONNECTED)
  {
    s += "<br>WiFi client connected";
    if (ipSTAstr.length() > 0)
    {
      s += ", ip address: <a href=\"http://";
      s += ipSTAstr;
      s += "\">";
      s += ipSTAstr;
      s += "</a>";

      s += ", RSSI: ";
      s += String(WiFi.RSSI());
      s += " dBm";
    }

  }
  else
    s += "<br>wifi client disabled";

  /*
    s += ", url http://";
    s += URL_HOST;
    s += URL_STORE;

    s += "<br>OTAWEB update url: ";
    s += OTAWEB_URL;
    s += ", build version: ";
    s += OTAWEB_BUILD_VERSION;
  */
  s += "<br>";
  s += "<br>uptime: ";

  s += humanTimeMillis(millis());

  s += "<br>";
  s += "<br>EEPROM at boot: ";
  s += setEEPROM ? "ok" : "fail";

  s += "</html>";

  server.send(200, "text/html", s);
}
// ----------------------------------- handleActions -----------------------------------
void handleActions() {
  String s = html_header;
  s += "<a href='/'>Status</a> - Actions - <a href='/c'>Config</a>";
  s += "<br><br><a href='/a?reboot=true'>Reboot</a>";
  s += "<br><br><a href='/a?serialcheck=true'>Serialcheck</a>";
  s += "<br>";

  if (ESP.getFlashChipRealSize() > 900000)
  {
    s += "<br><font color=\"red\">Check your new firmware compile time size! must be 1mbyte+</font>";
  }
  else
  {
    s += "<br><font color=\"red\">Your flash ";
    s += String(ESP.getFlashChipRealSize());
    s += " bytes only, it's too small for OTA WEB</font>";
  }


  if (   server.hasArg("serialcheck"))
  {
    Serial.println("serial checkcheckcheckcheckcheckcheckcheckcheckcheckcheckcheck");
    s = "HTTP/1.1 307 Temporary Redirect";
    s += "\r\nLocation: /a";
    s += "\r\n\r\n";
    server.sendContent(s);
    return;
  }
  else if (   server.hasArg("reboot"))
  {
    s = "<head>";
    s += "<meta http-equiv=\"refresh\" content=\"20;url=/\">";
    s += "</head>";
    s += "REDIRECTING in 20 seconds";


    s += "<br><br>AP address: <a href=\"http://";
    s += ipAPstr;
    s += "\">";
    s += ipAPstr;
    s += "</a>";

    if (ipSTAstr.length() > 0)
    {
      s += ", STA address: <a href=\"http://";
      s += ipSTAstr;
      s += "\">";
      s += ipSTAstr;
      s += "</a>";
    }

    rebootReq = true;
  }

  s += "</html>";
  server.send(200, "text/html", s);
}
// ----------------------------------- handleConfig -----------------------------------
void handleConfig() {

  // debug
  Serial.print("Server Arguments: "); Serial.println(server.args());
  for (uint8_t i = 0; i < server.args(); i++) {
    Serial.print(server.argName(i));  Serial.print(" : "); Serial.println(server.arg(i));
  }


  String s ;
  /*
    if (CONF_eeprom.CONFIGauthEnabled == true)
    {
      if (!server.authenticate(CONF_eeprom.CONFIGuser, CONF_eeprom.CONFIGpass))
        return server.requestAuthentication();
    }
  */
  if ( server.hasArg("ssid") )
  {
    Serial.println("config change request");

    //    String lPASS;
    //    lPASS = server.arg("pass");

    server.arg("ssid").toCharArray(CONF_eeprom.STAssid, sizeof(CONF_eeprom.STAssid));
    server.arg("pass").toCharArray(CONF_eeprom.STApass, sizeof(CONF_eeprom.STApass));

    writeSettingsESP();


    if (server.arg("rebootRq") == "on")
    {

      s += "<head>";
      s += "<meta http-equiv=\"refresh\" content=\"20;url=/c\">";
      s += "</head>";
      s += "REDIRECTING in 20S";

      s += "<br><br>AP address: <a href=\"http://";
      s += ipAPstr;
      s += "\">";
      s += ipAPstr;
      s += "</a>";

      if (ipSTAstr.length() > 0)
      {
        s += ", STA address: <a href=\"http://";
        s += ipSTAstr;
        s += "\">";
        s += ipSTAstr;
        s += "</a>";
      }

      s += "</html>\r\n\r\n";


      rebootReq = true;
    }
    else
    {
      //STAinit();
      s = "HTTP/1.1 307 Temporary Redirect";
      s += "\r\nLocation: /c";
      s += "\r\n\r\n";

    }
    server.sendContent(s);
    //client.print(s);
    return;

  }


  s = html_header;

  s += "<a href='/'>Status</a> - <a href='/a'>Actions</a> - Config";


  s += "<br><form>";
  s += "<br>ssid: <input type=text name=ssid size=30 maxlength=16 value='";
  s += CONF_eeprom.STAssid;

  s += "' />";
  s += "<br>pass: <input type=text name=pass size=30 maxlength=16 value='";
  s += CONF_eeprom.STApass;
  s += "' /><br>";


  s += "<br>Reboot after storing <input type=\"checkbox\" name=\"rebootRq\"/>";
  s += "<br>";

  s += "<input type='submit' value='Save'></form>";
  s += "</html>\r\n\r\n";
  server.send(200, "text/html", s);
}

// ----------------------------------- readSettingsESP -----------------------------------
void readSettingsESP()
{
  int i;
  byte CONF_eeprom_tmp[sizeof(CONF_eeprom)];

  Serial.print("In readSettingsESP size: "); Serial.println(sizeof(CONF_eeprom));

  EEPROM.begin(sizeof(CONF_eeprom));

  for (i = EEPROM_START; i < EEPROM_START + sizeof(CONF_eeprom); i++)
  {
    Serial.print("."); Serial.println(i);
    CONF_eeprom_tmp[i] = EEPROM.read(i);
  }

  Serial.print("Finish read: "); Serial.println(sizeof(CONF_eeprom_tmp));
  setEEPROM = true;
  memcpy(&CONF_eeprom, CONF_eeprom_tmp,  sizeof(CONF_eeprom));

  Serial.print("Finish memcpy: "); Serial.println(sizeof(CONF_eeprom));

}

// ----------------------------------- writeSettingsESP -----------------------------------
void writeSettingsESP()
{
  int i;
  byte CONF_eeprom_tmp[sizeof(CONF_eeprom)];

  EEPROM.begin(sizeof(CONF_eeprom));

  memcpy(CONF_eeprom_tmp, &CONF_eeprom, sizeof(CONF_eeprom));

  for (i = EEPROM_START; i < EEPROM_START + sizeof(CONF_eeprom); i++)
  {
    EEPROM.write(i, CONF_eeprom_tmp[i]);
  }

  EEPROM.commit();
  Serial.println("EEPROM success commit");
}


// ----------------------------------- humanTimeMillis -----------------------------------
String humanTimeMillis(unsigned long milli) {

  String s;

  unsigned long secs = milli / 1000, mins = secs / 60;
  unsigned int hours = mins / 60, days = hours / 24;
  milli -= secs * 1000;
  secs -= mins * 60;
  mins -= hours * 60;
  hours -= days * 24;
  s += days != 0 ?  (String)days : "";
  s += days != 0 ?  "d " : "";
  s += hours != 0 ?  (String)hours : "";
  s += hours != 0 ?  ":" : "";
  s += mins > 9 ?  "" : "0";
  s += mins;
  s += ":";
  s += secs > 9 ?  "" : "0";
  s += secs;
  /*s += ".";
    if (milli < 10)    s += "00";  else if (milli < 100)    s += "0";
    s += milli;*/

  return s;
}

// ----------------------------------- Print_IP -----------------------------------
String Print_IP(IPAddress ip)
{
  return (String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]));
}
// -----------------------------------  -----------------------------------
// -----------------------------------  -----------------------------------
