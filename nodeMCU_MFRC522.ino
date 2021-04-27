#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#define I2CADDR 0x20  //i2c address
#define SS_PIN 10  //SD2 of nodeMCU
#define RST_PIN 9 //SD1 of nodeMCU
#define wakeuptime 30 // Wake from sleep, in seconds.
#define sPin_length 5
#define price_length 8

//client setting
const char* ssid  = "J330";
const char* password = "noob1234";
//const char* host = "192.168.43.126";  //localhost
const char* host = "aotestweb.000webhostapp.com"; //webserver

//keypad 4x3 setting
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'.','0','#'}
};

// Digitran keypad, bit numbers of PCF8574 i/o port
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6}; //connect to the column pinouts of the keypad
char pin[sPin_length];
char price[8];
byte data_count = 0;
char key;
char selection[2];
//keypad input
String buyPrice;
int sPin;

Keypad_I2C kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );   // Create keypad instance.

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
WiFiClient client;
String card;

const byte numChars = 1000;
char receivedChars[numChars];
boolean newData = false;

void setup() {
  Serial.begin(9600);
  Wire.begin( );        // Initiate one wire for i2c
  kpd.begin( );         // Initiate keypad
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println();
  Serial.println("NodeMCU start");
  delay(10);
  connectToWiFi();
}

void loop() {
  buyPrice = "";
  sPin = 0;
  connectToHost();
  key = kpd.getKey();
  card = readCard();
  delay(200);

  //check for card status
  if(verifyCard(card)){
    Serial.println();
    Serial.println("Card is valid.");
    
    //begin taking input from user
    //input price
    Serial.print("Enter price: ");
    buyPrice = getNumber();
    Serial.println();
    Serial.println(buyPrice);

    //input customer security pin
    Serial.print("Enter PIN: ");
    sPin = getSPin();
    Serial.println();
    Serial.println(sPin);
    String realPin = String(sPin);  //conversion to string

    //connect to host
    connectToHost();

    //perform payment process
    int pay = processPayment(card, buyPrice, realPin);
    if(pay == 1){
      Serial.println("Payment success");
    }else if(pay == 2){
      Serial.println("Insufficient balance");
    }else if(pay == 3){
      Serial.println("Invalid PIN!");
    }else{
      Serial.println("Server error!");
    }
  }else{
    Serial.println("Invalid Card");
    Serial.println("Auto registeration");
    connectToHost();
    delay(2000);
    
    //automatically register card uid to the system
    if(registerUID(card)){
      Serial.println("Card registered.");
    }else{
      Serial.println("Failed to register");
    }
  }

  //check connection status
//  if(WiFi.status() == WL_CONNECTION_LOST){
//    connectToWiFi();
//  }
  
  //closing connection
  client.stop();
}

//////////////////////////READ CARD OR TAG//////////////////////////
String readCard(){
  delay(2000);
  Serial.println();
  Serial.println("Waiting for card");
  
  while(!mfrc522.PICC_IsNewCardPresent()){
    yield();
  }
  while(!mfrc522.PICC_ReadCardSerial()){
    yield();
  }

  //Show UID on serial monitor
  Serial.println();
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  String temp = content.substring(1);
  temp.replace(" ", ""); //replace whitespace
  Serial.println("New UID= " + temp);

  return temp;
}


