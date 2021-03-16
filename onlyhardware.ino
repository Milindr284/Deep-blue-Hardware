#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <Wire.h> 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <Arduino_JSON.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
int distanceCm , duration;
const int trigPin = 2; //D4 Or GPIO-2 of nodemcu
const int echoPin = 0; //D3 Or GPIO-0 of nodemcu
const char* ssid = "Ramesh";
const char* password = "rameshk204";
const char* serverName = "http://192.168.0.100:5000/temp";
const char* serverName1 = "http://192.168.0.100:5000/log";
String sensorReadings;

float sensorReadingsArr[3];

void setup_wifi() {
  Serial.begin(112500);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }
 
  Serial.println("Connected successfully.");
}

void setup() {
  //Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  mlx.begin();
  lcd.begin(16, 2);
  lcd.setCursor(4, 0);
  lcd.print("WELCOME");
  delay(3000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
lcd.clear();  
lcd.setCursor(0,0);
lcd.print("Place your hand");
lcd.setCursor(0,1);
lcd.print("close to sensor");
delay(200);


  digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);  
duration = pulseIn(echoPin,HIGH);
distanceCm = duration*0.034/2;


if (distanceCm <= 7){
  
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Reading");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Body Temperature");
  lcd.setCursor(0, 1);
  lcd.print(mlx.readObjectTempC());  
  lcd.print("*C");
  //delay(2000);
  
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("Body Temperature");
//  lcd.setCursor(0, 1);
//  lcd.print(mlx.readObjectTempC());  
//  lcd.print("*C");
  float Temp = mlx.readObjectTempC();
  delay(2000);
  char *hello_world = (char*)malloc(13 * sizeof(char));
  sprintf(hello_world, "{\"temp\":%f}", Temp);
//delay(1500);

 if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  } else {

    
    HTTPClient http;

    http.begin(serverName);

    http.addHeader("Content-Type", "application/json");
    Serial.print(hello_world);

    int httpResponseCode = http.POST(hello_world);
    
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.println(hello_world);
     
    //int httpCode = http.POST(String(Temp));
    //Serial.println(httpCode);
    http.end();
  }

 if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://192.168.0.100:5000/log");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing
      const size_t capacity = JSON_OBJECT_SIZE(3) + 70;
      DynamicJsonBuffer jsonBuffer(capacity);

      //DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters
      const char* displayName = root["displayName"]; // "milind"
      const char* name = root["name"]; // "Leanne Graham"
      const char* username = root["username"]; // "Bret"
      const char* email = root["email"]; // "Sincere@april.biz"
      // Output to serial monitor
      Serial.print("Name:");
      Serial.println(displayName);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome");
      lcd.setCursor(0, 1);
      lcd.print(displayName);
      delay(2000);
    }      
    http.end();   //Close connection
   }
  }
    
//  sensorReadings = httpGETRequest(serverName1);
//  Serial.println(sensorReadings);
//  JSONVar myObject = JSON.parse(sensorReadings);
//  
//      // JSON.typeof(jsonVar) can be used to get the type of the var
//  if (JSON.typeof(myObject) == "undefined") {
//    Serial.println("Parsing input failed!");
//    return;
//      }
//    
//  Serial.print("JSON object = ");
//  Serial.println(myObject);
//
//  // myObject.keys() can be used to get an array of all the keys in the object
//      JSONVar keys = myObject.keys();
//    
//      for (int i = 0; i < keys.length(); i++) {
//        JSONVar value = myObject[keys[i]];
//        Serial.print(keys[i]);
//        Serial.print(" = ");
//        Serial.println(value);
//        sensorReadingsArr[i] = double(value);
//      }
//      Serial.print("1 = ");
//      Serial.println(sensorReadingsArr[0]);
////      Serial.print("2 = ");
////      Serial.println(sensorReadingsArr[1]);
////      Serial.print("3 = ");
////      Serial.println(sensorReadingsArr[2]);
//      
//      lcd.clear();
//      lcd.setCursor(0, 0);
//      lcd.print(sensorReadingsArr[0]);
//      //lcd.print("   Welcome");
//      lcd.setCursor(0, 1);
//      lcd.print(sensorReadingsArr[1]);
//      delay(2000);

  
//    digitalWrite(trigPin,LOW);
//    delayMicroseconds(2);
//    digitalWrite(trigPin,HIGH);
//    delayMicroseconds(10);
//    digitalWrite(trigPin,LOW);
//    delayMicroseconds(2);  
//duration = pulseIn(echoPin,HIGH);
//distanceCm = duration*0.034/2;

//lcd.clear();
//lcd.setCursor(0,0);
//lcd.print("Place your hand");
//lcd.setCursor(0,1);
//lcd.print("close to sensor");
//delay(200);
}

//String httpGETRequest(const char* serverName1) {
//  HTTPClient http;
//    
//  // Your IP address with path or Domain name with URL path 
//  http.begin(serverName1);
//  
//  // Send HTTP POST request
//  int httpResponseCode = http.GET();
//  
//  String payload = "{}"; 
//  
//  if (httpResponseCode>0) {
//    Serial.print("HTTP Response code: ");
//    Serial.println(httpResponseCode);
//    payload = http.getString();
//  }
//  else {
//    Serial.print("Error code: ");
//    Serial.println(httpResponseCode);
//  }
//  // Free resources
//  http.end();
//
//  return payload;
//}
