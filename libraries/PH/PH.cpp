#include "application.h"
#include "PH.h"

PH::PH(){
    _data = String();
    _last = 0.0;
}

void PH::init(){
    Serial1.begin(38400);
    Serial1.print("X\r");
    Serial1.print("Z6\r");
    Serial1.print("L1\r");
    Serial1.print("I\r");
}

float PH::read(){
    Serial1.print("R\r");
    if(Serial1.available()){
        while(Serial1.available()){
            char in = (char)Serial1.read();
            if(in == '\r'){
                _last = _data.toFloat();
                _data = String();
                return _last;
            }else{
                _data+= in;
            }
        }
    }
    return _last;
}
