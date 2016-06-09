#include "application.h"
#include "libraries/AES/AES.h"
#include "libraries/AnalogSensor/AnalogSensor.h"
#include "libraries/DHT/DHT.h"
#include "libraries/CO2Monitor/CO2Monitor.h"
#include "libraries/HttpClient/HttpClient.h"
#include "libraries/DustSensor/DustSensor.h"
#include "libraries/PH/PH.h"
#include "key.h"
SYSTEM_MODE(MANUAL);

UDP udp;
unsigned int port = 5683;
IPAddress multicast(239,255,41,11);
IPAddress bad_ip(0,0,0,0);
IPAddress candc;

PH ph;
AnalogSensor water_temp(A0);
DHT dht(D4, DHT22);

String myIDStr = Particle.deviceID();
char id[36];

int ackd;
int no_ack = 30000;


void connect_wifi(){
    while(!WiFi.ready() || WiFi.localIP() == bad_ip){
        WiFi.connect();
        Serial.println("Connecting to Wifi...");
        delay(500);
    }
    Serial.println(WiFi.localIP());
}

void fix_connection(){
    Serial.println("Fixing WiFi");
    System.sleep(SLEEP_MODE_DEEP, 5);
    //WiFi.off();
    //delay(10000);
    //WiFi.on();
    //delay(10000);
    //connect_wifi();
}


void setup(){
    Serial.begin(9600);
    Serial.println("beginning");
    RGB.control(true);
    RGB.brightness(100);
    RGB.color(10, 100, 255);
    pinMode(D0, OUTPUT);
    connect_wifi();
    udp.begin(port);
    udp.joinMulticast(multicast);
    myIDStr.toCharArray(id, 36);
    ph.init();
    water_temp.init();
    dht.begin();
    ackd = millis();
    Serial.println("running");
}

void send_packet(char* packet){
    unsigned char body_out[256];
    aes_128_encrypt(packet, KEY, body_out);
    Serial.print("Sending: ");
    Serial.print(packet);
    IPAddress ip = multicast;
    if(candc) ip = candc;
    Serial.print("Sending to: "); Serial.println(ip);
    udp.beginPacket(ip, port);
    int bytes = udp.write(body_out, 256);
    udp.endPacket();
    Serial.print("Sent: ");
    Serial.println(bytes);
}

void check_ack(){
    if((millis()-ackd) > no_ack){
        fix_connection();
    }
}

unsigned int next = 0;

void send_stats(){
    if(millis() < next) return;
    next = millis()+1000;
    char json_body[256];
    int adc = water_temp.read();
    Serial.println(adc);
    float voltage = adc * 3.3;
    voltage /= 4096.0;
    float wt = (voltage-0.5)*100;
    float p = ph.read();
    float t = dht.readTemperature(false);
    float h = dht.readHumidity();
    Serial.println(wt);
    Serial.println(p);
    sprintf(json_body, "{\"type\":\"stats\",\"id\":\"%s\",\"data\":{\"temperature\":%.2f,\"humidity\":%.2f,\"ph\":%.2f ,\"ec\":%d,\"do\":%d}}",id,t,h,p,0,0);
    send_packet(json_body);
}

void check_packet(){
    int size = udp.parsePacket();
    if(size > 0){
        char data[256];
        udp.read(data, size);
        String s = String(data);
        int end = s.indexOf("\n", 0);
        String cmd = s.substring(0, end);
        Serial.println(cmd);
        if(cmd == String("ack:true:"+myIDStr)){
            candc = udp.remoteIP();
            ackd = millis();
        }
        if(cmd == String("light:on:"+myIDStr)){
            digitalWrite(D0, HIGH);
            Serial.println("Light ON!");
            char json_body[256];
            sprintf(json_body, "{\"type\":\"lighton\",\"id\":\"%s\"}",id);
            send_packet(json_body);
        }
        if(cmd == String("light:off:"+myIDStr)){
            digitalWrite(D0, LOW);
            Serial.println("Light OFF!");
            char json_body[256];
            sprintf(json_body, "{\"type\":\"lightoff\",\"id\":\"%s\"}",id);
            send_packet(json_body);
        }
        if(cmd == String("light:off:"+myIDStr)){
            digitalWrite(D0, LOW);
            Serial.println("Light OFF!");
        }
    }
}

void loop(){
    if(WiFi.localIP() == bad_ip){
        connect_wifi();
    }else{
        check_packet();
        send_stats();
        check_ack();
    }
}
