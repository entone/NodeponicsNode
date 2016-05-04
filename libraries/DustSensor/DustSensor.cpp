#include "application.h"
#include "DustSensor.h"
#include "math.h"


DustSensor::DustSensor(uint8_t pin) {
    _pin = pin;
    _concentration = 0;
}

void DustSensor::begin(void) {
    pinMode(_pin, INPUT);
    _starttime = millis();
}

float DustSensor::read() {
  _duration = pulseIn(_pin, LOW);
  _lowpulseoccupancy = _lowpulseoccupancy + _duration;
  if ((millis()-_starttime) >= _sampletime_ms){
      _ratio = _lowpulseoccupancy/(_sampletime_ms*10.0);
      _concentration = 1.1*pow(_ratio,3)-3.8*pow(_ratio,2)+520*_ratio+0.62; // using spec sheet curve
      _lowpulseoccupancy = 0;
      _starttime = millis();
  }
  return _concentration;
}
