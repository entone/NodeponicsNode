#include "application.h"
#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(uint8_t pin){
    _pin = pin;
    _index = 0;
    _total = 0;
    _started = false;
}

void AnalogSensor::init(){
    pinMode(_pin, INPUT);
}

int AnalogSensor::read(){
    if(_index >= NUM_READS){
        if(!_started) _started = true;
        _index = 0;
    }
    int val = analogRead(_pin);
    _total = _total-_avg[_index];
    _avg[_index] = val;
    _total = _total+val;
    _index++;
    if(!_started) return val;
    return _total/NUM_READS;
}
