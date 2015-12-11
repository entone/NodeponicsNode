#include "application.h"
SYSTEM_MODE(MANUAL);

void setup() {
    Serial.begin(9600);
    Serial.println("beginning");
    RGB.control(true);
    RGB.brightness(250);
    RGB.color(10, 100, 255);
    pinMode(D2, OUTPUT);
    //connect_wifi();
    Serial.println("running");
}

unsigned int nextTime = 0;
int state = 0;
void loop(){
    //if(WiFi.ready()){
    if (nextTime > millis()) return;
    RGB.brightness(250*state);
    Serial.println("running");
    digitalWrite(D2, (state) ? HIGH : LOW);
    state = !state;
    nextTime = millis() + 2000;
    //}else{
    //    connect_wifi();
    //}
}
