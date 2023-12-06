//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 04_Read_Data_from_Google_Sheets
//----------------------------------------Including the libraries.
#include "WiFi.h"
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
//----------------------------------------

// Defining LED PINs on the ESP32 Board.
#define On_Board_LED_PIN  2

// Defines the PIN for the LEDs.
#define LED_1_PIN 12
#define LED_2_PIN 13

// Defines the location number for the custom degree character to be displayed on the LCD.
#define degree_Char_Num 0

//----------------------------------------SSID and PASSWORD of your WiFi network.
const char* ssid = "EEE_MEC";  //--> Your wifi name
const char* password = "MEC_EEE1234"; //--> Your wifi password
//----------------------------------------

// Google script Web_App_URL.
String Web_App_URL = "https://script.google.com/macros/s/AKfycbzOAzW91Qr7cGU9haaN9RNv05XE8woPmNIYVvfXE78FI-QvnlODYnPJeNdPFk9ynZjPAQ/exec";

String Status_Read_Sensor = "";
float Temp;
int Humd;
String Switch_1_State = "";
String Switch_2_State = "";

// Byte array variable for custom degree characters.
byte degree_Char[] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

// Initialize the LiquidCrystal_I2C lib as "lcd" and set the LCD I2C address to 0x27 and set the LCD configuration to 16 x 2.
// In general, the address of a 16x2 I2C LCD is "0x27".
// However, if the address "0x27" doesn't work, you can find out the address with "i2c_scanner". Look here : https://playground.arduino.cc/Main/I2cScanner/
LiquidCrystal_I2C lcd(0x27,16,2);

//________________________________________________________________________________getValue()
// String function to process the data (Split String).
// I got this from : https://www.electroniclinic.com/reyax-lora-based-multiple-sensors-monitoring-using-arduino/
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
//________________________________________________________________________________ 

//________________________________________________________________________________VOID SETUP()
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println();
  delay(1000);

  pinMode(On_Board_LED_PIN, OUTPUT);

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);

  digitalWrite(LED_1_PIN, LOW);
  digitalWrite(LED_2_PIN, LOW);

  // Initialize lcd.
  lcd.init();
  // Turn on the LED backlight on the LCD.
  lcd.backlight();
  // Custom characters for degree symbols.
  lcd.createChar(degree_Char_Num, degree_Char);
  // Clean the LCD display.
  lcd.clear();

  //----------------------------------------Set Wifi to STA mode
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WIFI mode : STA");
  
  // Serial.println();
  // Serial.println("-------------");
  // Serial.println("WIFI mode : STA");
  // Serial.println("-------------");

  WiFi.mode(WIFI_STA);

  delay(1000);
  //---------------------------------------- 

  //----------------------------------------Connect to Wi-Fi (STA).
  // Serial.println();
  // Serial.println("------------");
  // Serial.print("Connecting to ");
  // Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  //:::::::::::::::::: The process of connecting ESP32 with WiFi Hotspot / WiFi Router.
  // The process timeout of connecting ESP32 with WiFi Hotspot / WiFi Router is 20 seconds.
  // If within 20 seconds the ESP32 has not been successfully connected to WiFi, the ESP32 will restart.
  // I made this condition because on my ESP32, there are times when it seems like it can't connect to WiFi, so it needs to be restarted to be able to connect to WiFi.

  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Connecting to");
    lcd.setCursor(0,1);
    lcd.print("SSID");
    // Serial.print(".");
    digitalWrite(On_Board_LED_PIN, HIGH);
    delay(500);
    lcd.clear();
    digitalWrite(On_Board_LED_PIN, LOW);
    delay(500);
    
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Connecting");
      lcd.setCursor(0,1);
      lcd.print("failed !");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ESP32 restarted");
      delay(1000);
      ESP.restart();
    }
  }

  digitalWrite(On_Board_LED_PIN, LOW);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi connected");
  lcd.setCursor(0,1);
  lcd.print("successfully.");
  delay(1000);
  
  // Serial.println();
  // Serial.println("WiFi connected");
  // Serial.println("------------");
  //::::::::::::::::::
  //----------------------------------------

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Get data from");
  lcd.setCursor(0,1);
  lcd.print("Google Sheets");
  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please wait...");
  delay(1000);
}
//________________________________________________________________________________ 

//________________________________________________________________________________VOID LOOP()
void loop() {
  // put your main code here, to run repeatedly:

  //----------------------------------------Conditions that are executed when WiFi is connected.
  // This condition is the condition for reading or getting data from Google Sheets.
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(On_Board_LED_PIN, HIGH);

    // Create a URL for reading or getting data from Google Sheets.
    String Read_Data_URL = Web_App_URL + "?sts=read";

    Serial.println();
    Serial.println("-------------");
    Serial.println("Read data from Google Spreadsheet...");
    Serial.print("URL : ");
    Serial.println(Read_Data_URL);

    //::::::::::::::::::The process of reading or getting data from Google Sheets.
      // Initialize HTTPClient as "http".
      HTTPClient http;

      // HTTP GET Request.
      http.begin(Read_Data_URL.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

      // Gets the HTTP status code.
      int httpCode = http.GET(); 
      Serial.print("HTTP Status Code : ");
      Serial.println(httpCode);
  
      // Getting response from google sheet.
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload : " + payload);  
      }
  
      http.end();
    //::::::::::::::::::
    
    digitalWrite(On_Board_LED_PIN, LOW);
    Serial.println("-------------");

    //::::::::::::::::::Conditions that are executed if reading or getting data from Google Sheets is successful (HTTP Status Codes : 200).
    if (httpCode == 200) {
      // The process of separating data that is read or obtained from Google Sheets.
      Status_Read_Sensor = getValue(payload, ',', 0);
      Temp = getValue(payload, ',', 1).toFloat();
      Humd = getValue(payload, ',', 2).toInt();
      Switch_1_State = getValue(payload, ',', 3);
      Switch_2_State = getValue(payload, ',', 4);

      if (Status_Read_Sensor == "Success") {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Temp : ");
        lcd.print(Temp);
        lcd.print(" ");
        lcd.write(degree_Char_Num);
        lcd.print("C");
        lcd.setCursor(0,1);
        lcd.print("Humd : ");
        lcd.print(Humd);
        lcd.print(" %");
      }
      if (Status_Read_Sensor == "Failed") {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Temp : Err");
        lcd.setCursor(0,1);
        lcd.print("Humd : Err");
      }

      if (Switch_1_State == "On") digitalWrite(LED_1_PIN, HIGH);
      if (Switch_1_State == "Off") digitalWrite(LED_1_PIN, LOW);
      if (Switch_2_State == "On") digitalWrite(LED_2_PIN, HIGH);
      if (Switch_2_State == "Off") digitalWrite(LED_2_PIN, LOW);
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Failed to");
      lcd.setCursor(0,1);
      lcd.print("get data !");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please wait to");
      lcd.setCursor(0,1);
      lcd.print("get data again.");
    }
    //::::::::::::::::::
  }
  //----------------------------------------
  
  delay(10000);
}
//________________________________________________________________________________ 
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<