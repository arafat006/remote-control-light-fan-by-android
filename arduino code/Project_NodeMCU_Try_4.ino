#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <EEPROM.h>
                       
#define FIREBASE_HOST "nodemcu-b9698.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "HsJC4UQl8MN63453eCV59Rg9FljBrwZxBQlDL6Vm" //Your Firebase Database Secret goes here

#define WIFI_SSID "Robust"                     //Friendz zone                          //WiFi SSID to which you want NodeMCU to connect
#define WIFI_PASSWORD "pinicnumber420"              //RandomNumber420                         //Password of your wifi network 

// define the number of bytes you want to access
#define EEPROM_SIZE 1

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Declare global variable to store value
int LED_1_PIN = LOW;
int LED_2_PIN = LOW;

// Variables will change:
//int ledState = LOW;         // the current state of the output pin

int GreenIndicator = D7;
int YellowIndicator = D6;
int RedIndicator = D5;

int LEDPIN_1 = D0;
int LEDPIN_2 = D1;

void setup() {

  Serial.begin(115200);                                   // Select the same baud rate if you want to see the datas on Serial Monitor
  //Serial.println("Serial communication started\n\n"); 

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  
  pinMode(LEDPIN_1,OUTPUT);
  pinMode(LEDPIN_2,OUTPUT);

  // read the last LED state from flash memory
  LED_1_PIN = EEPROM.read(1);
  LED_2_PIN = EEPROM.read(2);

  // set the LED to the last stored state
  digitalWrite(LEDPIN_1, LED_1_PIN);
  digitalWrite(LEDPIN_2, LED_2_PIN);

  pinMode(GreenIndicator,OUTPUT);
  pinMode(YellowIndicator,OUTPUT);
  pinMode(RedIndicator,OUTPUT);
  
  digitalWrite(RedIndicator,HIGH);
          
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  //Serial.print("Connecting to ");
  //Serial.print(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(100);
  }

  /*
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  */
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase

  Firebase.reconnectWiFi(true);
  delay(100);
  
  //After connected with WIFI...............
  digitalWrite(RedIndicator,LOW);
  digitalWrite(GreenIndicator,HIGH);

}



void loop() { 

    //Serial.println(EEPROM.read(2));
    
    if (Firebase.get(firebaseData,"/MSG/led1")) 
    {
      
      //Serial.println(Firebase.get(firebaseData,"/MSG/led1"));      
      //String LED = firebaseData.stringData();
      LedControl_1(firebaseData.stringData());
    }
    if (Firebase.get(firebaseData,"/MSG/led2")) 
    {
      //String LED = firebaseData.stringData();
      LedControl_2(firebaseData.stringData());
    }
    
    if(WiFi.status() != WL_CONNECTED){
      //Serial.println("Lost WiFi Connection! Restarting...");
      //ESP.restart();
      //digitalWrite(2,1);
      WiFi.disconnect();

      //After disconnected with WIFI...............
      digitalWrite(GreenIndicator,LOW);
      digitalWrite(RedIndicator,HIGH);
      
      //Serial.println("Booting Sketch...");
      WiFi.mode(WIFI_AP_STA);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      // Wait for connection
      while (WiFi.status() != WL_CONNECTED) {
        //Serial.print(".");
        delay(2000);
      }
      
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   
      Firebase.reconnectWiFi(true);
      //ESP.restart();
      
      //Serial.println("ok");
      //After connected with WIFI...............
      digitalWrite(RedIndicator,LOW);
      digitalWrite(GreenIndicator,HIGH);
      
    }

    //delay(1000);
}

void LedControl_1(String LED1){

  if (LED1 == "\\\"on\\\"")
  {
    digitalWrite(LEDPIN_1,HIGH); //led 1 is ON
    Serial.println("on");
  }
  else if(LED1 == "\\\"off\\\"")
  {
     digitalWrite(LEDPIN_1,LOW);//led 2 if OFF
     Serial.println("off");
  }

  if(LED1 == "\\\"on\\\"" && LED_1_PIN == LOW)
  {
    LED_1_PIN = HIGH;
    EEPROM.write(1, LED_1_PIN);
    EEPROM.commit();

    Firebase.setInt(firebaseData, "/MSG/led1_return", 1);
  }
  else if(LED1 == "\\\"off\\\"" && LED_1_PIN == HIGH)
  {
     LED_1_PIN = LOW;
     EEPROM.write(1, LED_1_PIN);
     EEPROM.commit();
     
     Firebase.setInt(firebaseData, "/MSG/led1_return", 0);
  }

}

void LedControl_2(String LED2){ 
    
  if (LED2 == "\\\"on\\\"")
  {
    digitalWrite(LEDPIN_2,HIGH); //led 1 is ON
  }
  else if(LED2 == "\\\"off\\\"")
  {
     digitalWrite(LEDPIN_2,LOW);//led 2 if OFF
  }


  if(LED2 == "\\\"on\\\"" && LED_2_PIN == LOW)
  {
    LED_2_PIN = HIGH;
    EEPROM.write(2, LED_2_PIN);
    EEPROM.commit();
    
    Firebase.setInt(firebaseData, "/MSG/led2_return", 1);
  }
  else if(LED2 == "\\\"off\\\"" && LED_2_PIN == HIGH)
  {
     LED_2_PIN = LOW;
     EEPROM.write(2, LED_2_PIN);
     EEPROM.commit();

     Firebase.setInt(firebaseData, "/MSG/led2_return", 0);
  }
}
