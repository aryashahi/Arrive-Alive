#include <LiquidCrystal.h>
#include <TinyGPS.h>
#include<SoftwareSerial.h>
SoftwareSerial SIM900(0,13);
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

const int relay_Pin = 2;
const int buzzer_Pin = 3;
const int green_led = 12;
const int red_led = 10;
const int alcohol_Sensor = 11;

TinyGPS gps;
//long lat,lon;
bool alcohol_Status = LOW;

void setup() {
  pinMode(relay_Pin, OUTPUT);
  pinMode(buzzer_Pin, OUTPUT);
  pinMode(red_led,OUTPUT);
  pinMode(green_led,OUTPUT);
  pinMode(alcohol_Sensor, INPUT);
  Serial.begin(9600);
  SIM900.begin(9600);    //Module baude rate, this is on max, it depends on the version
  lcd.begin(16, 2);
  lcd.print("ALCOHOL DETECTION");
  lcd.setCursor(5,1);
  lcd.print("SYSTEM");
  delay(600);
  lcd.clear();
  lcd.print("ARRIVE ALIVE");
  lcd.setCursor(5,1);
  lcd.print("ACTIVATED");
}
void loop() { 
     
      alcohol_Status = digitalRead(alcohol_Sensor);
      delay(100); 

      if(alcohol_Status == LOW)
      {
       digitalWrite(buzzer_Pin, LOW);  
       delay(200);
       lcd.clear();
       lcd.print("Alcohol not");
       lcd.setCursor(3,1);
       lcd.print("Detected");
       delay(500); 
       digitalWrite(green_led , HIGH);
       delay(100);
       digitalWrite(relay_Pin, HIGH);  
       delay(200);
       lcd.clear();
       lcd.print("Vehicle Started");
       delay(500);    
      }
     
      else if(alcohol_Status == HIGH)
      { 
          lcd.clear();
          lcd.print("Alcohol Detected");
          lcd.setCursor(3,2);
          digitalWrite(red_led,HIGH);
          digitalWrite(green_led,LOW);
          delay(100);
          digitalWrite(relay_Pin, LOW);  
          delay(200);             
          digitalWrite(buzzer_Pin, HIGH);  
          delay(200); 
          lcd.print("SENDING MSG..");
          delay(100);
          SIM900.print("AT + CMGF=1\r"); // set to sending msg mode
          delay(200);
          SIM900.println("AT + CMGS = \"+9190xxxxxxxx\"");// recipient's mobile number with country code
          delay(30);
          SIM900.println("AT + CMGS = \"+9163xxxxxxxx\"");// recipient's mobile number with country code
          delay(30);
          SIM900.println("AT + CMGS = \"+9184xxxxxxxx\"");// recipient's mobile number with country code
          delay(30);
          SIM900.println("Sending Message...");// recipient's mobile number with country code
          delay(30);
          SIM900.println("Alcohol Detected ");// The SMS text you want to send
          SIM900.println("please check the location of the vehicle");// The SMS text you want to send
          gps_read();
            
      }
}


void gps_read()
{
    bool newData = false;
          unsigned long chars;
          unsigned short sentences, failed;
          
          // For one second we parse GPS data and report some key values
          for (unsigned long start = millis(); millis() - start < 1000;)
          {
            while (Serial.available())
            {
              char c = Serial.read();
              if (gps.encode(c))
              newData = true;
              }
              }
              if(newData)      //If newData is true
              {
                float flat, flon;
                unsigned long age;
                bool newData = false;
                unsigned long chars;
                unsigned short sentences, failed;
                gps.f_get_position(&flat, &flon, &age);
                SIM900.print("LOCATION :: "); 
                SIM900.println(); 
                SIM900.print("Latitude = ");
                SIM900.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
                SIM900.println();
                SIM900.print("Longitude = ");
                SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
                delay(30);
                SIM900.println();
                SIM900.print("https://:maps.google.com/maps/?q="); 
                SIM900.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
                SIM900.print(",");
                SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
                SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26d
                delay(30);
                SIM900.println();
                delay(30);
                }
}
  
  
  
