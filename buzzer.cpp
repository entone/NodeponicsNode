#include "application.h"
#include "libraries/spark-core-mdns/firmware/MDNS.h"

#define HTTP_PORT 80

SYSTEM_MODE(MANUAL);

MDNS mdns;

TCPServer server = TCPServer(HTTP_PORT);

void connect_wifi(){
    while(!WiFi.ready()){
        WiFi.connect();
        Serial.println("Connecting to Wifi...");
        delay(500);
    }
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
    RGB.brightness(0);
    RGB.color(10, 100, 255);
    pinMode(D2, OUTPUT);
    connect_wifi();
    server.begin();
    bool success = mdns.setHostname("buzzer");
    if (success) {
        success = mdns.setService("tcp", "http", HTTP_PORT, "Buzzer");
    }
    if (success) {
        success = mdns.addTXTEntry("coreid", "1");
    }
    if (success) {
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

void loop(){
    if(WiFi.ready()){
        mdns.processQueries();
        TCPClient client = server.available();
        String st;
        while(client.available()){
            char c = client.read();
            st = String(st+c);
        }
        if(client.connected()){
            int bodyPos = st.indexOf("\r\n\r\n");
            String body = st.substring(bodyPos+4);
            Serial.println(body);
            client.write("OK\n\n");
            client.stop();
            if(body == "ON") buzz();
        }
    }else{
        connect_wifi();
    }
}
