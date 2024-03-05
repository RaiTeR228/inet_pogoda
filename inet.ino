

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> // http web access library
#include <ArduinoJson.h> // JSON decoding library
// Libraries for SSD1306 OLED display
#include <Wire.h> // include wire library (for I2C devices such as the SSD1306 display)
#include <Adafruit_GFX.h> // include Adafruit graphics library
#include <Adafruit_SSD1306.h> // include Adafruit SSD1306 OLED display driver
#include <iarduino_OLED_txt.h>
iarduino_OLED_txt myOLED(0x78); 
extern uint8_t MediumFontRus[];    // Подключаем шрифт SmallFontRus.

 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// set Wi-Fi SSID and password
const char *ssid = "MTS_Router_005317";
const char *password = "67903467";

// set location and API key
String Location = "Yasnyy, RU";
String API_Key = "926189bc421bb25be61af536cf415a0b";

void setup(void)
{


  myOLED.begin();                                    // Инициируем работу с дисплеем.
  myOLED.setFont(MediumFontRus);     
  myOLED.setCoding(TXT_UTF8);   
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  delay(1000);

  display.clearDisplay(); // clear the display buffer
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.display();

  WiFi.begin(ssid, password);

  Serial.print("Connecting.");

  while ( WiFi.status() != WL_CONNECTED )
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");

  delay(1000);
}

void loop()
{
  
  if (WiFi.status() == WL_CONNECTED) //Check WiFi connection status
  {
    WiFiClient client1;

    HTTPClient http; //Declare an object of class HTTPClient

    // specify request destination
    http.begin(client1, "http://api.openweathermap.org/data/2.5/weather?q=" + Location + "&APPID=" + API_Key); // !!

    int httpCode = http.GET(); // send the request

    if (httpCode > 0) // check the returning code
    {
      String payload = http.getString(); //Get the request response payload

      DynamicJsonBuffer jsonBuffer(512);

      // Parse JSON object
      JsonObject& root = jsonBuffer.parseObject(payload);
      if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        return;
      }

      float temp = (float)(root["main"]["temp"]) - 273.15; // get temperature in °C
      int humidity = root["main"]["humidity"]; // get humidity in %
     
      float wind_speed = root["wind"]["speed"]; // get wind speed in m/s
      

      
  

  // температура   
  myOLED.clrScr();
      myOLED.print("температура", OLED_C, 1);
      
       myOLED.print (  String(temp) + " C",  OLED_C, 6) ;
  delay(3000);
  //влажность
  myOLED.clrScr();
       myOLED.print("влажность", OLED_C, 1);
      
      myOLED.print( String(humidity) + " %", OLED_C, 6);
  delay(3000);
  //скорость вестра
  myOLED.clrScr();
      
      myOLED.print("скорость", 0, 1);
      myOLED.print("ветра: ", 0, 3);
      myOLED.print(  String(wind_speed) + " м/с", OLED_C, 6);
  delay(3000);
      display.display(); // display everything written to the display
    
    }

    http.end(); //Close connection
  }

  delay(3000); // wait 10 minute




}


// End of code.
