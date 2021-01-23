#include "SPI.h"
#include "MFRC522.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

//RFID PIN CONFIGURATION
#define SS_PIN          5  // pin 5
#define RST_PIN         22  // pin 22

String strID; // for the output of ID CARD

MFRC522 rfid(SS_PIN, RST_PIN);   
MFRC522::MIFARE_Key key;
const int SOLENOID = 4; //pin 4
const int RELAY_CHARGER = 2; //pin 2
const int LED_STRIP = 34; //pin 34
const int led_buzzer = 14;
int lock_num;
int flag_status = 0;
int relayON = LOW; //relay nyala
int relayOFF = HIGH; //relay mati
int prev_state_battery,current_state_battery;
int EP =25; // pin for vibration sensor
int flag = 0;
long measurement; // values of vibration sensor

unsigned long currentMillis = millis();
long previousMillis = 0;





const char* ssid = "COLIVING 42 43";
const char* password = "magelang43";

String serverName = "http://103.155.26.13/iot_sepeda/4.service_json/service.php";
String Stats;
String payload;
String lock;
int char_to_num_stats;

const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;




Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, 12, NEO_GRB + NEO_KHZ800);

//uint32_t yellow = strip.Color(255, 0, 147);
// 65556
uint32_t red = strip.Color(255,0, 0);
uint32_t blue = strip.Color(0, 255, 0);
uint32_t empty = strip.Color(0, 0, 0);


void battery_a()
{
    strip.setPixelColor(9, blue);
    strip.setPixelColor(8, blue);
    
}

void battery_a_off()
{
      strip.setPixelColor(9, empty);
      strip.setPixelColor(8, empty);
}




void battery_b()
{
    strip.setPixelColor(9, blue);
    strip.setPixelColor(8, blue);
    strip.setPixelColor(7, blue);
    strip.setPixelColor(6, blue);
    //strip.setPixelColor(5, blue);
    
}


void battery_b_off()
{


      strip.setPixelColor(7, empty);
      strip.setPixelColor(6, empty);
  
}



void battery_c()
  {
    
    strip.setPixelColor(9, blue);
    strip.setPixelColor(8, blue);
    strip.setPixelColor(7, blue);
    strip.setPixelColor(6, blue);
    strip.setPixelColor(5, blue);
    strip.setPixelColor(4, blue);
    strip.setPixelColor(3, blue);
  }

void battery_c_off()
{
      strip.setPixelColor(5, empty);
      strip.setPixelColor(4, empty);
      strip.setPixelColor(3, empty);
  
}





void battery_d()
  {
    strip.setPixelColor(9, blue);
    strip.setPixelColor(8, blue);
    strip.setPixelColor(7, blue);
    strip.setPixelColor(6, blue);
    strip.setPixelColor(5, blue);
    strip.setPixelColor(4, blue);
    strip.setPixelColor(3, blue);
    strip.setPixelColor(2, blue);
    strip.setPixelColor(1, blue);
    strip.setPixelColor(0, blue);

/*
    strip.setPixelColor(9, blue);
    strip.setPixelColor(8, blue);
    strip.setPixelColor(7, blue);
    strip.setPixelColor(6, blue);
    strip.setPixelColor(5, blue);
    strip.setPixelColor(4, blue);
    strip.setPixelColor(3, blue);
    strip.setPixelColor(2, blue);
    strip.setPixelColor(1, blue);
    strip.setPixelColor(0, blue);
  */  
  }

void battery_d_off()
{

    strip.setPixelColor(2, empty);
    strip.setPixelColor(1, empty);
    strip.setPixelColor(0, empty);
  
}



void LED_warning()
{
    /*
    LED_warning can use for knowing there's a bike or vica versa
    */


    strip.setPixelColor(9, red);
    strip.setPixelColor(8, red);
    strip.setPixelColor(7, red);
    strip.setPixelColor(6, red);
    strip.setPixelColor(5, red);
    strip.setPixelColor(4, red);
    strip.setPixelColor(3, red);
    strip.setPixelColor(2, red);
    strip.setPixelColor(1, red);
    strip.setPixelColor(0, red);
  
}

void LED_stanby()
{
  
    strip.setPixelColor(9, red);
    strip.setPixelColor(8, red);
    strip.setPixelColor(7, red);
    strip.setPixelColor(6, red);
    strip.setPixelColor(5, red);
    strip.setPixelColor(4, red);
    strip.setPixelColor(3, red);
    strip.setPixelColor(2, red);
    strip.setPixelColor(1, red);
    strip.setPixelColor(0, red);
  
}

void LED_warning_off()
{
    strip.setPixelColor(9, empty);
    strip.setPixelColor(8, empty);
    strip.setPixelColor(7, empty);
    strip.setPixelColor(6, empty);
    strip.setPixelColor(5, empty);
    strip.setPixelColor(4, empty);
    strip.setPixelColor(3, empty);
    strip.setPixelColor(2, empty);
    strip.setPixelColor(1, empty);
    strip.setPixelColor(0, empty);

}


long TP_init(){
  delay(10);
  measurement=pulseIn (EP, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
  
}
void vibe_bak()
{
  long measurement =TP_init();
  delay(50);
  Serial.print("measurment = ");
  Serial.println(measurement);
  Serial.println(flag);
   if(measurement != 0)
   {
    flag = flag +1;
    if(flag == 5)
    {
       Serial.println("ada getaran");
       HTTP_POST_VIBES();
       delay(100);
       strip.clear();
       LED_warning();
       
       for(int i=0; i < 25; i++)
       {
          strip.show();
          digitalWrite(led_buzzer, HIGH);
          delay(100);
          strip.clear();
          digitalWrite(led_buzzer,LOW);
          LED_warning();

          strip.show();

          delay(100);
          strip.clear();

          LED_warning();
 
          strip.show();

          delay(100);
          strip.clear();

          LED_warning();
          strip.show();
          digitalWrite(led_buzzer,HIGH);
          delay(100);
          strip.clear();
          digitalWrite(led_buzzer,LOW);   
       }
        //strip.show();
       // strip.clear();
       
       flag = 0;
       //battery_Status();

       if(flag_status == 1)
        {
          battery_Status();
          strip.clear();
          strip.show();
        }
        else if(flag_status == 0)
        {
          strip.clear();
          LED_warning();
          strip.show();
        }
        
    }    
    }
   else
   {

    Serial.println("tidak ada getaran ");
    
    if(flag_status == 1)
    {
      strip.clear();
      battery_Status();
      strip.show();
    }
    else if(flag_status == 0)
    {
      strip.clear();
      LED_warning();
      strip.show();
      
    }
    //strip.show();
    
   }
  
}


//pin vibe EP
int countGetaran,countLamaGetaran, nilaiSensorGetar = 0;

void animasi_led_warning(){
  digitalWrite(led_buzzer, HIGH);
  strip.clear();
  LED_warning();
  strip.show();
  delay(500);

  digitalWrite(led_buzzer, LOW);
  strip.clear();
  LED_warning_off();
  strip.show();
  delay(500);

  digitalWrite(led_buzzer, HIGH);
  strip.clear();
  LED_warning();
  strip.show();
  delay(500);

  digitalWrite(led_buzzer, LOW);
  strip.clear();
  LED_warning_off();
  strip.show();
  delay(500);

  digitalWrite(led_buzzer, HIGH);
  strip.clear();
  LED_warning();
  strip.show();
  delay(500);

  digitalWrite(led_buzzer, LOW);
  strip.clear();
  LED_warning_off();
  strip.show();
  delay(500);
}

void vibe()
{
  nilaiSensorGetar = digitalRead(EP);
  if(countLamaGetaran>0){
    countLamaGetaran++;
  }
  if(nilaiSensorGetar == 1){
      Serial.print("Count Getaran = ");
      Serial.print(countGetaran); 
      Serial.print("  Count Lama Getaran = ");
      Serial.println(countLamaGetaran); 
      countGetaran++;
      countLamaGetaran++;
  }
  if(countLamaGetaran>5){
    if(countGetaran>1){
      Serial.println("Peringatan Getaran Lampu dan upload ke server "); 
      animasi_led_warning();     
    }
    countGetaran = 0;
    countLamaGetaran = 0;
  }
  
}

void HTTP_REQ()
{

      String req_url = "no_rfid=";
      req_url+= strID.c_str();
      req_url+= "&";
      req_url+= "service_id=1";
      req_url+= "&";
      req_url+= "ip=";
      req_url+=  WiFi.localIP().toString().c_str();
      req_url+= "&";
      req_url+= "mac=";
      req_url+=  WiFi.macAddress();
      
     if (WiFi.status()== WL_CONNECTED)
     {
          HTTPClient http;
          http.begin(serverName);

          http.addHeader("Content-Type", "application/x-www-form-urlencoded");
          String httpRequestData = req_url;                      
          int httpResponseCode = http.POST(httpRequestData);
          
          payload = http.getString();   //Get the request response payload
          Serial.println(payload);      //Print the response payload
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          Serial.print("Response: ");
          Serial.println(payload);
          DynamicJsonBuffer jsonBuffer(capacity);
          JsonObject& root = jsonBuffer.parseObject(payload);
          //Stats = root["status"];
          http.end();
          Stats = (const char*)root["status"];
          Stats = root["status"].as<const char*>();
          Stats = root["status"].as<String>();          
          Serial.println(Stats);  


          lock = (const char*)root["lock"];
          lock = root["lock"].as<const char*>();
          lock = root["lock"].as<String>(); 
          lock_num = lock.toInt();         
          Serial.println(lock); 


        if(strcmp(Stats.c_str(), "AKTIF") == 0  && lock_num == 1) 
        {
            
            
            digitalWrite(SOLENOID, relayON);
            delay(5000);
            digitalWrite(SOLENOID, relayOFF);
            HTTP_POST_POWER();
            battery_Status();
            Serial.print("OPEN");

        }

        else 
        {
            //digitalWrite(SOLENOID, relayOFF);
            Serial.print("CLOSED");

        }
     }
  }


void HTTP_POST_POWER()
{



   String req_url = "no_rfid=";
   req_url+= "13:92:70:0C";
   req_url+= "&";
   req_url+= "service_id=4";

   if (WiFi.status()== WL_CONNECTED)
   {
          HTTPClient http;
          http.begin(serverName);
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");
          String httpRequestData = req_url;                      
          int httpResponseCode = http.POST(httpRequestData);
          payload = http.getString();   //Get the request response payload
          Serial.println(payload);      //Print the response payload
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          Serial.print("Response: ");
          Serial.println(payload);
          DynamicJsonBuffer jsonBuffer(capacity);
          JsonObject& root = jsonBuffer.parseObject(payload);
          // Stats = root["status"];
          http.end();
          Stats = (const char*)root["kapasitas_baterai"];
          Stats = root["kapasitas_baterai"].as<const char*>();
          Stats = root["kapasitas_baterai"].as<String>();          
          //Serial.println(Stats); 
          char_to_num_stats = Stats.toInt();
          Serial.print("battery power ");
          Serial.println(char_to_num_stats);
          //battery_Status();


    
   }





  
}


void battery_Status()
{  
  if(char_to_num_stats >= 0 && char_to_num_stats <= 25 )
  {
    for(int i=0; i < 5; i++)
       {
          battery_a();
          strip.show();
          delay(500);
          
          battery_b();
          strip.show();
          delay(500);
          
          battery_c();
          strip.show();
          delay(500);
          
          battery_d();
          strip.show();
          delay(500);

          battery_b_off();
          strip.show();
          battery_c_off();
          strip.show();
          battery_d_off();
          strip.show();

          

          
          //strip.clear();
           
       }
    
  }

  else if(char_to_num_stats >= 26 && char_to_num_stats <= 50)
  {

    for(int i=0; i < 5; i++)
    {
          battery_b();
          strip.show();
          delay(500);
          
          battery_c();
          strip.show();
          delay(500);
          
          battery_d();
          strip.show();
          delay(500);

          
          battery_c_off();
          strip.show();
          battery_d_off();
          strip.show();
    }
    
    //battery_50();
  }

  else if(char_to_num_stats >= 51 && char_to_num_stats <= 75 )
  {
    //digitalWrite(RELAY_CHARGER,relayON);
    for(int i=0; i < 5; i++)
    {
          battery_c();
          strip.show();
          delay(500);
          
          battery_d();
          strip.show();
          delay(500);
          
          battery_d_off();
          strip.show();
            
    }
    //battery_75();
  }

  else if(char_to_num_stats >= 76 && char_to_num_stats <= 100)
  {
    battery_d();
    strip.show();
    digitalWrite(RELAY_CHARGER,relayOFF);
  }
}

void HTTP_POST_VIBES()
{
  //strip.clear();
  String req_url = "no_rfid=";
  req_url+= "13:92:70:0C";
  req_url+= "&";
  req_url+= "service_id=2";
  req_url+= "&";
  req_url+= "status_vibrate=";
  req_url+=  "1";

       if (WiFi.status()== WL_CONNECTED){
          HTTPClient http;
          http.begin(serverName);

          http.addHeader("Content-Type", "application/x-www-form-urlencoded");
          String httpRequestData = req_url;                      
          int httpResponseCode = http.POST(httpRequestData);
          
          payload = http.getString();   //Get the request response payload
          Serial.println(payload);      //Print the response payload
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          Serial.print("Response: ");
          Serial.println(payload);
     }

}
void Scanning_RFIID()
{    
    strID = "";
    for (byte i = 0; i < 4; i++)
    {
        strID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "") + String(rfid.uid.uidByte[i], HEX) + (i != 3 ? ":" : "");
    }
}

int status_sensor_rfid,count_no_card_detected = 0;//status ketika baca rfid pertama kali adalah 1 setelah itu di scan oleh app jika ok akan 0
String karakter_rfid_prev,karakter_rfid_current ;


void run_rfid()
{
    
      if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
      {
          digitalWrite(RELAY_CHARGER,relayOFF);          
          count_no_card_detected++;
          if(count_no_card_detected>1){
            Serial.println("no card detected");
            LED_stanby();
          }else{
            battery_d();
          }
          //LED_warning();
          //strip.show();
          flag_status = 0;
          //return;
          
      }
      else
      {
      //Serial.print(F("PICC type: "));
       MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      //Serial.println(rfid.PICC_GetTypeName(piccType));
  
      // Check is the PICC of Classic MIFARE type
      if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) 
      {
          Serial.println(("Your tag is not of type MIFARE Classic."));
          return;
      }
      flag_status = 1;
      status_sensor_rfid = 1;
      Scanning_RFIID();
      strID.toUpperCase();
      //rfid.PICC_HaltA();
      Serial.print("Your ID CARD is : ");
      Serial.println(strID);
      karakter_rfid_current = strID;
      count_no_card_detected = 0;
      //var status_valid_rfid = HTTP_REQ();
      HTTP_REQ();

      }
    
}






void setup()
{
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  SPI.begin();
  rfid.PCD_Init();
   //WIFI_STATUS();
  Serial.println("waiting for card to scan");
  
   
  pinMode(SOLENOID, OUTPUT);
  pinMode(RELAY_CHARGER, OUTPUT);
  pinMode(EP, INPUT);
  pinMode(led_buzzer,OUTPUT);
  digitalWrite(led_buzzer,LOW);
  digitalWrite(SOLENOID, relayOFF);
  digitalWrite(RELAY_CHARGER,relayOFF);
  strip.begin();
  strip.show();
  strip.clear();
  Serial.begin(9600);
}

void loop()
{
  strip.clear();
 
  run_rfid();  
  delay(500);
  vibe();
  delay(500);
  strip.show();
  delay(500);

}