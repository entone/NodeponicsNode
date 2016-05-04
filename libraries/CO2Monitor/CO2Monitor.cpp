#include "application.h"
#include "CO2Monitor.h"

CO2Monitor::CO2Monitor(){}

void CO2Monitor::init(){
    Serial1.begin(9600);
}

int CO2Monitor::read(void){
    byte data[9];
    int i = 0;
    //transmit command data
    for(i=0; i<sizeof(cmd_get_sensor); i++){
        Serial1.write(cmd_get_sensor[i]);
    }
    delay(10);
    //begin receiving data
    if(Serial1.available()){
        while(Serial1.available()){
            for(int i=0;i<9; i++){
                data[i] = Serial1.read();
            }
        }
    }

    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3]
    + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }
    //temperature = (int)data[4] - 40;
    return (int)data[2] * 256 + (int)data[3];//CO2PPM
}
