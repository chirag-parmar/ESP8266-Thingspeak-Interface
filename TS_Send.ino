//code to send thingspeak data using http requests
//createdon 1st december, 2016 by Chirag Parmar M at NEXT TECH LAB, SRM chennai

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

int buttonpin = 2;

ESP8266WiFiMulti WiFiMulti;

void setup() {
    //intialize the serial port at 115200 baud
    Serial.begin(115200);

    pinMode(buttonpin,INPUT);

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
        String URL = "https://api.thingspeak.com/update.json?api_key=YOUR_API_KEY&field1=" + digitalRead(buttonpin);
        http.begin(URL);
        int httpCode = http.GET();
        
        //check for the code 200 (signifies good response)
        if(httpCode == HTTP_CODE_OK) {
           Serial.println("Button status updated successfully");
        }
        else {
            Serial.printf("Failed to send data: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
        
       
    }
    delay(10000);
}

