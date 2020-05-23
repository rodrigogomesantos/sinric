
//edit general configs

//#define MyApiKey "xxxxx" // TODO: Change to your sinric API Key. Your API Key is displayed on sinric.com dashboard
#define MyApiKey "xx-xxxxxxx-xxxxxxxx-xxxxxx" // TODO: Change to your sinric API Key. Your API Key is displayed on sinric.com dashboard
#define API_ENDPOINT "http://sinric.com"
#define HEARTBEAT_INTERVAL 300000 // 5 Minutes

//BordProfile
#define BOARD "ESP32"
//#define BOARD "ESP8266"

//Define your board profiles
#define BOARD_ESP32_REGUA

//if do you hame many diferent Boards
//-----------------------Profile 1 ---------------------------
#ifdef BOARD_ESP8266_LUZ

//ESP name
#define espName "ESP8266 Luzes"

//pins e Ids
#define RELE1_PIN 14 ///D5;
#define RELE1_ID "5xxxxxxxxxxxxxxxxxxxxxxx"
#define BUTTON1_PIN 14 ///D5;

#define RELE2_PIN 12 //D6;
#define RELE2_ID "5xxxxxxxxxxxxxxxxxxxxxxx"
#define BUTTON2_PIN 14 ///D5;

//#define RELE3_PIN = 13  //D7;
//#define RELE3_ID "5xxxxxxxxxxxxxxxxxxxxxxx" ;
//#define BUTTON3_PIN  14     ///D5;

// #define RELE4_PIN = 15  //D8;
//#define RELE4_ID "5xxxxxxxxxxxxxxxxxxxxxxx" ;
//#define BUTTON4_PIN  14     ///D5;

#endif


//-----------------------Profile 2 ---------------------------
#ifdef BOARD_ESP32_REGUA

#define espName "ESP32 Regua"

//pins e Ids
#define RELE1_PIN 15 ///D5;
#define RELE1_ID "5xxxxxxxxxxxxxxxxxxxxxxx"

#define RELE2_PIN 2 //D6;
#define RELE2_ID "5xxxxxxxxxxxxxxxxxxxxxxx"

#define RELE3_PIN 4 //D7;
#define RELE3_ID "5xxxxxxxxxxxxxxxxxxxxxxx"

//#define RELE4_PIN = 15  //D8;
//#define RELE4_ID "5xxxxxxxxxxxxxxxxxxxxxxx" 
#endif