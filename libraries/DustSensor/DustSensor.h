#ifndef DustSensor_h
#define DustSensor_h

#include "application.h"

class DustSensor {
    private:
        unsigned long _duration;
        unsigned long _starttime;
        unsigned long _sampletime_ms = 30000;
        unsigned long _lowpulseoccupancy = 0;
        float _ratio = 0;
        float _concentration = 0;
        uint8_t _pin;
    public:
        DustSensor(uint8_t pin);
        void begin(void);
        float read();
};

#endif
