#include "application.h"
#include "libraries/spark-core-mdns/firmware/MDNS.h"

#define HTTP_PORT 80

SYSTEM_MODE(MANUAL);

MDNS mdns;

UDP udp;
unsigned int port = 5683;
String ON = String("ON");

void connect_wifi(){
    while(!WiFi.ready()){
        WiFi.connect();
        Serial.println("Connecting to Wifi...");
        delay(500);
    }
    Serial.println(WiFi.localIP());
}

void fix_connection(){
    Serial.println("Fixing WiFi");
    WiFi.disconnect();
    delay(1000);
    WiFi.off();
    delay(1000);
    WiFi.on();
    delay(1000);
    connect_wifi();
}


void setup(){
    Serial.begin(9600);
    Serial.println("beginning");
    RGB.control(true);
    RGB.brightness(100);
    RGB.color(10, 100, 255);
    pinMode(D2, OUTPUT);
    connect_wifi();
    udp.begin(port);
    Serial.println("Setting Hostname");
    while(!mdns.setHostname("buzzer")){
        Serial.println("Not Set");
        delay(250);
    }
    bool success = mdns.setService("tcp", "http", HTTP_PORT, "Buzzer");
    if (success) {
        Serial.println("service set");
        success = mdns.addTXTEntry("coreid", "1");
    }
    if (success) {
        Serial.println("ID Set");
        success = mdns.begin();
    }
    Serial.println("running");
}

void buzz(){
    RGB.brightness(250);
    Serial.println("Buzzing");
    digitalWrite(D2, HIGH);
    delay(1000);
    digitalWrite(D2, LOW);
    RGB.brightness(0);
}

void ack(){
    char ack[] = "1";
    int sent = udp.sendPacket(ack, 1, udp.remoteIP(), udp.remotePort());
    Serial.print("Sent: "); Serial.println(sent);
}

void check_packet(){
    int size = udp.parsePacket();
    if(size > 0){
        char packet[16];
        udp.read(packet, size);
        String message = String(packet);
        Serial.print("Packet: "); Serial.println(packet);
        udp.flush();
        if(message == ON){
            Serial.println("BUZZ");
            ack();
            buzz();
        }
    }
}

void loop(){
    if(WiFi.ready()){
        mdns.processQueries();
        check_packet();
    }else{
        connect_wifi();
    }
}
