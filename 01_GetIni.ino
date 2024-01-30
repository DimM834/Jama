//------------------------------------------------------------------------- dht11
// все значения температуры умножаем на 10, чтобы работать с десятыми
int TEMPERATURE[4] = {0, 0, 0, 0};
int HUMIDITY[1] = {0};
byte TEMPERATURE_MIN = 25;
byte TEMPERATURE_MAX = 30;

#define _BOT_TOKEN "1463566344:AAH8-z-k5hqmSfj0VleMuzAliI8WcEDtHqg"
#define _CHAT_ID "1331422127,-1001644002787" // ,-1001701973117" // чат температура, чат подвал 
#define _CHAT_MY_ID "1331422127" //  только чат с ботом мой ID,
#define _MY_1MENU Овощная // пункт меню
//---////////////////////////////////

//---------------------------------------------------------------------------
#define _RELAY1  D0 // Лампа
#define _RELAY2  D5 // Реле для нагрева
//#define _RELAY3  D8
//#define _HUMIDITY_MAX 60

//#define _DHT22_PIN  D8
#define _DHT11_PIN  D4
#define _18B20_PIN  D7
#define _BUTTON_PIN  D6 // средний разьем - 

boolean RELAY1_STATUS = false  ;
boolean RELAY2_STATUS = false  ;
boolean BUTTON_STATUS = false  ;

#define _LOOP_MILLIS 1000 //раз в секунду опрос
unsigned long MQTT_HOLD_MILLIS = 0;

#define _SEND_MILLIS  180000   //3*60*1000 // время между отправками
unsigned long SEND_HOLD_MILLIS = _SEND_MILLIS;

#define _BUTTON_MILLIS 100 //раз в 1/10 секунду опрос кнопки
unsigned long BUTTON_HOLD_MILLIS = 0;

#define _MIN_TEMPER 25 // это 2,5 градуса



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

void callback(char* topic, byte* payload, unsigned int length)
{

  char json[length + 1];
  strncpy (json, (char*)payload, length);
  json[length] = '\0';

  String message = String(json);
  Serial.print("topic= ");
  Serial.println(topic);

  Serial.print("message= ");
  Serial.println(message);
  //Serial.println( strcmp(topic, _NUMBER_SEN "/relay/rel1"));

  if (strcmp(topic, _NUMBER_SEN "/relay/rel1") == 0)
  { // Если пришло сообщение для relay1
    byte rel = (byte)message.toInt();
    Relay_ON_OFF (rel);
  }
 
  
  //  // Тоже самое для relay2
  //  if (strcmp(topic, "esp01/relay/rel2") == 0)
  //  {
  //    if (message == "0")
  //    {
  //      //---///////////////
  //      Relay_ON_OFF (4) ;  // выключаем
  //      Serial.println("rel2 LOW");
  //      //---///////////////
  //
  //    }
  //    else
  //    {
  //      //---///////////////
  //      Relay_ON_OFF (3) ; // включаем
  //      Serial.println("rel2 HIGH");
  //      //---///////////////
  //    }
  //  }
  //  // проверка на состояние кнопки
  //  if (strcmp(topic, _NUMBER_SEN "/sensor/b1") == 0)
  //  {
  //    if (message == "HET" ) //and BUTTON_STATUS == true)
  //    {
  //      //---///////////////
  //      BUTTON_STATUS = false;
  //      Serial.println("BUTTON_STATUS   false");
  //      //---///////////////
  //    }
  //    if (message == "DA" )//and BUTTON_STATUS == false) //латиницей h e t
  //    {
  //      //---///////////////
  //      BUTTON_STATUS = true;
  //      Serial.println("BUTTON_STATUS   true");
  //      //---///////////////
  //    }
  //  }

}


//---/////////////////

WiFiClient wifiClient;
PubSubClient client; // PORT будет определён в Setup
//PubSubClient client(mqtt_server, PORT, callback, wifiClient);

FastBot bot(_BOT_TOKEN);
//---///////////////////////////////////////////////////////////////////////////
