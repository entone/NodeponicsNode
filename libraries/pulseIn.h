#ifndef pulseIn_h
#define pulseIn_h

#include "application.h"

unsigned long pulseIn(uint16_t pin, uint8_t state) {
    unsigned long start = millis();
    while (pinReadFast(pin) != state) {
        if((millis()-start) > 5000){
            return 0;
        }
    }
    unsigned long now = micros();
    while (pinReadFast(pin) == state) {
        if((millis()-start) > 5000){
            return 0;
        }
    }
    return micros()-now;
}

#endif
