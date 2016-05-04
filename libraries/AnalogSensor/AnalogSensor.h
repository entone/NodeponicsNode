#ifndef AnalogSensor_h
#define AnalogSensor_h

#define NUM_READS 10

#include "application.h"

class AnalogSensor{
    public:
        AnalogSensor(uint8_t pin);
        int read();
        void write();
        void init();
    private:
        bool _started;
        int _pin;
        int _index;
        int _avg[NUM_READS];
        int _total;
};

#endif
