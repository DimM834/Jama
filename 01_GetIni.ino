//------------------------------------------------------------------------- dht11
#define _TEMPERATURE_MIN  25
#define _TEMPERATURE_MAX  30
#define _TEMPERATURE_CLOSE  40
#define _TEMPERATURE_Z  50
#define _TEMPERATURE_OPEN  60


// все значения температуры умножаем на 10, чтобы работать с десятыми
int TEMPERATURE[4] = { 0, _TEMPERATURE_MAX, 0, 0 }; // чтобы при запуске не включался обогреватель
int HUMIDITY[1] = { 0 };
//byte _TEMPERATURE_MIN = 45; //25
//byte _TEMPERATURE_MAX = 60; //30

int TEMPERATURE_YANDEX = 888; 

boolean FLAG_EXTERNAL = false;  //внешнее управление

#define _BOT_TOKEN "1463566344:AAH8-z-k5hqmSfj0VleMuzAliI8WcEDtHqg"
#define _CHAT_ID "1331422127,-1001644002787"  // ,-1001701973117" // чат температура, чат подвал
#define _CHAT_MY_ID "1331422127"              //  только чат с ботом мой ID,
#define _MY_1MENU Овощная                     // пункт меню
//---////////////////////////////////

// Ключ API Я.Погода 
const char* access_key = "43671af9-6f2a-48f0-8709-66ae45be84df"; 
// Координаты Мельникова 20А
const float lat = 58.625620;
const float lon = 49.637350;
//---------------------------------------------------------------------------
#define _RELAY1 D0  // Лампа
#define _RELAY2 D5  // Реле для нагрева
//---Задвижка--------------------------------------------------------------
#define _PIN_SENSOR D1        // D1 //Кнопка подключена к пину 2 с подтяжкой к питанию
#define _PIN_RELAY_OUT D8         //D3 // пин светодиода.
#define _SHIFT_ZADVIGKA 100  // перебежка мотора при приоткрытии

unsigned long LAST_PRESS_TIME = 0;       // Время последнего нажатия
unsigned long LAST_RELEASE_TIME = 0;     // Время последнего отпускания
unsigned long SHORT_PRESS_DURATION = 0;  // Длительность первого нажатия
unsigned long LONG_PRESS_DURATION = 0;   // Длительность второго нажатия
unsigned long CYCLE_START_TIME = 0;      // Время начала полного цикла
unsigned long CYCLE_DURATION = 0;        // Длительность полного цикла
bool BUTTON_PRESSED = false;             // Состояние кнопки (нажата/отпущена)
int PRESS_COUNT = 0;                     // Счетчик нажатий
bool MOTOR_RUNNING = false;              // Состояние двигателя (включен/выключен)
const unsigned long DELTA = 70;          // Дельта для сравнения длительностей (в мс)
byte STATUS_ZADVIGKA = 0;
byte  FLAG_RUN_CYCLE =0;

#define _STATUS_OPEN 1   // задвижка открыта
#define _STATUS_CLOSE 2  // задвижка закрыта
#define _STATUS_Z 3      // задвижка приоткрыта
#define _STATUS_ERROR 4      // Статус задвижки не определён

//---Задвижка--------------------------------------------------------------

//#define _RELAY3  D8
//#define _HUMIDITY_MAX 60

//#define _DHT22_PIN  D8
#define _DHT11_PIN D4
#define _18B20_PIN D7
#define _BUTTON_PIN D6  // средний разьем -

boolean RELAY1_STATUS = false;
boolean RELAY2_STATUS = false;
boolean BUTTON_STATUS = false;

#define _LOOP_MILLIS 3000  //раз в 3 секунду опрос
unsigned long MQTT_HOLD_MILLIS = 0;

#define _SEND_MILLIS  180000  //3*60*1000 // время между отправками

unsigned long SEND_HOLD_MILLIS = _SEND_MILLIS;

#define _BUTTON_MILLIS 100  //раз в 1/10 секунду опрос кнопки
unsigned long BUTTON_HOLD_MILLIS = 0;

#if (_GET_WEB == 1) 
// отправка запроса прогноза погоды раз в _SEND_MILLIS_WEATHER
#define _SEND_MILLIS_WEATHER 3600000 // раз в час
unsigned long WEATHER_HOLD_MILLIS = 0;
#endif



//Датчик DHT11 подключен к цифровому пину _DHT11_PIN- Подвал
DHT dht11(_DHT11_PIN, DHT11);  // Объявление переменной класса dht11

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

void callback(char* topic, byte* payload, unsigned int length) {
  //---/// Отключил управление по MQTT

  char json[length + 1];
  strncpy(json, (char*)payload, length);
  json[length] = '\0';

  String message = String(json);
  ////////Serial.print("topic= ");
  ////////Serial.println(topic);

  ////////Serial.print("message= ");
  ////////Serial.println(message);
  //////////Serial.println( strcmp(topic, _NUMBER_SEN "/relay/rel1"));

  // if (strcmp(topic, _NUMBER_SEN "/relay/rel1") == 0)
  // { // Если пришло сообщение для relay1
  //   byte rel = (byte)message.toInt();
  //   Relay_ON_OFF (rel);
  // }


  // Тоже самое для relay2
  if (strcmp(topic, _NUMBER_SEN "/relay/rel2") == 0) {
    if (message == "0") {
      //---///////////////
      if (RELAY2_STATUS)  // реле включено
      {
        Relay_ON_OFF(4);  // выключаем
        ////////Serial.println("rel2 LOW");
        //---///////////////
      }
    } else {
      //---///////////////
      if (RELAY2_STATUS == false)  // реле выключено
      {
        Relay_ON_OFF(3);  // включаем
        ////////Serial.println("rel2 HIGH");
        //---///////////////
      }
    }
  }


  //  // проверка на состояние кнопки
  //  if (strcmp(topic, _NUMBER_SEN "/sensor/b1") == 0)
  //  {
  //    if (message == "HET" ) //and BUTTON_STATUS == true)
  //    {
  //      //---///////////////
  //      BUTTON_STATUS = false;
  //      ////////Serial.println("BUTTON_STATUS   false");
  //      //---///////////////
  //    }
  //    if (message == "DA" )//and BUTTON_STATUS == false) //латиницей h e t
  //    {
  //      //---///////////////
  //      BUTTON_STATUS = true;
  //      ////////Serial.println("BUTTON_STATUS   true");
  //      //---///////////////
  //    }
  //  }
}


//---/////////////////

WiFiClient wifiClient;
PubSubClient client;  // PORT будет определён в Setup
//PubSubClient client(mqtt_server, PORT, callback, wifiClient);

FastBot bot(_BOT_TOKEN);
//---///////////////////////////////////////////////////////////////////////////
// для я.погоды
#if (_GET_WEB == 1) 
WiFiClientSecure clientSecure;
#endif