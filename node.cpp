#include "application.h"
#include "libraries/SparkJson/ArduinoJson.h"
#include "libraries/AES/AES.h"
#include "libraries/AnalogSensor/AnalogSensor.h"
#include "libraries/DHT/DHT.h"
#include "libraries/CO2Monitor/CO2Monitor.h"
#include "libraries/HttpClient/HttpClient.h"
#include "libraries/DustSensor/DustSensor.h"
#include "key.h"
SYSTEM_MODE(MANUAL);

UDP udp;
unsigned int port = 5683;
IPAddress multicast(239,255,41,11);

String myIDStr = Particle.deviceID();
char id[36];

bool ackd = false;

StaticJsonBuffer<256> jsonBuffer;

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
    udp.joinMulticast(multicast);
    myIDStr.toCharArray(id, 36);
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

unsigned int next = 0;

void send_stats(){
    if(millis() < next) return;
    next = millis()+1000;
    char json_body[256];
    unsigned char body_out[256];
    sprintf(json_body, "{\"type\":\"stats\",\"id\":\"%s\",\"data\":{\"temperature\":%.2f,\"humidity\":%.2f,\"ph\":%.2f ,\"ec\":%d,\"do\":%d}}",id,25.09,43.98,6.73,99,25);
    aes_128_encrypt(json_body, KEY, body_out);
    Serial.print("Sending: ");
    Serial.print(json_body);
    udp.beginPacket(multicast, port);
    int bytes = udp.write(body_out, 256);
    udp.endPacket();
    Serial.print("Sent: ");
    Serial.println(bytes);
}

void check_packet(){
    int size = udp.parsePacket();
    if(size > 0){
        char data[256];
        udp.read(data, size);
        String s = String(data);
        int end = s.indexOf("|", 1);
        String cmd = s.substring(1, end);
        Serial.println(cmd);
        if(cmd == String("ack:true:"+myIDStr)){
            ackd = true;
        }
        if(cmd == String("light:on:"+myIDStr)){
            Serial.println("Light ON!");
        }
    }
}

void loop(){
    if(WiFi.ready()){
        check_packet();
        send_stats();
    }else{
        connect_wifi();
    }
}
