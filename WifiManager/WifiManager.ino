//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>
#include <DHT.h>
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(1, 2);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>   
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4ecda56a656542f0825bb1704b711c4b";

#define DHTPIN D6          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  delay(2000);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
        lcd.print("Failed to read !");
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, h);
  Blynk.virtualWrite(V8, t);
}

void setup()
{
  EEPROM.begin(512);
  Serial.begin(115200);
  WiFiManager wifiManager;
  //wifiManager.resetSettings();    //Uncomment this to wipe WiFi settings from EEPROM on boot.  Comment out and recompile/upload after 1 boot cycle.
  wifiManager.autoConnect("NodeMCU");
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output, I like blinkies.
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str(), "cyrax.top", 8080);

  dht.begin(); //初始化DHT传感器
  timer.setInterval(1000L, sendSensor);
  lcd.clear(); //Use it to clear the LCD Widget
  lcd.begin(5,4);  // sda=0 | D3, scl=2 | D4
  // Turn on the blacklight and print a message.
  lcd.backlight();
}

void backpack() 
{
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(round(dht.readTemperature()));
  lcd.print("C/");
  lcd.print(round(dht.readTemperature(true)));
  lcd.print("F");
  lcd.setCursor(0,1);
  lcd.print("Humidity:");
  lcd.print(round(dht.readHumidity(true)));
  lcd.print("% ");
}

void loop()
{
  Blynk.run();  // Commented out for a short time.
  timer.run();
  backpack();
}

