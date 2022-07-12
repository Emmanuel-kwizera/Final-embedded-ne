#include<Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
WiFiClient wifiClient;
#define redLED 14
#define greenLED 16
#define buzzer 12


int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup(){
    pinMode(redLED, OUTPUT);
   pinMode(greenLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  WiFi.begin("RCA", "1@rca@2019!");
  //lcd.begin();
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
}

void loop(){  
  int rawData = analogRead(A0);
  float vcc = 5.0;
  float voltage = rawData * (vcc / 1024.0);
  float temperature = voltage * 100;
  Serial.print("Temperature: ");
  Serial.print(temperature+25);
  Serial.println(" *C");
//  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println(temperature+25);
  lcd.println(" ");
  lcd.println(char(223));
  lcd.println("C");

  
  char* host = "192.168.1.150"; //change your to your IP
  char* path = "/iot/";
  int port = 80;
  String tempData="";
  String device_name = "340722SPE0212022";
//  String temprature =  temperature;
  tempData = "device="+device_name+"&temprature="+(String)temperature;

    if(temperature<35){
    
      digitalWrite(redLED, 0);
      digitalWrite(greenLED, 1);
      digitalWrite(buzzer, 0); 
      lcd.println("Normal");
     
    }
    else{
    
      digitalWrite(redLED, 1);
      digitalWrite(greenLED, 0);
      digitalWrite(buzzer, 1); 
      sendMyData(port, host, path, tempData);
      //call the function for data uploading.
      lcd.println("Abnormal");
    
      delay(50);
    }  

//  String request = "POST /iot?device=340722SPE0212022&temperature="+(String)temperature+" HTTP/1.1";
//  wifiClient.connect(host, port);
//  wifiClient.println(request);
//  wifiClient.println("Host: "+host);
//  wifiClient.println("User-Agent: ESP8266/1.0");
//  wifiClient.println("C: ESP8266/1.0");
//  wifiClient.println();
//  Serial.println("Response: "+wifiClient.readStringUntil('\n'));
//   delay(500);
}

void sendMyData(const int httpPort, const char* host,const char* filepath , String data){
  
  wifiClient.connect(host, httpPort); 

  wifiClient.println("POST "+(String)filepath+" HTTP/1.1");
  wifiClient.println("Host: " + (String)host);
  wifiClient.println("User-Agent: ESP8266/1.0");
  wifiClient.println("Content-Type: application/x-www-form-urlencoded");
  wifiClient.println("Content-Length: " +(String)data.length());
  wifiClient.println();
  wifiClient.print(data);

  Serial.println("Response: " + wifiClient.readStringUntil('\n'));

}
