//code to receive thingspeak data using http requests
//createdon 1st december, 2016 by Chirag Parmar M at NEXT TECH LAB, SRM chennai

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

int led1 = 0; //mapping the physical pin gpio0 to the variable led1
int led2 = 2; //mapping the physical pin gpio2 to the variable led2

int field[4]; //hold the field values

ESP8266WiFiMulti WiFiMulti;

void setup() {
    //intialize the serial port at 115200 baud
    Serial.begin(115200);
    
    //initialize the gpio pins as output
    pinMode(led1,OUTPUT);
    pinMode(led2,OUTPUT);

    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }
    WiFiMulti.addAP("SSID", "PASSWORD");
}

void loop() {
    if((WiFiMulti.run() == WL_CONNECTED)) {
        HTTPClient http;
        Serial.print("Thingspeak fetching data");
        http.begin("http://api.thingspeak.com/channels/YOUR_CHANNEL_ID/feeds/last.json?api_key=YOUR_API_KEY");
        int httpCode = http.GET();
        
        //check for the code 200 (signifies good response)
        if(httpCode == HTTP_CODE_OK) {
           String payload = http.getString();
           
           //scan through the whole string for the field values
           for (int i = 0; i< payload.length() ; i++) {
             if(payload.substring(i,i+6) == "field1") {
               field[0] = int(payload[i+2]);
             }
             else if(payload.substring(i,i+6) == "field2") {
               field[1] = int(payload[i+2]);
             }
             else if(payload.substring(i,i+6) == "field3") {
               field[2] = int(payload[i+2]);
             }
             else if(payload.substring(i,i+6) == "field4") {
               field[3] = int(payload[i+2]);
             }
           }
        }
        else {
            Serial.printf("Failed to fetch data: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
        
        //control the gpio pins using the field values received
        if(field[0] == 0) {
          digitalWrite(led1,LOW);
        }
        else if (field[0] == 1) {
          digitalWrite(led1,HIGH);
        }
    }
    delay(10000);
}

